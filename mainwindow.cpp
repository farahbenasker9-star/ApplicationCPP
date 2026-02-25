#include "mainwindow.h"
#include "ui_mainwindow.h"

// ─── Constructeur ────────────────────────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Set landing page to Dashboard (Index 0)
    ui->stackedWidget->setCurrentIndex(0);
    ui->navbar_container->hide();

    // ==================== DASHBOARD CONNECTIONS ====================
    connect(ui->btn_dash_employe,    &QPushButton::clicked, this, [this]() { navigateToPage(6); ui->btn_nav_employes->setChecked(true); });
    connect(ui->btn_dash_client,     &QPushButton::clicked, this, [this]() { navigateToPage(5); ui->btn_nav_client->setChecked(true); });
    connect(ui->btn_dash_contrat,    &QPushButton::clicked, this, [this]() { navigateToPage(2); ui->btn_nav_contrat->setChecked(true); });
    connect(ui->btn_dash_poubelle,   &QPushButton::clicked, this, [this]() { navigateToPage(7); ui->btn_nav_poubelle->setChecked(true); });
    connect(ui->btn_dash_equipement, &QPushButton::clicked, this, [this]() { navigateToPage(3); ui->btn_nav_equipements->setChecked(true); });
    connect(ui->btn_dash_stock,      &QPushButton::clicked, this, [this]() { navigateToPage(4); ui->btn_nav_stock->setChecked(true); });

    // ==================== SIDEBAR VISIBILITY ====================
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [this](int index) {
        ui->navbar_container->setVisible(index != 0);
    });

    // ==================== NAVIGATION (SIDEBAR) ====================
    connect(ui->btn_nav_employes,    &QPushButton::clicked, this, [this]() { navigateToPage(6); ui->btn_nav_employes->setChecked(true); });
    connect(ui->btn_nav_client,      &QPushButton::clicked, this, [this]() { navigateToPage(5); ui->btn_nav_client->setChecked(true); });
    connect(ui->btn_nav_poubelle,    &QPushButton::clicked, this, [this]() { navigateToPage(7); ui->btn_nav_poubelle->setChecked(true); });
    connect(ui->btn_nav_contrat,     &QPushButton::clicked, this, [this]() { navigateToPage(2); ui->btn_nav_contrat->setChecked(true); });
    connect(ui->btn_nav_stock,       &QPushButton::clicked, this, [this]() { navigateToPage(4); ui->btn_nav_stock->setChecked(true); });
    connect(ui->btn_nav_equipements, &QPushButton::clicked, this, [this]() { navigateToPage(3); ui->btn_nav_equipements->setChecked(true); });

    // ==================== QUIT ====================
    connect(ui->btn_quit, &QPushButton::clicked, this, &QMainWindow::close);

    // ==================== CONTRAT : INITIALISATION ET ACTIONS ====================
    ui->tab_contrat_2->setModel(Contrat().afficher()); // Affichage initial
    populateComboBoxes(); // New: Load Clients from DB

    // ==================== CONTRAT : SELECTION & CRUD ====================
    connect(ui->tab_contrat_2, &QTableView::clicked,
            this, &MainWindow::on_tab_contrat_2_clicked);
    connect(ui->tab_contrat_2, &QTableView::doubleClicked,
            this, &MainWindow::on_tab_contrat_2_doubleClicked);
    
    // Slots for ajouter, modifier, supprimer are now auto-connected by name

    // ==================== EDITABILITÉ ET LIMITES ====================
    ui->ID_Contrat->setReadOnly(false);
    ui->ID_Contrat->setEnabled(true);
    ui->obj_ach_ann->setMaximum(9999999.99); 
    ui->tau_rem_acc->setMaximum(9999999.99); 

    // ==================== VALIDATION CONNECTIONS ====================
    connect(ui->ID_Contrat, &QLineEdit::textChanged, this, &MainWindow::validateID);
    
    connect(ui->date_debut, &QDateEdit::dateChanged, this, &MainWindow::validateDates);
    connect(ui->date_fin, &QDateEdit::dateChanged, this, &MainWindow::validateDates);
    
    connect(ui->obj_ach_ann, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::validateFloats);
    connect(ui->tau_rem_acc, QOverload<double>::of(&QDoubleSpinBox::valueChanged), this, &MainWindow::validateFloats);
    
    connect(ui->clause_penale, &QTextEdit::textChanged, this, &MainWindow::validateDescription);

    connect(ui->combo_ID_Client, &QComboBox::currentIndexChanged, this, &MainWindow::validateID);

    // Set default dates to today
    ui->date_debut->setDate(QDate::currentDate());
    ui->date_fin->setDate(QDate::currentDate());

    // Validation will be triggered when the user starts interacting with the widgets
    /*
    validateID();
    validateDates();
    validateFloats();
    validateDescription();
    */
}

// ─── Destructeur ─────────────────────────────────────────────────────────────
MainWindow::~MainWindow()
{
    delete ui;
}

// ─── Slot navigation employés (garde pour compatibilité) ─────────────────────
void MainWindow::on_btn_nav_employes_clicked()
{
    // Slot de compatibilité — la navigation est gérée dans le constructeur
}

