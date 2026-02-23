#include "employe.h"
#include "ui_mainwindow.h"   // Full definition needed in the .cpp
#include "connection.h"      // For CONNECTION_NAME

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QDate>
#include <QRegularExpression>

// ==================== CONSTRUCTOR ====================

Employe::Employe(Ui::MainWindow *ui, QObject *parent)
    : QObject(parent), ui(ui), employeModel(nullptr)
{
    // Load the table data
    afficherEmployes();

    // Manually connect UI signals to this object's slots
    // (replaces Qt's auto-connect which only works for slots on the form's own class)
    connect(ui->tab_employes,   &QTableView::clicked,
            this, &Employe::onTabEmployesClicked);

    connect(ui->btn_ajouter,    &QPushButton::clicked,
            this, &Employe::onBtnAjouterClicked);

    connect(ui->btn_modifier,   &QPushButton::clicked,
            this, &Employe::onBtnModifierClicked);

    connect(ui->btn_supprimer,  &QPushButton::clicked,
            this, &Employe::onBtnSupprimerClicked);

    connect(ui->le_recherche,   &QLineEdit::textChanged,
            this, &Employe::onLeRechercheTextChanged);

    connect(ui->btn_tri,        &QPushButton::clicked,
            this, &Employe::onBtnTriClicked);
}

// ==================== DISPLAY / LOAD ====================

void Employe::afficherEmployes()
{
    QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);

    // Insert dummy data if the table is empty (for testing)
    QSqlQuery query(db);
    query.exec("SELECT COUNT(*) FROM EMPLOYE");
    if (query.next() && query.value(0).toInt() == 0) {
        qDebug() << "Table EMPLOYE is empty. Inserting dummy data...";

        query.exec("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE) "
                   "VALUES (11111111, 'Ben Ali', 'Ahmed', 'Manager', 2500.50, TO_DATE('2023-01-15', 'YYYY-MM-DD'), 'B-001')");

        query.exec("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE) "
                   "VALUES (22222222, 'Trabelsi', 'Sami', 'Technicien', 1800.00, TO_DATE('2023-06-20', 'YYYY-MM-DD'), 'B-002')");

        query.exec("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE) "
                   "VALUES (33333333, 'Mansour', 'Sarra', 'Ingenieur', 3200.75, TO_DATE('2024-02-10', 'YYYY-MM-DD'), 'B-003')");

        db.commit();
    }

    employeModel = new QSqlTableModel(this, db);
    employeModel->setTable("EMPLOYE");
    employeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    employeModel->select();

    // Human-readable column headers
    employeModel->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    employeModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    employeModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    employeModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Poste"));
    employeModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Salaire (TND)"));
    employeModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Date d'embauche"));
    employeModel->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Badge"));

    ui->tab_employes->setModel(employeModel);
    ui->tab_employes->setAlternatingRowColors(false);
    ui->tab_employes->resizeColumnsToContents();
}

// ==================== SHARED VALIDATION ====================

bool Employe::validerFormulaire(const QString &cin, const QString &nom,
                                const QString &prenom, const QString &idBadge,
                                double salaire)
{
    QRegularExpression cinRegex("^[0-9]{8}$");
    if (!cinRegex.match(cin).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de saisie",
                             "Le CIN doit contenir exactement 8 chiffres.");
        return false;
    }

    QRegularExpression nameRegex("^[A-Za-zÀ-ÿ\\s]+$");
    if (nom.isEmpty() || nom.length() > 50 || !nameRegex.match(nom).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de saisie",
                             "Le nom est invalide (lettres uniquement, max 50 caractères).");
        return false;
    }
    if (prenom.isEmpty() || prenom.length() > 50 || !nameRegex.match(prenom).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de saisie",
                             "Le prénom est invalide (lettres uniquement, max 50 caractères).");
        return false;
    }

    if (idBadge.isEmpty() || idBadge.length() > 20) {
        QMessageBox::warning(nullptr, "Erreur de saisie",
                             "L'ID Badge ne peut pas être vide et doit faire max 20 caractères.");
        return false;
    }

    if (salaire <= 0) {
        QMessageBox::warning(nullptr, "Erreur de saisie",
                             "Le salaire doit être supérieur à 0.");
        return false;
    }

    return true;
}

// ==================== TABLE CLICK (populate form) ====================

void Employe::onTabEmployesClicked(const QModelIndex &index)
{
    int row = index.row();

    ui->le_cin->setText(employeModel->data(employeModel->index(row, 0)).toString());
    ui->le_nom->setText(employeModel->data(employeModel->index(row, 1)).toString());
    ui->le_prenom->setText(employeModel->data(employeModel->index(row, 2)).toString());
    ui->cb_poste->setCurrentText(employeModel->data(employeModel->index(row, 3)).toString());
    ui->dsb_salaire->setValue(employeModel->data(employeModel->index(row, 4)).toDouble());
    ui->de_embauche->setDate(employeModel->data(employeModel->index(row, 5)).toDate());
    ui->le_badge->setText(employeModel->data(employeModel->index(row, 6)).toString());
}

