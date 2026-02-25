#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>

// ─── Constructeur ────────────────────────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(this);

    // Initialisation de l'affichage des poubelles
    ui->tab_poubelle->setModel(tmp_poubelle.afficher());
    
    // Initialisation du formulaire (bloquer les champs si "En stock" au début)
    on_cb_etat_poubelle_currentIndexChanged(ui->cb_etat_poubelle->currentIndex());

    // 1. Initialisation de l'affichage
    ui->stackedWidget->setCurrentIndex(0); // Dashboard par défaut
    ui->navbar_container->hide();

    // 2. INITIALISATION DES MANAGERS (Très important pour le CRUD)
    employe = new Employe(ui, this);
    client  = new Client(ui, this); // Initialise ta gestion client

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
}

// ─── Destructeur ─────────────────────────────────────────────────────────────
MainWindow::~MainWindow()
{
    delete ui;
}

<<<<<<< HEAD
void MainWindow::navigateToPage(int pageIndex)
{
    // Vérifie si l'index existe dans ton StackedWidget
    if(pageIndex < ui->stackedWidget->count()) {
        ui->stackedWidget->setCurrentIndex(pageIndex);
    } else {
        qDebug() << "Erreur : L'index de page" << pageIndex << "n'existe pas !";
    }
}

void MainWindow::on_btn_nav_employes_clicked() { ui->stackedWidget->setCurrentIndex(6); }
void MainWindow::on_btn_nav_client_clicked() { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_btn_nav_contrat_clicked() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::on_btn_nav_poubelle_clicked() { ui->stackedWidget->setCurrentIndex(7); }
void MainWindow::on_btn_nav_equipements_clicked() { ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_btn_nav_stock_clicked() { ui->stackedWidget->setCurrentIndex(4); }

void MainWindow::on_btn_dash_poubelle_clicked() { ui->stackedWidget->setCurrentIndex(7); }
void MainWindow::on_btn_dash_employe_clicked() { ui->stackedWidget->setCurrentIndex(6); }
void MainWindow::on_btn_dash_client_clicked() { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_btn_dash_contrat_clicked() { ui->stackedWidget->setCurrentIndex(2); }
void MainWindow::on_btn_dash_equipement_clicked() { ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_btn_dash_stock_clicked() { ui->stackedWidget->setCurrentIndex(4); }

// ==================== CRUD POUBELLE ====================

void MainWindow::on_btn_ajouter_poubelle_clicked()
{
    if (!validerFormulairePoubelle(false)) return;

    int id = ui->le_id_poubelle->text().toInt();
    QString adresse = ui->le_adresse_poubelle->text();
    QString type = ui->le_type_dechet->text();
    int capacite = ui->sb_capacite->value();
    QString etat = ui->cb_etat_poubelle->currentText();
    
    int remplissage = ui->sb_remplissage->value();
    int batterie = ui->sb_batterie->value();
    QString statut_capteur = ui->cb_statut_capteur->currentText();
    QDate date_inst = ui->de_installation->date();
    QDate date_coll = ui->de_collecte->date();

    Poubelle p(id, type, adresse, capacite, etat, remplissage, batterie, statut_capteur, date_inst, date_coll);

    if(p.ajouter()) {
        QMessageBox::information(this, "Succ�s", "Poubelle ajout�e avec succ�s");
        ui->tab_poubelle->setModel(tmp_poubelle.afficher());
        clearFormPoubelle();
    } else {
        QMessageBox::critical(this, "Erreur", "�chec de l'ajout.\nD�tail : " + p.getLastError());
    }
}

void MainWindow::on_btn_supprimer_poubelle_clicked()
{
    int row = ui->tab_poubelle->currentIndex().row();
    if (row == -1) {
        QMessageBox::warning(this, "Selection", "Veuillez s�lectionner une poubelle.");
        return;
    }
    
    int id = ui->tab_poubelle->model()->data(ui->tab_poubelle->model()->index(row, 0)).toInt();

    if(tmp_poubelle.supprimer(id)) {
        QMessageBox::information(this, "Succ�s", "Poubelle supprim�e");
        ui->tab_poubelle->setModel(tmp_poubelle.afficher());
        clearFormPoubelle();
    } else {
        QMessageBox::critical(this, "Erreur", "�chec de la suppression");
    }
}

void MainWindow::on_btn_modifier_poubelle_clicked()
{
    if (!validerFormulairePoubelle(true)) return;

    int id = ui->le_id_poubelle->text().toInt();
    QString adresse = ui->le_adresse_poubelle->text();
    QString type = ui->le_type_dechet->text();
    int capacite = ui->sb_capacite->value();
    QString etat = ui->cb_etat_poubelle->currentText();
    
    int remplissage = ui->sb_remplissage->value();
    int batterie = ui->sb_batterie->value();
    QString statut_capteur = ui->cb_statut_capteur->currentText();
    QDate date_inst = ui->de_installation->date();
    QDate date_coll = ui->de_collecte->date();

    Poubelle p;
    p.setId(id); p.setAdresse(adresse); p.setType(type); p.setCapacite(capacite); p.setEtat(etat);
    p.setNiveauRemplissage(remplissage); p.setEtatBatterie(batterie); p.setStatutCapteur(statut_capteur);
    p.setDateInstallation(date_inst); p.setDateCollecte(date_coll);

    if(p.modifier()) {
        QMessageBox::information(this, "Succ�s", "Poubelle modifi�e");
        ui->tab_poubelle->setModel(tmp_poubelle.afficher());
        clearFormPoubelle();
    } else {
        QMessageBox::critical(this, "Erreur", "�chec de la modification");
    }
}

void MainWindow::on_tab_poubelle_clicked(const QModelIndex &index)
{
    int row = index.row();
    QAbstractItemModel *model = ui->tab_poubelle->model();
    ui->le_id_poubelle->setText(model->data(model->index(row, 0)).toString());
    ui->le_id_poubelle->setEnabled(false);
    ui->le_type_dechet->setText(model->data(model->index(row, 1)).toString());
    ui->le_adresse_poubelle->setText(model->data(model->index(row, 2)).toString());
    ui->sb_capacite->setValue(model->data(model->index(row, 3)).toInt());
    ui->cb_etat_poubelle->setCurrentText(model->data(model->index(row, 4)).toString());
    ui->sb_remplissage->setValue(model->data(model->index(row, 5)).toInt());
    ui->sb_batterie->setValue(model->data(model->index(row, 6)).toInt());
    ui->cb_statut_capteur->setCurrentText(model->data(model->index(row, 7)).toString());
    ui->de_installation->setDate(model->data(model->index(row, 8)).toDate());
    ui->de_collecte->setDate(model->data(model->index(row, 9)).toDate());
}

void MainWindow::on_cb_etat_poubelle_currentIndexChanged(int index)
{
    bool estInstalle = (index == 1);
    ui->le_id_poubelle->setEnabled(true);
    ui->le_adresse_poubelle->setEnabled(true);
    ui->le_type_dechet->setEnabled(true);
    ui->sb_capacite->setEnabled(true);
    ui->sb_remplissage->setEnabled(estInstalle);
    ui->sb_batterie->setEnabled(estInstalle);
    ui->de_installation->setEnabled(estInstalle);
    ui->de_collecte->setEnabled(estInstalle);
    ui->cb_statut_capteur->setEnabled(estInstalle);
}

void MainWindow::clearFormPoubelle()
{
    ui->le_id_poubelle->clear();
    ui->le_id_poubelle->setEnabled(true);
    ui->le_adresse_poubelle->clear();
    ui->le_type_dechet->clear();
    ui->sb_capacite->setValue(0);
    ui->cb_etat_poubelle->setCurrentIndex(0);
    ui->sb_remplissage->setValue(0);
    ui->sb_batterie->setValue(0);
    ui->cb_statut_capteur->setCurrentIndex(-1);
    ui->de_installation->setDate(QDate::currentDate());
    ui->de_collecte->setDate(QDate::currentDate());
}

bool MainWindow::validerFormulairePoubelle(bool isUpdate)
{
    QString idStr = ui->le_id_poubelle->text();
    QString adresse = ui->le_adresse_poubelle->text();
    QString type = ui->le_type_dechet->text();
    QString etat = ui->cb_etat_poubelle->currentText();
    QRegularExpression idRegex("^\\d{5}$");
    QRegularExpression addrRegex("^[a-zA-Z\\s,]+$");
    QRegularExpression typeRegex("^[a-zA-Z\\s]+$");
    if (!idRegex.match(idStr).hasMatch()) {
        QMessageBox::warning(this, "Erreur", "L'ID doit faire 5 chiffres.");
        return false;
    }
    if (!isUpdate && tmp_poubelle.existe(idStr.toInt())) {
        QMessageBox::warning(this, "Erreur", "ID existant.");
        return false;
    }
    if (!addrRegex.match(adresse).hasMatch() || adresse.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Adresse invalide.");
        return false;
    }
    if (!typeRegex.match(type).hasMatch() || type.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Type invalide.");
        return false;
    }
    if (etat == "INSTALLE") {
        if (ui->sb_capacite->value() <= 0) return false;
        if (ui->de_installation->date() > QDate::currentDate()) return false;
    }
    return true;
}

void MainWindow::on_btn_tri_poubelle_clicked()
{
    QString critere = ui->cb_tri_poubelle->currentText();
    QString tri;
    if (critere == "Niveau de remplissage") tri = "NIVEAU_REMPLISSAGE";
    else if (critere == "Etat batterie") tri = "ETAT_BATTERIE";
    else if (critere == "Adresse") tri = "ADRESSE";
    
    ui->tab_poubelle->setModel(tmp_poubelle.trier(tri));
}

void MainWindow::on_le_recherche_poubelle_textChanged(const QString &arg1)
{
    ui->tab_poubelle->setModel(tmp_poubelle.rechercher(arg1));
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
}

