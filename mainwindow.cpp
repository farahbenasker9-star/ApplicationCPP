#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QDateTime>
#include <QTableView>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFont>
#include <QColor>
#include <QMargins>
#include <QSqlDatabase>
#include <QPushButton>
#include <QSqlQueryModel>

#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

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

    // 2. INITIALISATION DES MANAGERS
    employe    = new Employe(ui, this);
    client     = new Client(ui, this);
    equipement = new Equipement(ui, this);

    // ==================== DASHBOARD CONNECTIONS ====================
    connect(ui->btn_dash_employe,   &QPushButton::clicked, this, [this]() { navigateToPage(6); ui->btn_nav_employes->setChecked(true); });
    connect(ui->btn_dash_client,    &QPushButton::clicked, this, [this]() { navigateToPage(5); ui->btn_nav_client->setChecked(true); });
    connect(ui->btn_dash_contrat,   &QPushButton::clicked, this, [this]() { navigateToPage(2); ui->btn_nav_contrat->setChecked(true); });
    connect(ui->btn_dash_poubelle,  &QPushButton::clicked, this, [this]() { navigateToPage(7); ui->btn_nav_poubelle->setChecked(true); });
    connect(ui->btn_dash_equipement,&QPushButton::clicked, this, [this]() { navigateToPage(3); ui->btn_nav_equipements->setChecked(true); });
    connect(ui->btn_dash_stock,     &QPushButton::clicked, this, [this]() { navigateToPage(4); ui->btn_nav_stock->setChecked(true); });

    // ==================== SIDEBAR VISIBILITY ====================
    connect(ui->stackedWidget, &QStackedWidget::currentChanged, this, [this](int index) {
        if (index == 0) {
            ui->navbar_container->hide();
        } else {
            ui->navbar_container->show();
        }
    });

    // ==================== NAVIGATION (SIDEBAR) ====================
    connect(ui->btn_nav_employes,   &QPushButton::clicked, this, [this]() { navigateToPage(6); ui->btn_nav_employes->setChecked(true); });
    connect(ui->btn_nav_client,     &QPushButton::clicked, this, [this]() { navigateToPage(5); ui->btn_nav_client->setChecked(true); });
    connect(ui->btn_nav_poubelle,   &QPushButton::clicked, this, [this]() { navigateToPage(7); ui->btn_nav_poubelle->setChecked(true); });
    connect(ui->btn_nav_contrat,    &QPushButton::clicked, this, [this]() { navigateToPage(2); ui->btn_nav_contrat->setChecked(true); });
    connect(ui->btn_nav_stock,      &QPushButton::clicked, this, [this]() { navigateToPage(4); ui->btn_nav_stock->setChecked(true); });
    connect(ui->btn_nav_equipements,&QPushButton::clicked, this, [this]() { navigateToPage(3); ui->btn_nav_equipements->setChecked(true); });

    // Connect Quit button
    connect(ui->btn_quit, &QPushButton::clicked, this, &QMainWindow::close);

    // ==================== MODULE CONTRAT ====================
    // Affichage initial du tableau
    ui->tab_contrat_2->setModel(Contrat().afficher());
    contrat_populateComboBoxes();

    // Connexions CRUD
    connect(ui->tab_contrat_2, &QTableView::clicked,       this, &MainWindow::contrat_onTabClicked);
    connect(ui->tab_contrat_2, &QTableView::doubleClicked, this, &MainWindow::contrat_onTabDoubleClicked);
    connect(ui->ajouter_contrat,   &QPushButton::clicked, this, &MainWindow::contrat_onAjouterClicked);
    connect(ui->supprimer_contrat, &QPushButton::clicked, this, &MainWindow::contrat_onSupprimerClicked);
    connect(ui->modifier_contrat,  &QPushButton::clicked, this, &MainWindow::contrat_onModifierClicked);
    connect(ui->exporter_contrat,  &QPushButton::clicked, this, &MainWindow::contrat_onExporterClicked);
    connect(ui->tri2_contrat,      &QPushButton::clicked, this, &MainWindow::contrat_onTriClicked);

    // Editabilité & limites
    ui->ID_Contrat->setReadOnly(false);
    ui->ID_Contrat->setEnabled(true);
    ui->obj_ach_ann->setMaximum(9999999.99);
    ui->tau_rem_acc->setMaximum(9999999.99);

    // Connexions validation en temps réel
    connect(ui->ID_Contrat,    &QLineEdit::textChanged,
            this, &MainWindow::contrat_validateID);
    connect(ui->date_debut,    &QDateEdit::dateChanged,
            this, &MainWindow::contrat_validateDates);
    connect(ui->date_fin,      &QDateEdit::dateChanged,
            this, &MainWindow::contrat_validateDates);
    connect(ui->obj_ach_ann,   QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::contrat_validateFloats);
    connect(ui->tau_rem_acc,   QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &MainWindow::contrat_validateFloats);
    connect(ui->clause_penale, &QTextEdit::textChanged,
            this, &MainWindow::contrat_validateDescription);
    connect(ui->combo_ID_Client, &QComboBox::currentIndexChanged,
            this, &MainWindow::contrat_validateID);

    // Dates par défaut
    ui->date_debut->setDate(QDate::currentDate());
    ui->date_fin->setDate(QDate::currentDate());

    // Barre de recherche
    ui->recherche0_contrat->setReadOnly(false);
    ui->recherche0_contrat->setEnabled(true);
    ui->recherche0_contrat->setFocusPolicy(Qt::StrongFocus);
    ui->recherche0_contrat->setPlaceholderText("🔍 Rechercher par ID contrat, Type, Statut...");
    ui->recherche0_contrat->setClearButtonEnabled(true);
    ui->recherche0_contrat->setStyleSheet(
        "QLineEdit { background: #e8eceb; color: #3a3a3a; border: 1px solid #bfbfbf; border-radius: 8px; padding: 8px; } "
        "QLineEdit:focus { background: #e8eceb; color: #222; border: 1px solid #6aa84f; }");

    connect(ui->recherche0_contrat, &QLineEdit::textChanged,
            this, &MainWindow::contrat_onRechercheTextChanged);

    ui->recherche0_contrat->installEventFilter(this);

    // Statistiques
    contrat_setupStatsUI();
    contrat_refreshStats();

    // cb_statut (produit)
    on_cb_statut_currentIndexChanged(ui->cb_statut->currentIndex());
}