// ─── Navigation par index ────────────────────────────────────────────────────
void MainWindow::navigateToPage(int pageIndex)
{
    ui->stackedWidget->setCurrentIndex(pageIndex);
}

void MainWindow::on_ajouter_contrat_clicked()
{
    // 1. Récupérer les valeurs de tous les champs
    int    id_client         = ui->combo_ID_Client->currentText().toInt();
    int    cin               = 1; // Placeholder simplifié pour le CIN
    QString type_ex          = ui->type_ex->currentText();
    QString prod_con         = ui->prod_con->currentText();
    QDate   date_debut       = ui->date_debut->date();
    QDate   date_fin         = ui->date_fin->date();
    double  obj_ach_ann      = ui->obj_ach_ann->value();
    double  tau_rem_acc      = ui->tau_rem_acc->value();
    QString statut_contrat   = ui->status_contrat->currentText();
    QString clause_penale    = ui->clause_penale->toPlainText().trimmed();

    // 2. Validation basique
    if (id_client == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner ou saisir un Client ID.");
        return;
    }

    // 3. Créer l'objet Contrat
    Contrat c(id_client, cin, type_ex, prod_con, date_debut, date_fin,
              obj_ach_ann, tau_rem_acc, statut_contrat, clause_penale);

    // 4. Ajouter à la base
    if (c.ajouter()) {
        QMessageBox::information(this, "Succès", "Le contrat a été ajouté avec succès !");
        
        // Nettoyage et rafraîchissement
        ui->ID_Contrat->clear();
        ui->combo_ID_Client->setCurrentIndex(0);
        ui->tab_contrat_2->setModel(Contrat().afficher());
        resetValidationStyles();
    } else {
        // Affichage de l'erreur détaillée pour aider au diagnostic
        QSqlError err = QSqlDatabase::database().lastError();
        QMessageBox::critical(this, "Erreur lors de l'ajout", 
            "L'opération a échoué.\n\nDétails techniques :\n" + err.text());
    }
}

// ─── Slot : Sélection simple (Afficher ID) ──────────────────────────────────
void MainWindow::on_tab_contrat_2_clicked(const QModelIndex &index)
{
    // On récupère l'ID du contrat (colonne 0) de la ligne cliquée
    currentSelectedId = ui->tab_contrat_2->model()->data(ui->tab_contrat_2->model()->index(index.row(), 0)).toInt();
    ui->ID_Contrat->setText(QString::number(currentSelectedId));
}

// ─── Slot : Double clic (Remplir tout le formulaire) ────────────────────────
void MainWindow::on_tab_contrat_2_doubleClicked(const QModelIndex &index)
{
    int row = index.row();
    auto model = ui->tab_contrat_2->model();

    currentSelectedId = model->data(model->index(row, 0)).toInt();
    ui->ID_Contrat->setText(QString::number(currentSelectedId));

    // Remplissage du dropdown Client
    QString id_client = model->data(model->index(row, 1)).toString();
    int clientIdx = ui->combo_ID_Client->findText(id_client);
    if (clientIdx != -1) ui->combo_ID_Client->setCurrentIndex(clientIdx);

    // CIN (2) est ignoré dans le formulaire mais présent dans le modèle
    
    ui->type_ex->setCurrentText(model->data(model->index(row, 3)).toString());
    ui->prod_con->setCurrentText(model->data(model->index(row, 4)).toString());
    ui->date_debut->setDate(model->data(model->index(row, 5)).toDate());
    ui->date_fin->setDate(model->data(model->index(row, 6)).toDate());
    
    // Nettoyage des virgules éventuelles pour les nombres
    QString objStr = model->data(model->index(row, 7)).toString().replace(",", ".");
    QString tauStr = model->data(model->index(row, 8)).toString().replace(",", ".");
    ui->obj_ach_ann->setValue(objStr.toDouble());
    ui->tau_rem_acc->setValue(tauStr.toDouble());

    ui->status_contrat->setCurrentText(model->data(model->index(row, 9)).toString());
    ui->clause_penale->setPlainText(model->data(model->index(row, 10)).toString());

    validateID();
    validateDates();
    validateFloats();
    validateDescription();
}

// ─── Slot : Supprimer un contrat ───────────────────────────────────────────
void MainWindow::on_supprimer_contrat_clicked()
{
    bool ok;
    int id = ui->ID_Contrat->text().toInt(&ok);
    
    if (!ok) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner un contrat dans le tableau.");
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer ce contrat ?",
                                  QMessageBox::Yes|QMessageBox::No);
    
    if (reply == QMessageBox::Yes) {
        if (Contrat().supprimer(id)) {
            QMessageBox::information(this, "Succès", "Contrat supprimé avec succès.");
            ui->tab_contrat_2->setModel(Contrat().afficher()); // Rafraîchir
            ui->ID_Contrat->clear();
            currentSelectedId = -1;
        } else {
            QMessageBox::critical(this, "Erreur", "La suppression a échoué.");
        }
    }
}

