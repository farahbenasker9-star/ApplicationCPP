#include "client.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>

Client::Client(Ui::MainWindow *ui, QObject *parent) : QObject(parent), ui(ui) {
    // 1. Modèle en lecture pour limiter les colonnes (évite les doublons visibles)
    model = new QSqlQueryModel(this);

    // 2. Lier le modèle au QTableView
    ui->tab_clients->setModel(model);
    ui->tab_clients->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab_clients->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tab_clients->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    rafraichirAffichage();

    // Connexions
    connect(ui->btn_ajouter_client, &QPushButton::clicked, this, &Client::onBtnAjouterClicked);
    connect(ui->btn_modifier_client, &QPushButton::clicked, this, &Client::onBtnModifierClicked);
    connect(ui->btn_supprimer_client, &QPushButton::clicked, this, &Client::onBtnSupprimerClicked);
    connect(ui->tab_clients, &QTableView::clicked, this, &Client::onTableClicked);
    connect(ui->le_recherche_client, &QLineEdit::textChanged, this, &Client::onRechercheTextChanged);
}

void Client::rafraichirAffichage() {
    QString sql = "SELECT ID_CLIENT, NOM_CLIENT, VILLE, CODE_POSTAL, NUM_TEL, PDG, ADRESSE FROM CLIENT";
    if (!currentFilter.isEmpty()) {
        sql += " WHERE " + currentFilter;
    }
    model->setQuery(sql, QSqlDatabase::database());

    model->setHeaderData(0, Qt::Horizontal, "ID Client");
    model->setHeaderData(1, Qt::Horizontal, "Nom Client");
    model->setHeaderData(2, Qt::Horizontal, "Ville");
    model->setHeaderData(3, Qt::Horizontal, "Code Postal");
    model->setHeaderData(4, Qt::Horizontal, "Num Tel");
    model->setHeaderData(5, Qt::Horizontal, "Responsable");
    model->setHeaderData(6, Qt::Horizontal, "Adresse");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur requête Client:" << model->lastError().text();
    }
}

bool Client::verifierSaisie() {
    QString id = ui->le_id_client->text();
    QString nom = ui->le_nom_client->text();
    QString tel = ui->le_tel_client->text();
    QString adr = ui->le_adresse_client->text();
    QString responsable = ui->le_responsable_client->text();
    QString cp = ui->dsb_codepostal_client->text();

    if(id.isEmpty() || nom.isEmpty() || tel.isEmpty() || adr.isEmpty() || responsable.isEmpty() || cp.isEmpty()) {
        QMessageBox::warning(nullptr, "Champs vides", "Veuillez remplir tous les champs du formulaire.");
        return false;
    }

    QRegularExpression rxNom("^[A-Za-zÀ-ÿ\\s]+$");
    if(!rxNom.match(nom).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur Nom", "Le nom ne doit contenir que des lettres.");
        return false;
    }

    QRegularExpression rxTel("^[0-9]{8}$");
    if(!rxTel.match(tel).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur Téléphone", "Le numéro de téléphone doit contenir 8 chiffres.");
        return false;
    }

    if(id.toInt() <= 0) {
        QMessageBox::warning(nullptr, "Erreur ID", "L'ID doit être un nombre positif.");
        return false;
    }

    QRegularExpression rxCP("^[0-9]+$");
    if(!rxCP.match(cp).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur Code Postal", "Le code postal doit être numérique.");
        return false;
    }

    return true;
}

void Client::onBtnAjouterClicked() {
    if(!verifierSaisie()) return;

    QSqlQuery query;
    query.prepare("INSERT INTO CLIENT (ID_CLIENT, NOM_CLIENT, VILLE, CODE_POSTAL, NUM_TEL, PDG, ADRESSE, POINT_DE_FIDELITE) "
                  "VALUES (:id, :nom, :ville, :cp, :tel, :pdg, :adr, 0)");

    query.bindValue(":id", ui->le_id_client->text().toInt());
    query.bindValue(":nom", ui->le_nom_client->text());
    query.bindValue(":ville", ui->cb_ville_client->currentText());
    query.bindValue(":cp", ui->dsb_codepostal_client->text());
    query.bindValue(":tel", ui->le_tel_client->text());
    query.bindValue(":pdg", ui->le_responsable_client->text());
    query.bindValue(":adr", ui->le_adresse_client->text());

    if(query.exec()) {
        QMessageBox::information(nullptr, "Succès", "Client ajouté avec succès !");
        rafraichirAffichage();
    } else {
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Erreur lors de l'ajout :\n" + query.lastError().text());
        qDebug() << "Erreur INSERT Client:" << query.lastError().text();
    }
}