// ─── Destructeur ─────────────────────────────────────────────────────────────
MainWindow::~MainWindow()
{
    delete ui;
}

// ─── Event Filter (barre de recherche contrat) ───────────────────────────────
bool MainWindow::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->recherche0_contrat) {
        if (event->type() == QEvent::FocusIn) {
            if (ui->recherche0_contrat->text().isEmpty()) {
                ui->recherche0_contrat->clear();
            }
            ui->recherche0_contrat->setStyleSheet(
                "QLineEdit { background: white; color: black; border: 1px solid #4CAF50; border-radius: 8px; padding: 5px; } "
                "QLineEdit:focus { background: white; color: black; }");
        } else if (event->type() == QEvent::FocusOut) {
            if (ui->recherche0_contrat->text().isEmpty()) {
                ui->recherche0_contrat->setStyleSheet(
                    "QLineEdit { background: #f5f5f5; color: #555; border: 1px solid #c8c8c8; border-radius: 8px; padding: 5px; } "
                    "QLineEdit:focus { background: white; color: black; border: 1px solid #4CAF50; }");
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

// ─── Navigation Slots ────────────────────────────────────────────────────────
void MainWindow::on_btn_nav_employes_clicked()   { ui->stackedWidget->setCurrentIndex(6); }
void MainWindow::on_btn_nav_client_clicked()     { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_btn_nav_contrat_clicked()    { navigateToPage(2); }
void MainWindow::on_btn_nav_poubelle_clicked()   { ui->stackedWidget->setCurrentIndex(7); }
void MainWindow::on_btn_nav_equipements_clicked(){ ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_btn_nav_stock_clicked()      { ui->stackedWidget->setCurrentIndex(4); }

void MainWindow::on_btn_dash_poubelle_clicked()  { ui->stackedWidget->setCurrentIndex(7); }
void MainWindow::on_btn_dash_employe_clicked()   { ui->stackedWidget->setCurrentIndex(6); }
void MainWindow::on_btn_dash_client_clicked()    { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_btn_dash_contrat_clicked()   { navigateToPage(2); }
void MainWindow::on_btn_dash_equipement_clicked(){ ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_btn_dash_stock_clicked()     { ui->stackedWidget->setCurrentIndex(4); }

// ─── Navigation par index ────────────────────────────────────────────────────
void MainWindow::navigateToPage(int pageIndex)
{
    ui->stackedWidget->setCurrentIndex(pageIndex);
    if (pageIndex == 2) { // Page Contrat
        contrat_populateComboBoxes();
        ui->tab_contrat_2->setModel(Contrat().afficher());
        contrat_refreshStats();
    }
    if (pageIndex == 4) {
        Produit P;
        ui->tab_produit->setModel(P.afficher());
        chargerIdsClients();
    }
}

// =========================================================
// ===                  MODULE POUBELLE                  ===
// =========================================================

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
        QMessageBox::information(this, "Succès", "Poubelle ajoutée avec succès");
        ui->tab_poubelle->setModel(tmp_poubelle.afficher());
        clearFormPoubelle();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'ajout.\nDétail : " + p.getLastError());
    }
}

void MainWindow::on_btn_supprimer_poubelle_clicked()
{
    int row = ui->tab_poubelle->currentIndex().row();
    if (row == -1) {
        QMessageBox::warning(this, "Selection", "Veuillez sélectionner une poubelle.");
        return;
    }

    int id = ui->tab_poubelle->model()->data(ui->tab_poubelle->model()->index(row, 0)).toInt();

    if(tmp_poubelle.supprimer(id)) {
        QMessageBox::information(this, "Succès", "Poubelle supprimée");
        ui->tab_poubelle->setModel(tmp_poubelle.afficher());
        clearFormPoubelle();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la suppression");
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
        QMessageBox::information(this, "Succès", "Poubelle modifiée");
        ui->tab_poubelle->setModel(tmp_poubelle.afficher());
        clearFormPoubelle();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification");
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

// =========================================================
// ===                  MODULE PRODUIT                   ===
// =========================================================

void MainWindow::chargerIdsClients() {
    ui->cb_id_client->clear();
    ui->cb_id_client->addItem("");

    QSqlQuery query("SELECT ID_CLIENT FROM CLIENT ORDER BY ID_CLIENT ASC");
    while (query.next()) {
        ui->cb_id_client->addItem(query.value(0).toString());
    }
}

void MainWindow::on_btn_ajouter_produit_clicked() {
    if (!controleSaisieProduit()) return;
    int id = ui->le_id->text().toInt();
    int id_c = ui->cb_id_client->currentText().toInt();
    QString type = ui->cb_type_2->currentText();
    float poids = ui->sb_poids->value();
    float prix = ui->sb_prix->value();
    QDate dc = ui->de_date_c->date();
    QDate dv = ui->de_date_v->date();
    QString statut = ui->cb_statut->currentText();

    Produit P(id, id_c, type, poids, dc, dv, statut, prix);

    if(P.ajouter()) {
        ui->tab_produit->setModel(P.afficher());
        QMessageBox::information(this, "Succès", "Produit ajouté !");
        viderFormulaire();
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout échoué.");
    }
}

void MainWindow::on_btn_supprimer_produit_clicked() {
    int id = ui->le_id->text().toInt();
    Produit P;
    if(P.supprimer(id)) {
        ui->tab_produit->setModel(P.afficher());
        QMessageBox::information(this, "Succès", "Produit supprimé !");
        viderFormulaire();
    }
}

void MainWindow::on_btn_modifier_produit_clicked() {
    if (id_a_modifier == -1) {
        QMessageBox::warning(this, "Erreur", "Veuillez d'abord sélectionner un produit dans le tableau.");
        return;
    }

    if (!controleSaisieProduit()) return;

    int id_c = 0;
    if (!ui->cb_id_client->currentText().isEmpty()) {
        id_c = ui->cb_id_client->currentText().toInt();
    }

    Produit P(ui->le_id->text().toInt(),
              id_c,
              ui->cb_type_2->currentText(),
              ui->sb_poids->value(),
              ui->de_date_c->date(),
              ui->de_date_v->date(),
              ui->cb_statut->currentText(),
              ui->sb_prix->value());

    if(P.modifier(id_a_modifier)) {
        ui->tab_produit->setModel(P.afficher());
        QMessageBox::information(this, "Succès", "Produit mis à jour !");
        id_a_modifier = ui->le_id->text().toInt();
        viderFormulaire();
    } else {
        QMessageBox::critical(this, "Erreur", "La modification a échoué. Vérifiez que le nouvel ID n'est pas déjà utilisé par un autre produit.");
    }
}

void MainWindow::on_cb_statut_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    QString statut = ui->cb_statut->currentText();

    if (statut == "Vendu") {
        ui->cb_id_client->setEnabled(true);
        ui->de_date_v->setEnabled(true);
        ui->de_date_v->setDate(QDate::currentDate());
    }
    else if (statut == "Réservé") {
        ui->cb_id_client->setEnabled(true);
        ui->de_date_v->setEnabled(false);
    }
    else {
        ui->cb_id_client->setEnabled(false);
        ui->de_date_v->setEnabled(false);
        ui->cb_id_client->setCurrentIndex(-1);
    }
}

void MainWindow::on_tab_produit_clicked(const QModelIndex &index)
{
    int row = index.row();
    auto model = ui->tab_produit->model();
    this->id_a_modifier = model->index(row, 0).data().toInt();

    QString statut_db = model->index(row, 6).data().toString().trimmed();
    ui->cb_statut->setCurrentText(statut_db);

    on_cb_statut_currentIndexChanged(ui->cb_statut->currentIndex());

    QString id_client_db = model->index(row, 1).data().toString().trimmed();
    int idx_client = ui->cb_id_client->findText(id_client_db);
    if (idx_client != -1) {
        ui->cb_id_client->setCurrentIndex(idx_client);
    } else {
        ui->cb_id_client->setCurrentIndex(0);
    }

    ui->le_id->setText(model->index(row, 0).data().toString());
    ui->cb_type_2->setCurrentText(model->index(row, 2).data().toString().trimmed());
    ui->sb_poids->setValue(model->index(row, 3).data().toDouble());
    ui->de_date_c->setDate(model->index(row, 4).data().toDate());
    ui->de_date_v->setDate(model->index(row, 5).data().toDate());
    ui->sb_prix->setValue(model->index(row, 7).data().toDouble());
}

bool MainWindow::controleSaisieProduit() {
    QString id_p = ui->le_id->text();
    double prix = ui->sb_prix->value();
    double poids = ui->sb_poids->value();
    QString statut = ui->cb_statut->currentText();
    QString id_c = ui->cb_id_client->currentText();

    if (id_p.length() != 8) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID Produit doit contenir exactement 8 chiffres.");
        return false;
    }
    if (poids <= 0 || prix <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le poids et le prix doivent être supérieurs à 0.");
        return false;
    }

    QDate dateCreation = ui->de_date_c->date();
    QDate aujourdhui = QDate::currentDate();
    QDate limitePassee = aujourdhui.addMonths(-1);

    if (dateCreation < limitePassee) {
        QMessageBox::warning(this, "Erreur de date",
                             "La date de création est trop ancienne. Elle doit être comprise entre le "
                                 + limitePassee.toString("dd/MM/yyyy") + " et aujourd'hui.");
        return false;
    }
    if (dateCreation > aujourdhui) {
        QMessageBox::warning(this, "Erreur de date",
                             "La date de création ne peut pas être une date future.");
        return false;
    }
    if (statut != "Disponible" && id_c.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Un ID Client est obligatoire pour un produit Réservé ou Vendu.");
        return false;
    }

    return true;
}

void MainWindow::viderFormulaire() {
    ui->le_id->clear();
    ui->sb_poids->setValue(0.0);
    ui->sb_prix->setValue(0.0);
    ui->de_date_c->setDate(QDate::currentDate());
    ui->de_date_v->setDate(QDate::currentDate());
    ui->cb_type_2->setCurrentIndex(0);
    ui->cb_statut->setCurrentText("Disponible");
    on_cb_statut_currentIndexChanged(ui->cb_statut->currentIndex());
    this->id_a_modifier = -1;
}

// =========================================================
// ===                  MODULE CONTRAT                   ===
// =========================================================

// ─── Initialisation de la zone graphiques ────────────────────────────────────
void MainWindow::contrat_setupStatsUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->gb_stats_contrat);
    mainLayout->setSpacing(6);
    mainLayout->setContentsMargins(6, 22, 6, 6);

    // Histogramme
    chartViewContratType = new QChartView();
    chartViewContratType->setRenderHint(QPainter::Antialiasing);
    chartViewContratType->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartViewContratType->setMinimumHeight(160);
    chartViewContratType->setMaximumHeight(295);
    chartViewContratType->setMinimumWidth(0);

    // Camembert
    chartViewContratTypePie = new QChartView();
    chartViewContratTypePie->setRenderHint(QPainter::Antialiasing);
    chartViewContratTypePie->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartViewContratTypePie->setMinimumHeight(160);
    chartViewContratTypePie->setMaximumHeight(295);
    chartViewContratTypePie->setMinimumWidth(0);

    mainLayout->addWidget(chartViewContratType);
    mainLayout->addWidget(chartViewContratTypePie);

    ui->gb_stats_contrat->setLayout(mainLayout);
}

// ─── Mise à jour des graphiques statistiques ─────────────────────────────────
void MainWindow::contrat_refreshStats()
{
    QSqlQuery q;

    // ── 1. Histogramme : 3 barres côte à côte (Totale / Sur gamme / Partielle) ──
    int cntTotale = 0, cntSurGamme = 0, cntPartielle = 0;
    if (q.exec("SELECT TYPE_EXCLUSIVITE, COUNT(*) FROM CONTRAT GROUP BY TYPE_EXCLUSIVITE")) {
        while (q.next()) {
            QString t = q.value(0).toString().trimmed();
            int n     = q.value(1).toInt();
            if      (t == "Totale")    cntTotale    = n;
            else if (t == "Sur gamme") cntSurGamme  = n;
            else if (t == "Partielle") cntPartielle = n;
        }
    }

    QChart *chartType = new QChart();
    chartType->setTitle("Contrats par Type");
    chartType->setTitleFont(QFont("Arial", 9, QFont::Bold));
    chartType->setAnimationOptions(QChart::SeriesAnimations);
    chartType->setMargins(QMargins(0, 4, 8, 0));
    chartType->setBackgroundRoundness(0);
    chartType->legend()->setVisible(true);
    chartType->legend()->setAlignment(Qt::AlignBottom);
    chartType->legend()->setFont(QFont("Arial", 7));

    QBarSet *sTotale    = new QBarSet("Totale");
    QBarSet *sSurGamme  = new QBarSet("Sur gamme");
    QBarSet *sPartielle = new QBarSet("Partielle");

    sTotale->setColor(QColor("#1b5e20"));
    sTotale->setBorderColor(QColor("#0d3d12"));
    sTotale->setLabelColor(Qt::white);
    *sTotale << cntTotale;

    sSurGamme->setColor(QColor("#e53935"));
    sSurGamme->setBorderColor(QColor("#b71c1c"));
    sSurGamme->setLabelColor(Qt::white);
    *sSurGamme << cntSurGamme;

    sPartielle->setColor(QColor("#1565c0"));
    sPartielle->setBorderColor(QColor("#0d47a1"));
    sPartielle->setLabelColor(Qt::white);
    *sPartielle << cntPartielle;

    QBarSeries *barSeries = new QBarSeries();
    barSeries->setBarWidth(0.5);
    barSeries->append(sTotale);
    barSeries->append(sSurGamme);
    barSeries->append(sPartielle);
    barSeries->setLabelsVisible(true);
    barSeries->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    barSeries->setLabelsFormat("@value");

    chartType->addSeries(barSeries);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(QStringList{"Types d'exclusivite"});
    axisX->setLabelsFont(QFont("Arial", 8));
    axisX->setGridLineVisible(false);
    chartType->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    int maxVal = qMax(qMax(cntTotale, cntSurGamme), cntPartielle);
    QValueAxis *axisY = new QValueAxis();
    axisY->setMin(0);
    axisY->setMax(maxVal < 1 ? 5 : maxVal + 2);
    axisY->setLabelFormat("%i");
    axisY->setLabelsFont(QFont("Arial", 7));
    axisY->setTickCount(qMin(maxVal + 3, 7));
    chartType->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    chartViewContratType->setChart(chartType);

    // ── 2. Camembert : Répartition par statut ────────────────────────────────
    QChart *chartPie = new QChart();
    chartPie->setTitle("Statuts des contrats");
    chartPie->setTitleFont(QFont("Arial", 8, QFont::Bold));
    chartPie->setMargins(QMargins(0, 4, 0, 0));
    chartPie->setBackgroundRoundness(0);
    chartPie->setAnimationOptions(QChart::SeriesAnimations);

    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setPieStartAngle(30);
    pieSeries->setHoleSize(0.3);
    pieSeries->setPieSize(0.62);

    QList<QColor> pieColors = {
        QColor("#1b5e20"), QColor("#42a5f5"),
        QColor("#ef9a9a"), QColor("#ffe082")
    };
    int ci = 0;
    if (q.exec("SELECT STATUT_CONTRAT, COUNT(*) FROM CONTRAT GROUP BY STATUT_CONTRAT")) {
        while (q.next()) {
            QString statut = q.value(0).toString();
            if (statut.isEmpty()) statut = "Indéfini";
            int count = q.value(1).toInt();
            QPieSlice *slice = pieSeries->append(statut + " (" + QString::number(count) + ")", count);
            slice->setLabelVisible(true);
            slice->setLabelFont(QFont("Arial", 7));
            slice->setColor(pieColors[ci % pieColors.size()]);
            ci++;
        }
    }

    chartPie->addSeries(pieSeries);
    chartPie->legend()->setAlignment(Qt::AlignRight);
    chartPie->legend()->setFont(QFont("Arial", 7));
    chartViewContratTypePie->setChart(chartPie);
}

// ─── Remplir les comboboxes Client / Employé ─────────────────────────────────
void MainWindow::contrat_populateComboBoxes()
{
    ui->combo_ID_Client->clear();
    ui->combo_ID_Client->setEditable(true);
    ui->combo_ID_Client->addItem("Sélectionner...", 0);

    QSqlQuery qClient("SELECT ID_CLIENT FROM CLIENT ORDER BY ID_CLIENT ASC");
    while (qClient.next()) {
        ui->combo_ID_Client->addItem(qClient.value(0).toString());
    }

    ui->combo_CIN_Employe->clear();
    ui->combo_CIN_Employe->setEditable(true);
    ui->combo_CIN_Employe->addItem("Sélectionner...", 0);

    QSqlQuery qEmp("SELECT CIN FROM EMPLOYE ORDER BY CIN ASC");
    while (qEmp.next()) {
        ui->combo_CIN_Employe->addItem(qEmp.value(0).toString());
    }
}

// ─── En-têtes du modèle ──────────────────────────────────────────────────────
void MainWindow::contrat_setupModelHeaders(QSqlQueryModel *m)
{
    m->setHeaderData(0,  Qt::Horizontal, QObject::tr("ID"));
    m->setHeaderData(1,  Qt::Horizontal, QObject::tr("ID Client"));
    m->setHeaderData(2,  Qt::Horizontal, QObject::tr("CIN Employé"));
    m->setHeaderData(3,  Qt::Horizontal, QObject::tr("Type Exclusivité"));
    m->setHeaderData(4,  Qt::Horizontal, QObject::tr("Produits"));
    m->setHeaderData(5,  Qt::Horizontal, QObject::tr("Date Début"));
    m->setHeaderData(6,  Qt::Horizontal, QObject::tr("Date Fin"));
    m->setHeaderData(7,  Qt::Horizontal, QObject::tr("Obj. Achat"));
    m->setHeaderData(8,  Qt::Horizontal, QObject::tr("Taux Rem."));
    m->setHeaderData(9,  Qt::Horizontal, QObject::tr("Statut"));
    m->setHeaderData(10, Qt::Horizontal, QObject::tr("Clause Pénale"));
}

// ─── Clic simple : affiche l'ID dans le formulaire ───────────────────────────
void MainWindow::contrat_onTabClicked(const QModelIndex &index)
{
    contrat_currentSelectedId = ui->tab_contrat_2->model()
                        ->data(ui->tab_contrat_2->model()->index(index.row(), 0)).toInt();
    ui->ID_Contrat->setText(QString::number(contrat_currentSelectedId));
}

// ─── Double-clic : remplit tout le formulaire ─────────────────────────────────
void MainWindow::contrat_onTabDoubleClicked(const QModelIndex &index)
{
    int row = index.row();
    auto m = ui->tab_contrat_2->model();

    contrat_currentSelectedId = m->data(m->index(row, 0)).toInt();
    ui->ID_Contrat->setText(QString::number(contrat_currentSelectedId));

    QString id_client = m->data(m->index(row, 1)).toString();
    int clientIdx = ui->combo_ID_Client->findText(id_client);
    if (clientIdx != -1) ui->combo_ID_Client->setCurrentIndex(clientIdx);

    QString cin_val = m->data(m->index(row, 2)).toString();
    int cinIdx = ui->combo_CIN_Employe->findText(cin_val);
    if (cinIdx != -1) ui->combo_CIN_Employe->setCurrentIndex(cinIdx);

    ui->type_ex->setCurrentText(m->data(m->index(row, 3)).toString());
    ui->prod_con->setCurrentText(m->data(m->index(row, 4)).toString());
    ui->date_debut->setDate(m->data(m->index(row, 5)).toDate());
    ui->date_fin->setDate(m->data(m->index(row, 6)).toDate());

    QString objStr = m->data(m->index(row, 7)).toString().replace(",", ".");
    QString tauStr = m->data(m->index(row, 8)).toString().replace(",", ".");
    ui->obj_ach_ann->setValue(objStr.toDouble());
    ui->tau_rem_acc->setValue(tauStr.toDouble());

    ui->status_contrat->setCurrentText(m->data(m->index(row, 9)).toString());
    ui->clause_penale->setPlainText(m->data(m->index(row, 10)).toString());

    contrat_validateID();
    contrat_validateDates();
    contrat_validateFloats();
    contrat_validateDescription();
}

// ─── Ajouter un contrat ───────────────────────────────────────────────────────
void MainWindow::contrat_onAjouterClicked()
{
    int     id_client       = ui->combo_ID_Client->currentText().toInt();
    int     cin             = ui->combo_CIN_Employe->currentText().toInt();
    QString type_ex         = ui->type_ex->currentText();
    QString prod_con        = ui->prod_con->currentText();
    QDate   date_debut      = ui->date_debut->date();
    QDate   date_fin        = ui->date_fin->date();
    double  obj_ach_ann     = ui->obj_ach_ann->value();
    double  tau_rem_acc     = ui->tau_rem_acc->value();
    QString statut_contrat  = ui->status_contrat->currentText();
    QString clause_penale   = ui->clause_penale->toPlainText().trimmed();

    if (!contrat_checkDates()) {
        QMessageBox::warning(this, "Erreur",
            "La date de début doit être au plus aujourd'hui et la date de fin doit être après ou égale à la date de début.");
        return;
    }
    if (id_client == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner ou saisir un Client ID.");
        return;
    }
    if (cin == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un CIN Employé.");
        return;
    }

    Contrat c(id_client, cin, type_ex, prod_con, date_debut, date_fin,
              obj_ach_ann, tau_rem_acc, statut_contrat, clause_penale);

    if (c.ajouter()) {
        QMessageBox::information(this, "Succès", "Le contrat a été ajouté avec succès !");
        ui->ID_Contrat->clear();
        ui->combo_ID_Client->setCurrentIndex(0);
        ui->combo_CIN_Employe->setCurrentIndex(0);
        ui->tab_contrat_2->setModel(Contrat().afficher());
        contrat_resetValidationStyles();
        contrat_refreshStats();
    } else {
        QSqlError err = QSqlDatabase::database().lastError();
        QMessageBox::critical(this, "Erreur lors de l'ajout",
                              "L'opération a échoué.\n\nDétails techniques :\n" + err.text());
    }
}

// ─── Modifier un contrat ──────────────────────────────────────────────────────
void MainWindow::contrat_onModifierClicked()
{
    if (contrat_currentSelectedId == -1) {
        QMessageBox::warning(this, "Sélection requise",
            "Veuillez sélectionner (double-clic) un contrat pour le modifier.");
        return;
    }

    int     id_client       = ui->combo_ID_Client->currentText().toInt();
    int     cin             = ui->combo_CIN_Employe->currentText().toInt();
    QString type_ex         = ui->type_ex->currentText();
    QString prod_con        = ui->prod_con->currentText();
    QDate   date_debut      = ui->date_debut->date();
    QDate   date_fin        = ui->date_fin->date();
    double  obj_ach_ann     = ui->obj_ach_ann->value();
    double  tau_rem_acc     = ui->tau_rem_acc->value();
    QString statut_contrat  = ui->status_contrat->currentText();
    QString clause_penale   = ui->clause_penale->toPlainText().trimmed();

    bool ok;
    int new_id = ui->ID_Contrat->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "ID invalide", "Veuillez saisir un ID numérique valide.");
        return;
    }

    if (!contrat_checkDates()) {
        QMessageBox::warning(this, "Erreur",
            "La date de début doit être au plus aujourd'hui et la date de fin doit être après ou égale à la date de début.");
        return;
    }
    if (id_client == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un Client.");
        return;
    }
    if (cin == 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner un Employé.");
        return;
    }

    Contrat c(id_client, cin, type_ex, prod_con, date_debut, date_fin,
              obj_ach_ann, tau_rem_acc, statut_contrat, clause_penale);
    c.setId_contrat(new_id);

    if (c.modifier(contrat_currentSelectedId)) {
        QMessageBox::information(this, "Succès", "Le contrat a été mis à jour !");
        contrat_currentSelectedId = new_id;
        ui->tab_contrat_2->setModel(Contrat().afficher());
        contrat_refreshStats();
    } else {
        QSqlError err = QSqlDatabase::database().lastError();
        QMessageBox::critical(this, "Erreur lors de la modification",
                              "L'opération a échoué.\n\nDétails techniques :\n" + err.text());
    }
}

// ─── Supprimer un contrat ─────────────────────────────────────────────────────
void MainWindow::contrat_onSupprimerClicked()
{
    bool ok;
    int id = ui->ID_Contrat->text().toInt(&ok);

    if (!ok) {
        QMessageBox::warning(this, "Sélection requise",
            "Veuillez sélectionner un contrat dans le tableau.");
        return;
    }

    if (QMessageBox::question(this, "Confirmation",
            "Voulez-vous vraiment supprimer ce contrat ?",
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if (Contrat().supprimer(id)) {
            QMessageBox::information(this, "Succès", "Contrat supprimé avec succès.");
            ui->tab_contrat_2->setModel(Contrat().afficher());
            ui->ID_Contrat->clear();
            contrat_currentSelectedId = -1;
            contrat_refreshStats();
        } else {
            QMessageBox::critical(this, "Erreur", "La suppression a échoué.");
        }
    }
}

// ─── Exporter PDF ─────────────────────────────────────────────────────────────
void MainWindow::contrat_onExporterClicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,
        "Exporter la liste des contrats", "Liste_Contrats.pdf", "PDF (*.pdf)");
    if (filePath.isEmpty()) return;

    Contrat c;
    QSqlQueryModel *contractModel = c.afficher();
    if (!contractModel) {
        QMessageBox::critical(this, "Erreur",
            "Impossible de charger la liste des contrats pour l'export PDF.");
        return;
    }

    bool ok2 = c.exporterPdf(filePath, contractModel);
    delete contractModel;

    if (ok2) {
        QMessageBox::information(this, "Succès",
            "La liste des contrats a été exportée en PDF avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'export PDF pour les contrats.");
    }
}

