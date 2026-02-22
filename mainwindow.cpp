#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "connection.h" // To access CONNECTION_NAME
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QDate>
#include <QRegularExpression> // Required for input validation

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Set landing page to Dashboard (Index 0)
    ui->stackedWidget->setCurrentIndex(0);
    ui->navbar_container->hide(); // Hide sidebar on dashboard initially

    // Initialize the employee model and display data
    afficherEmployes();

    // ==================== DASHBOARD CONNECTIONS ====================
    connect(ui->btn_dash_employe, &QPushButton::clicked, this, [this]() { navigateToPage(6); ui->btn_nav_employes->setChecked(true); });
    connect(ui->btn_dash_client, &QPushButton::clicked, this, [this]() { navigateToPage(5); ui->btn_nav_client->setChecked(true); });
    connect(ui->btn_dash_contrat, &QPushButton::clicked, this, [this]() { navigateToPage(2); ui->btn_nav_contrat->setChecked(true); });
    connect(ui->btn_dash_poubelle, &QPushButton::clicked, this, [this]() { navigateToPage(7); ui->btn_nav_poubelle->setChecked(true); });
    connect(ui->btn_dash_equipement, &QPushButton::clicked, this, [this]() { navigateToPage(3); ui->btn_nav_equipements->setChecked(true); });
    connect(ui->btn_dash_stock, &QPushButton::clicked, this, [this]() { navigateToPage(4); ui->btn_nav_stock->setChecked(true); });


    // ==================== SIDEBAR VISIBILITY ====================
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [this](int index) {
        if (index == 0) { // Dashboard
            ui->navbar_container->hide();
        } else {
            ui->navbar_container->show();
        }
    });

    // ==================== NAVIGATION (SIDEBAR) ====================
    connect(ui->btn_nav_employes, &QPushButton::clicked, this, [this]() {
        navigateToPage(6); // Page Employee
        ui->btn_nav_employes->setChecked(true);
    });

    connect(ui->btn_nav_client, &QPushButton::clicked, this, [this]() {
        navigateToPage(5); // Page Client
        ui->btn_nav_client->setChecked(true);
    });

    connect(ui->btn_nav_poubelle, &QPushButton::clicked, this, [this]() {
        navigateToPage(7); // Page Poubelle
        ui->btn_nav_poubelle->setChecked(true);
    });

    connect(ui->btn_nav_contrat, &QPushButton::clicked, this, [this]() {
        navigateToPage(2); // Page Contrat
        ui->btn_nav_contrat->setChecked(true);
    });

    connect(ui->btn_nav_stock, &QPushButton::clicked, this, [this]() {
        navigateToPage(4); // Page Stock
        ui->btn_nav_stock->setChecked(true);
    });

    connect(ui->btn_nav_equipements, &QPushButton::clicked, this, [this]() {
        navigateToPage(3); // Page Equipement
        ui->btn_nav_equipements->setChecked(true);
    });
    
    // Connect Quit button to close application
    connect(ui->btn_quit, &QPushButton::clicked, this, &QMainWindow::close);


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btn_nav_employes_clicked()
{
    // Pour l'instant, ce bouton ne fait rien, mais le slot est là pour que ça compile
    // On pourra rajouter : ui->stackedWidget->setCurrentIndex(0); plus tard
}
// ==================== NAVIGATION ====================

void MainWindow::navigateToPage(int pageIndex)
{
    ui->stackedWidget->setCurrentIndex(pageIndex);
}

// ==================== EMPLOYEES MANAGEMENT ====================

void MainWindow::afficherEmployes()
{
    // 1. Get the active database connection we created in main.cpp
    QSqlDatabase db = QSqlDatabase::database(CONNECTION_NAME);
    
    // 2. Insert dummy data if the table is empty (just for testing)
    QSqlQuery query(db);
    query.exec("SELECT COUNT(*) FROM EMPLOYE");
    if (query.next() && query.value(0).toInt() == 0) {
        qDebug() << "Table EMPLOYE is empty. Inserting dummy data...";
        
        // Using TO_DATE for Oracle date format compatibility
        query.exec("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE) "
                   "VALUES (11111111, 'Ben Ali', 'Ahmed', 'Manager', 2500.50, TO_DATE('2023-01-15', 'YYYY-MM-DD'), 'B-001')");
                   
        query.exec("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE) "
                   "VALUES (22222222, 'Trabelsi', 'Sami', 'Technicien', 1800.00, TO_DATE('2023-06-20', 'YYYY-MM-DD'), 'B-002')");
                   
        query.exec("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE) "
                   "VALUES (33333333, 'Mansour', 'Sarra', 'Ingenieur', 3200.75, TO_DATE('2024-02-10', 'YYYY-MM-DD'), 'B-003')");
                   
        // Commit the changes to the database
        db.commit();
    }

    // 3. Initialize the QSqlTableModel
    // We pass 'this' so Qt handles memory management automatically
    employeModel = new QSqlTableModel(this, db);
    
    // 4. Link the model to the EMPLOYE table
    employeModel->setTable("EMPLOYE");
    
    // 5. Set the edit strategy (OnManualSubmit is best for CRUD apps)
    // This means changes in the table view won't save to DB until we call submitAll()
    employeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    
    // 6. Fetch the data from the database
    employeModel->select();
    
    // 7. Rename the columns to be more user-friendly (Optional but recommended)
    employeModel->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    employeModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    employeModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    employeModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Poste"));
    employeModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Salaire (TND)"));
    employeModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Date d'embauche"));
    employeModel->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Badge"));

    // 8. Link the model to the QTableView in the UI
    ui->tab_employes->setModel(employeModel);
    
    // Disable alternating row colors as requested
    ui->tab_employes->setAlternatingRowColors(false);
    
    // 9. Adjust column sizes to fit the content
    ui->tab_employes->resizeColumnsToContents();
}