// ─── Slot : Modifier un contrat ────────────────────────────────────────────
void MainWindow::on_modifier_contrat_clicked()
{
    if (currentSelectedId == -1) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner (double-clic) un contrat pour le modifier.");
        return;
    }

    int    id_client         = ui->combo_ID_Client->currentText().toInt();
    int    cin               = 1; // Placeholder simplifié
    QString type_ex          = ui->type_ex->currentText();
    QString prod_con         = ui->prod_con->currentText();
    QDate   date_debut       = ui->date_debut->date();
    QDate   date_fin         = ui->date_fin->date();
    double  obj_ach_ann      = ui->obj_ach_ann->value();
    double  tau_rem_acc      = ui->tau_rem_acc->value();
    QString statut_contrat   = ui->status_contrat->currentText();
    QString clause_penale    = ui->clause_penale->toPlainText().trimmed();

    // On récupère le NOUVEL ID tapé dans le formulaire
    bool ok;
    int new_id = ui->ID_Contrat->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "ID invalide", "Veuillez saisir un ID numérique valide.");
        return;
    }

    if (id_client == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un Client.");
        return;
    }

    Contrat c(id_client, cin, type_ex, prod_con, date_debut, date_fin,
              obj_ach_ann, tau_rem_acc, statut_contrat, clause_penale);
    c.setId_contrat(new_id); // On met le NOUVEL ID dans l'objet

    if (c.modifier(currentSelectedId)) {
        QMessageBox::information(this, "Succès", "Le contrat a été mis à jour !");
        currentSelectedId = new_id; // Très important : mettre à jour le tracker
        ui->tab_contrat_2->setModel(Contrat().afficher());
    } else {
        QSqlError err = QSqlDatabase::database().lastError();
        QMessageBox::critical(this, "Erreur lors de la modification", 
            "L'opération a échoué.\n\nDétails techniques :\n" + err.text());
    }
}

// ==================== VALIDATION LOGIC ====================

void MainWindow::setWidgetStyle(QWidget* widget, bool isValid) {
    QString style = "background-color: white; color: black; border-radius: 5px; padding: 5px; font-weight: bold;";
    if (isValid) {
        style += "border: 2px solid green;";
    } else {
        style += "border: 2px solid red;";
    }
    widget->setStyleSheet(style);
}

void MainWindow::resetValidationStyles() {
    ui->ID_Contrat->setStyleSheet("");
    ui->combo_ID_Client->setStyleSheet("");
    ui->date_debut->setStyleSheet("");
    ui->date_fin->setStyleSheet("");
    ui->obj_ach_ann->setStyleSheet("");
    ui->tau_rem_acc->setStyleSheet("");
    ui->clause_penale->setStyleSheet("");
}

void MainWindow::validateID() {
    QString text = ui->ID_Contrat->text().trimmed();
    if (text.isEmpty()) {
        ui->ID_Contrat->setStyleSheet("");
    } else {
        bool idOk;
        int id = text.toInt(&idOk);
        setWidgetStyle(ui->ID_Contrat, idOk && id > 0);
    }
    
    if (ui->combo_ID_Client->currentIndex() <= 0) { // "Sélectionner..." ou vide
        ui->combo_ID_Client->setStyleSheet("");
    } else {
        setWidgetStyle(ui->combo_ID_Client, true); // Si sélectionné, c'est forcément valide
    }
}

void MainWindow::validateDates() {
    QDate today = QDate::currentDate();
    QDate debut = ui->date_debut->date();
    QDate fin = ui->date_fin->date();

    setWidgetStyle(ui->date_debut, debut >= today);
    setWidgetStyle(ui->date_fin, fin >= today && fin >= debut);
}

void MainWindow::validateFloats() {
    if (ui->obj_ach_ann->value() == 0.0) {
        ui->obj_ach_ann->setStyleSheet("");
    } else {
        setWidgetStyle(ui->obj_ach_ann, ui->obj_ach_ann->value() > 0);
    }

    if (ui->tau_rem_acc->value() == 0.0) {
        ui->tau_rem_acc->setStyleSheet("");
    } else {
        setWidgetStyle(ui->tau_rem_acc, ui->tau_rem_acc->value() >= 0 && ui->tau_rem_acc->value() <= 100);
    }
}

void MainWindow::validateDescription() {
    QString text = ui->clause_penale->toPlainText().trimmed();
    if (text.isEmpty()) {
        ui->clause_penale->setStyleSheet("");
    } else {
        setWidgetStyle(ui->clause_penale, text.length() >= 5);
    }
}

void MainWindow::populateComboBoxes() {
    ui->combo_ID_Client->clear();
    ui->combo_ID_Client->setEditable(true); // Permet de taper un ID manuellement pour le moment
    ui->combo_ID_Client->addItem("Sélectionner...", 0);
    // addItem("1") supprimé car l'utilisateur l'a ajouté en base

    QSqlQuery query("SELECT ID_CLIENT FROM CLIENT ORDER BY ID_CLIENT ASC");
    while (query.next()) {
        ui->combo_ID_Client->addItem(query.value(0).toString());
    }
}