// ─── Recherche ────────────────────────────────────────────────────────────────
void MainWindow::contrat_onRechercheTextChanged(const QString &arg1)
{
    Contrat c;
    QSqlQueryModel *m = c.rechercher(arg1);
    contrat_setupModelHeaders(m);
    ui->tab_contrat_2->setModel(m);
}

// ─── Tri ──────────────────────────────────────────────────────────────────────
void MainWindow::contrat_onTriClicked()
{
    QString critere = ui->tri_contrat->currentText();
    QString orderBy;
    if      (critere == "ID contrat")        orderBy = "ID_CONTRAT";
    else if (critere == "Type exclusivité")  orderBy = "TYPE_EXCLUSIVITE";
    else if (critere == "Statut contrat")    orderBy = "STATUT_CONTRAT";
    else                                     orderBy = "ID_CONTRAT";

    QSqlQueryModel *m = new QSqlQueryModel();
    QString sql = QString(
        "SELECT ID_CONTRAT, ID_CLIENT, CIN, TYPE_EXCLUSIVITE, PRODUITS_CONCERNES, "
        "DATE_DEBUT, DATE_FIN, OBJECTIF_ACHAT_ANNUEL, TAUX_REMISE_ACCORDE, STATUT_CONTRAT, CLAUSE_PENALE "
        "FROM CONTRAT ORDER BY %1"
    ).arg(orderBy);

    m->setQuery(sql);
    contrat_setupModelHeaders(m);
    ui->tab_contrat_2->setModel(m);
}