// ==================== CRUD EMPLOYEES ====================

void MainWindow::on_tab_employes_clicked(const QModelIndex &index)
{
    // Get the row that was clicked
    int row = index.row();

    // Extract data from the model for the selected row
    QString cin = employeModel->data(employeModel->index(row, 0)).toString();
    QString nom = employeModel->data(employeModel->index(row, 1)).toString();
    QString prenom = employeModel->data(employeModel->index(row, 2)).toString();
    QString poste = employeModel->data(employeModel->index(row, 3)).toString();
    double salaire = employeModel->data(employeModel->index(row, 4)).toDouble();
    QDate dateEmbauche = employeModel->data(employeModel->index(row, 5)).toDate();
    QString idBadge = employeModel->data(employeModel->index(row, 6)).toString();

    // Fill the form inputs
    ui->le_cin->setText(cin);
    ui->le_nom->setText(nom);
    ui->le_prenom->setText(prenom);
    ui->cb_poste->setCurrentText(poste);
    ui->dsb_salaire->setValue(salaire);
    ui->de_embauche->setDate(dateEmbauche);
    ui->le_badge->setText(idBadge);
}

void MainWindow::on_btn_ajouter_clicked()
{
    // 1. Get data from the form
    QString cin = ui->le_cin->text();
    QString nom = ui->le_nom->text();
    QString prenom = ui->le_prenom->text();
    QString poste = ui->cb_poste->currentText();
    double salaire = ui->dsb_salaire->value();
    QDate dateEmbauche = ui->de_embauche->date();
    QString idBadge = ui->le_badge->text();

    // ==================== INPUT VALIDATION ====================
    
    // Rule 1: CIN must be exactly 8 digits
    QRegularExpression cinRegex("^[0-9]{8}$");
    if (!cinRegex.match(cin).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le CIN doit contenir exactement 8 chiffres.");
        return; // Stop execution here
    }

    // Rule 2: Nom & Prenom cannot be empty, only letters, max 50 chars
    QRegularExpression nameRegex("^[A-Za-zÀ-ÿ\\s]+$"); // Allows letters and spaces
    if (nom.isEmpty() || nom.length() > 50 || !nameRegex.match(nom).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le nom est invalide (lettres uniquement, max 50 caractères).");
        return;
    }
    if (prenom.isEmpty() || prenom.length() > 50 || !nameRegex.match(prenom).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le prénom est invalide (lettres uniquement, max 50 caractères).");
        return;
    }

    // Rule 3: ID Badge cannot be empty, max 20 chars
    if (idBadge.isEmpty() || idBadge.length() > 20) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID Badge ne peut pas être vide et doit faire max 20 caractères.");
        return;
    }

    // Rule 4: Salaire must be greater than 0
    if (salaire <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le salaire doit être supérieur à 0.");
        return;
    }

    // ==========================================================

    // 2. Prepare the SQL query
    QSqlQuery query(QSqlDatabase::database(CONNECTION_NAME));
    query.prepare("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE) "
                  "VALUES (:cin, :nom, :prenom, :poste, :salaire, :date_embauche, :id_badge)");

    // 3. Bind the values to prevent SQL injection
    query.bindValue(":cin", cin);
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":date_embauche", dateEmbauche);
    query.bindValue(":id_badge", idBadge);

    // 4. Execute the query and check for success
    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès.");
        employeModel->select(); // Refresh the table
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout: " + query.lastError().text());
    }
}