// ==================== ADD ====================

void Employe::onBtnAjouterClicked()
{
    QString cin      = ui->le_cin->text();
    QString nom      = ui->le_nom->text();
    QString prenom   = ui->le_prenom->text();
    QString poste    = ui->cb_poste->currentText();
    double  salaire  = ui->dsb_salaire->value();
    QDate   dateEmb  = ui->de_embauche->date();
    QString idBadge  = ui->le_badge->text();

    if (!validerFormulaire(cin, nom, prenom, idBadge, salaire))
        return;

    QSqlQuery query(QSqlDatabase::database(CONNECTION_NAME));
    query.prepare("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE) "
                  "VALUES (:cin, :nom, :prenom, :poste, :salaire, :date_embauche, :id_badge)");
    query.bindValue(":cin",          cin);
    query.bindValue(":nom",          nom);
    query.bindValue(":prenom",       prenom);
    query.bindValue(":poste",        poste);
    query.bindValue(":salaire",      salaire);
    query.bindValue(":date_embauche", dateEmb);
    query.bindValue(":id_badge",     idBadge);

    if (query.exec()) {
        QMessageBox::information(nullptr, "Succès", "Employé ajouté avec succès.");
        employeModel->select();
    } else {
        QMessageBox::critical(nullptr, "Erreur",
                              "Erreur lors de l'ajout: " + query.lastError().text());
    }
}

// ==================== EDIT ====================

void Employe::onBtnModifierClicked()
{
    QString cin      = ui->le_cin->text();
    QString nom      = ui->le_nom->text();
    QString prenom   = ui->le_prenom->text();
    QString poste    = ui->cb_poste->currentText();
    double  salaire  = ui->dsb_salaire->value();
    QDate   dateEmb  = ui->de_embauche->date();
    QString idBadge  = ui->le_badge->text();

    if (!validerFormulaire(cin, nom, prenom, idBadge, salaire))
        return;

    QSqlQuery query(QSqlDatabase::database(CONNECTION_NAME));
    query.prepare("UPDATE EMPLOYE SET NOM = :nom, PRENOM = :prenom, POSTE = :poste, "
                  "SALAIRE = :salaire, DATE_EMBAUCHE = :date_embauche, ID_BADGE = :id_badge "
                  "WHERE CIN = :cin");
    query.bindValue(":nom",          nom);
    query.bindValue(":prenom",       prenom);
    query.bindValue(":poste",        poste);
    query.bindValue(":salaire",      salaire);
    query.bindValue(":date_embauche", dateEmb);
    query.bindValue(":id_badge",     idBadge);
    query.bindValue(":cin",          cin);

    if (query.exec()) {
        QMessageBox::information(nullptr, "Succès", "Employé modifié avec succès.");
        employeModel->select();
    } else {
        QMessageBox::critical(nullptr, "Erreur",
                              "Erreur lors de la modification: " + query.lastError().text());
    }
}

// ==================== DELETE ====================

void Employe::onBtnSupprimerClicked()
{
    QString cin = ui->le_cin->text();

    if (cin.isEmpty()) {
        QMessageBox::warning(nullptr, "Avertissement",
                             "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    QSqlQuery query(QSqlDatabase::database(CONNECTION_NAME));
    query.prepare("DELETE FROM EMPLOYE WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (query.exec()) {
        QMessageBox::information(nullptr, "Succès", "Employé supprimé avec succès.");

        // Clear the form
        ui->le_cin->clear();
        ui->le_nom->clear();
        ui->le_prenom->clear();
        ui->cb_poste->setCurrentIndex(0);
        ui->dsb_salaire->setValue(0.0);
        ui->de_embauche->setDate(QDate::currentDate());
        ui->le_badge->clear();

        employeModel->select();
    } else {
        QMessageBox::critical(nullptr, "Erreur",
                              "Erreur lors de la suppression: " + query.lastError().text());
    }
}

// ==================== SEARCH ====================

void Employe::onLeRechercheTextChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        employeModel->setFilter("");
    } else {
        QString filter = QString(
            "(TO_CHAR(CIN) LIKE '%%1%' OR UPPER(NOM) LIKE UPPER('%%1%') OR UPPER(ID_BADGE) LIKE UPPER('%%1%'))"
        ).arg(arg1);
        employeModel->setFilter(filter);
    }
    employeModel->select();
}

// ==================== SORT ====================

void Employe::onBtnTriClicked()
{
    QString currentSort = ui->comboBox->currentText();

    if (currentSort == "CIN") {
        employeModel->setSort(0, Qt::AscendingOrder);
    } else if (currentSort == "Nom") {
        employeModel->setSort(1, Qt::AscendingOrder);
    } else if (currentSort == "ID Badge") {
        employeModel->setSort(6, Qt::AscendingOrder);
    }

    employeModel->select();
}