// ─── Style validation ─────────────────────────────────────────────────────────
void MainWindow::contrat_setWidgetStyle(QWidget *widget, bool isValid)
{
    QString style = "background-color: white; color: black; border-radius: 5px; padding: 5px; font-weight: bold;";
    style += isValid ? "border: 2px solid green;" : "border: 2px solid red;";
    widget->setStyleSheet(style);
}

void MainWindow::contrat_resetValidationStyles()
{
    ui->ID_Contrat->setStyleSheet("");
    ui->combo_ID_Client->setStyleSheet("");
    ui->date_debut->setStyleSheet("");
    ui->date_fin->setStyleSheet("");
    ui->obj_ach_ann->setStyleSheet("");
    ui->tau_rem_acc->setStyleSheet("");
    ui->clause_penale->setStyleSheet("");
}

bool MainWindow::contrat_checkDates()
{
    QDate debut = ui->date_debut->date();
    QDate fin   = ui->date_fin->date();

    bool debutOk = Contrat::validerDateDebut(debut);
    bool finOk   = Contrat::validerDateFin(fin, debut);

    contrat_setWidgetStyle(ui->date_debut, debutOk);
    contrat_setWidgetStyle(ui->date_fin,   finOk);

    return debutOk && finOk;
}

void MainWindow::contrat_validateID()
{
    QString text = ui->ID_Contrat->text().trimmed();
    if (text.isEmpty()) {
        ui->ID_Contrat->setStyleSheet("");
    } else {
        bool idOk;
        int id = text.toInt(&idOk);
        contrat_setWidgetStyle(ui->ID_Contrat, idOk && Contrat::validerID(id));
    }

    if (ui->combo_ID_Client->currentIndex() <= 0) {
        ui->combo_ID_Client->setStyleSheet("");
    } else {
        contrat_setWidgetStyle(ui->combo_ID_Client, true);
    }
}

void MainWindow::contrat_validateDates()
{
    contrat_checkDates();
}

void MainWindow::contrat_validateFloats()
{
    double obj = ui->obj_ach_ann->value();
    double tau = ui->tau_rem_acc->value();

    if (obj == 0.0) {
        ui->obj_ach_ann->setStyleSheet("");
    } else {
        contrat_setWidgetStyle(ui->obj_ach_ann, Contrat::validerFloats(obj, tau));
    }

    if (tau == 0.0) {
        ui->tau_rem_acc->setStyleSheet("");
    } else {
        contrat_setWidgetStyle(ui->tau_rem_acc, Contrat::validerFloats(obj, tau));
    }
}

void MainWindow::contrat_validateDescription()
{
    QString text = ui->clause_penale->toPlainText().trimmed();
    if (text.isEmpty()) {
        ui->clause_penale->setStyleSheet("");
    } else {
        contrat_setWidgetStyle(ui->clause_penale, Contrat::validerDescription(text));
    }
}

// =========================================================
// ===                  MODULE CLIENT                    ===
// =========================================================

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
