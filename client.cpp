#include "client.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QRegularExpression>

Client::Client(Ui::MainWindow *ui, QObject *parent) : QObject(parent), ui(ui) {
    // 1. Initialisation du Modèle SQL
    model = new QSqlTableModel(this, QSqlDatabase::database(CONNECTION_NAME));
    model->setTable("CLIENT");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);

    // 2. Lier le modèle au QTableView
    ui->tab_clients->setModel(model);
    ui->tab_clients->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab_clients->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 3. Titres des colonnes (Synchronisés avec le formulaire)
    // Ordre Oracle : 0:ID, 1:NOM, 2:VILLE, 3:CP, 4:POINTS, 5:TEL, 6:PDG, 7:ADRESSE
    model->setHeaderData(0, Qt::Horizontal, "ID Client");
    model->setHeaderData(1, Qt::Horizontal, "Nom");
    model->setHeaderData(5, Qt::Horizontal, "Num Tél");
    model->setHeaderData(7, Qt::Horizontal, "Adresse");
    model->setHeaderData(2, Qt::Horizontal, "Ville");
    model->setHeaderData(3, Qt::Horizontal, "Code Postal");
    model->setHeaderData(6, Qt::Horizontal, "Responsable");

    rafraichirAffichage();

    // 4. CONFIGURATION DE L'AFFICHAGE (Masquer ce qui n'est pas dans le formulaire)
    ui->tab_clients->hideColumn(4); // Masquer la colonne "Points de fidélité"

    // Optimisation visuelle
    ui->tab_clients->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // 5. Connexions des signaux
    connect(ui->btn_ajouter_client, &QPushButton::clicked, this, &Client::onBtnAjouterClicked);
    connect(ui->btn_modifier_client, &QPushButton::clicked, this, &Client::onBtnModifierClicked);
    connect(ui->btn_supprimer_client, &QPushButton::clicked, this, &Client::onBtnSupprimerClicked);
    connect(ui->tab_clients, &QTableView::clicked, this, &Client::onTableClicked);
    connect(ui->le_recherche_client, &QLineEdit::textChanged, this, &Client::onRechercheTextChanged);
}

void Client::rafraichirAffichage() {
    model->select();
}

// ==================== CONTROLE DE SAISIE LOGIQUE ====================
bool Client::verifierSaisie() {
    QString id = ui->le_id_client->text();
    QString nom = ui->le_nom_client->text();
    QString tel = ui->le_tel_client->text();
    QString adr = ui->le_adresse_client->text();
    QString responsable = ui->le_responsable_client->text();
    QString cp = ui->dsb_codepostal_client->text();

    // A. Vérification des champs vides
    if(id.isEmpty() || nom.isEmpty() || tel.isEmpty() || adr.isEmpty() || responsable.isEmpty() || cp.isEmpty()) {
        QMessageBox::warning(nullptr, "Champs vides", "Veuillez remplir tous les champs du formulaire.");
        return false;
    }

    // B. Vérification du Nom (Lettres et espaces uniquement)
    QRegularExpression rxNom("^[A-Za-zÀ-ÿ\\s]+$");
    if(!rxNom.match(nom).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur Nom", "Le nom ne doit contenir que des lettres.");
        return false;
    }

    // C. Vérification du Téléphone (Exactement 8 chiffres)
    QRegularExpression rxTel("^[0-9]{8}$");
    if(!rxTel.match(tel).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur Téléphone", "Le numéro de téléphone doit contenir 8 chiffres.");
        return false;
    }

    // D. Vérification de l'ID (Doit être un nombre positif)
    if(id.toInt() <= 0) {
        QMessageBox::warning(nullptr, "Erreur ID", "L'ID doit être un nombre positif.");
        return false;
    }

    return true;
}

// ==================== ACTIONS CRUD ====================

void Client::onBtnAjouterClicked() {
    if(!verifierSaisie()) return; // Blocage si erreur de saisie

    QSqlQuery query(QSqlDatabase::database(CONNECTION_NAME));
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
        QMessageBox::critical(nullptr, "Erreur", "L'ID existe déjà dans la base.");
    }
}

void Client::onBtnModifierClicked() {
    if(!verifierSaisie()) return; // Contrôle de saisie aussi présent pour la modif

    QSqlQuery query(QSqlDatabase::database(CONNECTION_NAME));
    query.prepare("UPDATE CLIENT SET NOM_CLIENT=:nom, VILLE=:ville, CODE_POSTAL=:cp, NUM_TEL=:tel, PDG=:pdg, ADRESSE=:adr WHERE ID_CLIENT=:id");

    query.bindValue(":id", ui->le_id_client->text().toInt());
    query.bindValue(":nom", ui->le_nom_client->text());
    query.bindValue(":ville", ui->cb_ville_client->currentText());
    query.bindValue(":cp", ui->dsb_codepostal_client->text());
    query.bindValue(":tel", ui->le_tel_client->text());
    query.bindValue(":pdg", ui->le_responsable_client->text());
    query.bindValue(":adr", ui->le_adresse_client->text());

    if(query.exec()) {
        QMessageBox::information(nullptr, "Succès", "Client modifié avec succès.");
        rafraichirAffichage();
    }
}

void Client::onBtnSupprimerClicked() {
    int row = ui->tab_clients->currentIndex().row();
    if(row < 0) {
        QMessageBox::warning(nullptr, "Sélection", "Veuillez sélectionner un client dans le tableau.");
        return;
    }

    int id = model->data(model->index(row, 0)).toInt();
    if(QMessageBox::question(nullptr, "Suppression", "Voulez-vous supprimer ce client ?", QMessageBox::Yes|QMessageBox::No) == QMessageBox::Yes) {
        QSqlQuery query(QSqlDatabase::database(CONNECTION_NAME));
        query.prepare("DELETE FROM CLIENT WHERE ID_CLIENT = :id");
        query.bindValue(":id", id);
        if(query.exec()) rafraichirAffichage();
    }
}

void Client::onTableClicked(const QModelIndex &index) {
    int r = index.row();
    // On remplit les 7 champs du formulaire
    ui->le_id_client->setText(model->data(model->index(r, 0)).toString());
    ui->le_nom_client->setText(model->data(model->index(r, 1)).toString());
    ui->cb_ville_client->setCurrentText(model->data(model->index(r, 2)).toString());
    ui->dsb_codepostal_client->setText(model->data(model->index(r, 3)).toString());
    ui->le_tel_client->setText(model->data(model->index(r, 5)).toString());
    ui->le_responsable_client->setText(model->data(model->index(r, 6)).toString());
    ui->le_adresse_client->setText(model->data(model->index(r, 7)).toString());
}

void Client::onRechercheTextChanged(const QString &text) {
    model->setFilter(QString("UPPER(NOM_CLIENT) LIKE UPPER('%%1%')").arg(text));
    rafraichirAffichage();
}