void Client::onBtnModifierClicked() {
    if(!verifierSaisie()) return;

    if (selectedClientId == -1) {
        QMessageBox::warning(nullptr, "Sélection requise", "Veuillez sélectionner un client dans le tableau avant de modifier.");
        return;
    }

    int newId = ui->le_id_client->text().toInt();
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.transaction()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de démarrer la transaction :\n" + db.lastError().text());
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE CLIENT SET ID_CLIENT=:newId, NOM_CLIENT=:nom, VILLE=:ville, CODE_POSTAL=:cp, NUM_TEL=:tel, PDG=:pdg, ADRESSE=:adr WHERE ID_CLIENT=:oldId");

    query.bindValue(":newId", newId);
    query.bindValue(":oldId", selectedClientId);
    query.bindValue(":nom", ui->le_nom_client->text());
    query.bindValue(":ville", ui->cb_ville_client->currentText());
    query.bindValue(":cp", ui->dsb_codepostal_client->text());
    query.bindValue(":tel", ui->le_tel_client->text());
    query.bindValue(":pdg", ui->le_responsable_client->text());
    query.bindValue(":adr", ui->le_adresse_client->text());

    if(!query.exec()) {
        db.rollback();
        QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Erreur lors de la modification :\n" + query.lastError().text());
        return;
    }

    if (!reaffecterIdClientDansRelations(selectedClientId, newId)) {
        db.rollback();
        return;
    }

    if (!db.commit()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de valider la transaction :\n" + db.lastError().text());
        return;
    }

    selectedClientId = newId;
    QMessageBox::information(nullptr, "Succès", "Client modifié avec succès.");
    rafraichirAffichage();
}

void Client::onBtnSupprimerClicked() {
    int row = ui->tab_clients->currentIndex().row();
    if(row < 0) {
        QMessageBox::warning(nullptr, "Sélection", "Veuillez sélectionner un client dans le tableau.");
        return;
    }

    int id = model->data(model->index(row, 0)).toInt();

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CONTRAT WHERE ID_CLIENT = :id");
    checkQuery.bindValue(":id", id);
    checkQuery.exec();
    checkQuery.next();
    int count = checkQuery.value(0).toInt();

    QString msg = "Voulez-vous supprimer ce client ?";
    if (count > 0) {
        msg = QString("Ce client est lié à %1 contrat(s).\n"
                      "Si vous le supprimez, ces contrats deviendront 'non-assignés'.\n\n"
                      "Confirmer la suppression ?").arg(count);
    }

    if (QMessageBox::question(nullptr, "Suppression", msg, QMessageBox::Yes|QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    if (count > 0) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE CONTRAT SET ID_CLIENT = NULL WHERE ID_CLIENT = :id");
        updateQuery.bindValue(":id", id);
        if (!updateQuery.exec()) {
            QMessageBox::critical(nullptr, "Erreur", "Impossible de détacher les contrats : " + updateQuery.lastError().text());
            return;
        }
    }

    QSqlQuery query;
    query.prepare("DELETE FROM CLIENT WHERE ID_CLIENT = :id");
    query.bindValue(":id", id);
    if(query.exec()) {
        rafraichirAffichage();
        QMessageBox::information(nullptr, "Succès", "Client supprimé avec succès.");
    } else {
         QMessageBox::critical(nullptr, "Erreur de base de données",
                              "Erreur lors de la suppression :\n" + query.lastError().text());
    }
}

void Client::onTableClicked(const QModelIndex &index) {
    int r = index.row();
    selectedClientId = model->data(model->index(r, 0)).toInt();
    ui->le_id_client->setText(model->data(model->index(r, 0)).toString());
    ui->le_nom_client->setText(model->data(model->index(r, 1)).toString());
    ui->cb_ville_client->setCurrentText(model->data(model->index(r, 2)).toString());
    ui->dsb_codepostal_client->setText(model->data(model->index(r, 3)).toString());
    ui->le_tel_client->setText(model->data(model->index(r, 4)).toString());
    ui->le_responsable_client->setText(model->data(model->index(r, 5)).toString());
    ui->le_adresse_client->setText(model->data(model->index(r, 6)).toString());
}

void Client::onRechercheTextChanged(const QString &text) {
    if (text.isEmpty()) {
        currentFilter.clear();
    } else {
        currentFilter = QString("UPPER(NOM_CLIENT) LIKE UPPER('%%1%')").arg(text);
    }
    rafraichirAffichage();
}

bool Client::reaffecterIdClientDansRelations(int oldId, int newId) {
    QSqlQuery q(QSqlDatabase::database());

    // Contrats
    q.prepare("UPDATE CONTRAT SET ID_CLIENT = :newId WHERE ID_CLIENT = :oldId");
    q.bindValue(":newId", newId);
    q.bindValue(":oldId", oldId);
    if (!q.exec()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Échec de la mise à jour des contrats :\n" + q.lastError().text());
        return false;
    }

    // Produits
    q.prepare("UPDATE PRODUIT SET ID_CLIENT = :newId WHERE ID_CLIENT = :oldId");
    q.bindValue(":newId", newId);
    q.bindValue(":oldId", oldId);
    if (!q.exec()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Échec de la mise à jour des produits :\n" + q.lastError().text());
        return false;
    }

    return true;
}