void MainWindow::on_btn_modifier_clicked()
{
    // 1. Get data from the form
    QString cin = ui->le_cin->text();
    QString nom = ui->le_nom->text();
    QString prenom = ui->le_prenom->text();
    QString poste = ui->cb_poste->currentText();
    double salaire = ui->dsb_salaire->value();
    QDate dateEmbauche = ui->de_embauche->date();
    QString idBadge = ui->le_badge->text();

    // ==================== INPUT VALIDATION ====================
    
    // Rule 1: CIN must be exactly 8 digits
    QRegularExpression cinRegex("^[0-9]{8}$");
    if (!cinRegex.match(cin).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le CIN doit contenir exactement 8 chiffres.");
        return; // Stop execution here
    }

    // Rule 2: Nom & Prenom cannot be empty, only letters, max 50 chars
    QRegularExpression nameRegex("^[A-Za-zÀ-ÿ\\s]+$"); // Allows letters and spaces
    if (nom.isEmpty() || nom.length() > 50 || !nameRegex.match(nom).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le nom est invalide (lettres uniquement, max 50 caractères).");
        return;
    }
    if (prenom.isEmpty() || prenom.length() > 50 || !nameRegex.match(prenom).hasMatch()) {
        QMessageBox::warning(this, "Erreur de saisie", "Le prénom est invalide (lettres uniquement, max 50 caractères).");
        return;
    }

    // Rule 3: ID Badge cannot be empty, max 20 chars
    if (idBadge.isEmpty() || idBadge.length() > 20) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID Badge ne peut pas être vide et doit faire max 20 caractères.");
        return;
    }

    // Rule 4: Salaire must be greater than 0
    if (salaire <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le salaire doit être supérieur à 0.");
        return;
    }

    // ==========================================================

    // 2. Prepare the SQL query
    QSqlQuery query(QSqlDatabase::database(CONNECTION_NAME));
    query.prepare("UPDATE EMPLOYE SET NOM = :nom, PRENOM = :prenom, POSTE = :poste, "
                  "SALAIRE = :salaire, DATE_EMBAUCHE = :date_embauche, ID_BADGE = :id_badge "
                  "WHERE CIN = :cin");

    // 3. Bind the values
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":poste", poste);
    query.bindValue(":salaire", salaire);
    query.bindValue(":date_embauche", dateEmbauche);
    query.bindValue(":id_badge", idBadge);
    query.bindValue(":cin", cin); // CIN is used in the WHERE clause

    // 4. Execute the query
    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Employé modifié avec succès.");
        employeModel->select(); // Refresh the table
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification: " + query.lastError().text());
    }
}

void MainWindow::on_btn_supprimer_clicked()
{
    // 1. Get the CIN from the form
    QString cin = ui->le_cin->text();

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    // 2. Prepare the SQL query
    QSqlQuery query(QSqlDatabase::database(CONNECTION_NAME));
    query.prepare("DELETE FROM EMPLOYE WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    // 3. Execute the query
    if (query.exec()) {
        QMessageBox::information(this, "Succès", "Employé supprimé avec succès.");
        
        // Clear the form
        ui->le_cin->clear();
        ui->le_nom->clear();
        ui->le_prenom->clear();
        ui->cb_poste->setCurrentIndex(0);
        ui->dsb_salaire->setValue(0.0);
        ui->de_embauche->setDate(QDate::currentDate());
        ui->le_badge->clear();

        employeModel->select(); // Refresh the table
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression: " + query.lastError().text());
    }
}

// ==================== RECHERCHE & SORT (SEARCH & SORT) ====================

// 1. Dynamic Search across CIN, NOM, and ID Badge
void MainWindow::on_le_recherche_textChanged(const QString &arg1)
{
    // If the search bar is empty, clear the filter to show all rows
    if (arg1.isEmpty()) {
        employeModel->setFilter("");
    } else {
        // Construct a SQL WHERE clause dynamically
        // TO_CHAR(CIN) -> Converts number to string for text search
        // UPPER(...) -> Makes search case-insensitive
        QString filter = QString("(TO_CHAR(CIN) LIKE '%%1%' OR UPPER(NOM) LIKE UPPER('%%1%') OR UPPER(ID_BADGE) LIKE UPPER('%%1%'))")
                         .arg(arg1);
        
        employeModel->setFilter(filter);
    }
    
    // Refresh the table view with the new filter
    employeModel->select();
}

// 2. Sort by Selected Column (CIN, Nom, ID Badge)
void MainWindow::on_btn_tri_clicked()
{
    // Get the current selection from the ComboBox
    QString currentSort = ui->comboBox->currentText();

    // Map the selection to the correct column index and sort order
    // 0: CIN | 1: NOM | 6: ID_BADGE
    
    if (currentSort == "CIN") {
        employeModel->setSort(0, Qt::AscendingOrder); // Sort by CIN (Column 0)
    } 
    else if (currentSort == "Nom") {
        employeModel->setSort(1, Qt::AscendingOrder); // Sort by Nom (Column 1)
    } 
    else if (currentSort == "ID Badge") {
        employeModel->setSort(6, Qt::AscendingOrder); // Sort by ID Badge (Column 6)
    }

    // Refresh the table view to apply sorting
    employeModel->select();
}
