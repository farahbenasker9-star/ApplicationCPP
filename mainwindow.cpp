#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QDateTime>
#include <QtCharts/QChartView>
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

    // 2. INITIALISATION DES MANAGERS (Très important pour le CRUD)
    employe = new Employe(ui, this);
    client  = new Client(ui, this); // Initialise ta gestion client
    equipement = new Equipement(ui, this);

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

    // Assurer la recherche Contrat est utilisable
    ui->recherche0_contrat->setReadOnly(false);
    ui->recherche0_contrat->setEnabled(true);
    ui->recherche0_contrat->setFocusPolicy(Qt::StrongFocus);
    ui->recherche0_contrat->setPlaceholderText("🔍 Rechercher par ID contrat, Type, Statut...");
    ui->recherche0_contrat->setClearButtonEnabled(true);
    ui->recherche0_contrat->setStyleSheet(
        "QLineEdit { background: #e8eceb; color: #3a3a3a; border: 1px solid #bfbfbf; border-radius: 8px; padding: 8px; } "
        "QLineEdit:focus { background: #e8eceb; color: #222; border: 1px solid #6aa84f; }");

    // Connexion explicite du signal de recherche (quand auto-connect échoue)
    connect(ui->recherche0_contrat, &QLineEdit::textChanged,
            this, &MainWindow::on_recherche0_contrat_textChanged);

    // Installer un event filter pour focus action
    ui->recherche0_contrat->installEventFilter(this);

    // Initialiser les statistiques contrat
    setupContratStatsUI();
    refreshContratStats();
}

void MainWindow::setupContratStatsUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->gb_stats_contrat);
    mainLayout->setSpacing(4);
    mainLayout->setContentsMargins(2, 2, 2, 2);

    // Limitez la taille verticale pour s'adapter à l'espace disponible
    chartViewContratType = new QChartView();
    chartViewContratType->setRenderHint(QPainter::Antialiasing);
    chartViewContratType->setMinimumHeight(150);
    chartViewContratType->setMinimumWidth(0);
    chartViewContratType->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    chartViewContratType->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartViewContratType->setContentsMargins(2, 2, 2, 2);

    chartViewContratTypePie = new QChartView();
    chartViewContratTypePie->setRenderHint(QPainter::Antialiasing);
    chartViewContratTypePie->setMinimumHeight(150);
    chartViewContratTypePie->setMinimumWidth(0);
    chartViewContratTypePie->setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    chartViewContratTypePie->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartViewContratTypePie->setContentsMargins(2, 2, 2, 2);

    mainLayout->addWidget(chartViewContratType);
    mainLayout->addWidget(chartViewContratTypePie);

    ui->gb_stats_contrat->setLayout(mainLayout);
}

void MainWindow::refreshContratStats()
{
    QSqlQuery query;

    // 1 - Histogramme: Nombre contrats par type d'exclusivité
    QChart *chartType = new QChart();
    QBarSeries *barSeries = new QBarSeries();
    QBarSet *setType = new QBarSet("Contrats");

    QStringList typeCategories;
    if (query.exec("SELECT TYPE_EXCLUSIVITE, COUNT(*) FROM CONTRAT GROUP BY TYPE_EXCLUSIVITE ORDER BY COUNT(*) DESC")) {
        while (query.next()) {
            QString type = query.value(0).toString();
            if (type.isEmpty()) type = "Indéfini";
            typeCategories << type;
            *setType << query.value(1).toDouble();
        }
    }

    barSeries->append(setType);
    barSeries->setBarWidth(0.35); // barres plus fines, adaptation à l'espace
    chartType->addSeries(barSeries);
    chartType->setTitle("Contrats par Type d'exclusivité");
    chartType->setAnimationOptions(QChart::SeriesAnimations);
    chartType->setMargins(QMargins(4, 4, 4, 4));
    chartType->legend()->setVisible(false);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(typeCategories);
    axisX->setLabelsAngle(-35);
    axisX->setGridLineVisible(false);
    axisX->setLabelsFont(QFont("Arial", 8));
    chartType->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%i");
    axisY->setTitleText("Nbre contrats");
    axisY->setTickCount(4);
    axisY->setLabelsFont(QFont("Arial", 8));
    chartType->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    chartViewContratType->setChart(chartType);

    // 2 - Camembert : Part (%) par statut contrat
    QChart *chartPie = new QChart();
    QPieSeries *pieSeries = new QPieSeries();

    if (query.exec("SELECT STATUT_CONTRAT, COUNT(*) FROM CONTRAT GROUP BY STATUT_CONTRAT")) {
        while (query.next()) {
            QString statut = query.value(0).toString();
            if (statut.isEmpty()) statut = "Indéfini";
            int count = query.value(1).toInt();
            QPieSlice *slice = pieSeries->append(statut + " (" + QString::number(count) + ")", count);
            slice->setLabelVisible(true);
        }
    }

    pieSeries->setPieStartAngle(30); // orienter le début du camembert vers la gauche
    pieSeries->setHoleSize(0.35); // camembert plus compact dans l'espace

    chartPie->addSeries(pieSeries);
    chartPie->setTitle("Répartition des statuts des contrats");
    chartPie->legend()->setAlignment(Qt::AlignRight);
    chartPie->setMargins(QMargins(4, 4, 4, 4));
    chartPie->setAnimationOptions(QChart::SeriesAnimations);
    chartViewContratTypePie->setChart(chartPie);
}


// ─── Destructeur ─────────────────────────────────────────────────────────────
MainWindow::~MainWindow()
{
    delete ui;
}

// ─── Navigation Slots ────────────────────────────────────────────────────────

void MainWindow::on_btn_nav_employes_clicked() { ui->stackedWidget->setCurrentIndex(6); }
void MainWindow::on_btn_nav_client_clicked() { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_btn_nav_contrat_clicked() { navigateToPage(2); }
void MainWindow::on_btn_nav_poubelle_clicked() { ui->stackedWidget->setCurrentIndex(7); }
void MainWindow::on_btn_nav_equipements_clicked() { ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_btn_nav_stock_clicked() { ui->stackedWidget->setCurrentIndex(4); }

void MainWindow::on_btn_dash_poubelle_clicked() { ui->stackedWidget->setCurrentIndex(7); }
void MainWindow::on_btn_dash_employe_clicked() { ui->stackedWidget->setCurrentIndex(6); }
void MainWindow::on_btn_dash_client_clicked() { ui->stackedWidget->setCurrentIndex(5); }
void MainWindow::on_btn_dash_contrat_clicked() { navigateToPage(2); }
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


// ─── Navigation par index ────────────────────────────────────────────────────
void MainWindow::navigateToPage(int pageIndex)
{
    if (pageIndex == 2) { // Page Contrat
        populateComboBoxes();
        ui->tab_contrat_2->setModel(Contrat().afficher());
        refreshContratStats();
    }
    ui->stackedWidget->setCurrentIndex(pageIndex);
    if (pageIndex == 4) {
        Produit P;
        ui->tab_stock->setModel(P.afficher());
    }
}

void MainWindow::on_ajouter_contrat_clicked()
{
    // 1. Récupérer les valeurs de tous les champs
    int    id_client         = ui->combo_ID_Client->currentText().toInt();
    int    cin               = ui->combo_CIN_Employe->currentText().toInt();
    QString type_ex          = ui->type_ex->currentText();
    QString prod_con         = ui->prod_con->currentText();
    QDate   date_debut       = ui->date_debut->date();
    QDate   date_fin         = ui->date_fin->date();
    double  obj_ach_ann      = ui->obj_ach_ann->value();
    double  tau_rem_acc      = ui->tau_rem_acc->value();
    QString statut_contrat   = ui->status_contrat->currentText();
    QString clause_penale    = ui->clause_penale->toPlainText().trimmed();

    // 2. Validation basique
    if (!checkContratDates()) {
        QMessageBox::warning(this, "Erreur", "La date de début doit être au plus aujourd'hui et la date de fin doit être après ou égale à la date de début.");
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

    // 3. Créer l'objet Contrat
    Contrat c(id_client, cin, type_ex, prod_con, date_debut, date_fin,
              obj_ach_ann, tau_rem_acc, statut_contrat, clause_penale);

    // 4. Ajouter à la base
    if (c.ajouter()) {
        QMessageBox::information(this, "Succès", "Le contrat a été ajouté avec succès !");
        
        // Nettoyage et rafraîchissement
        ui->ID_Contrat->clear();
        ui->combo_ID_Client->setCurrentIndex(0);
        ui->combo_CIN_Employe->setCurrentIndex(0);
        ui->tab_contrat_2->setModel(Contrat().afficher());
        resetValidationStyles();
    } else {
        // Affichage de l'erreur détaillée pour aider au diagnostic
        QSqlError err = QSqlDatabase::database().lastError();
        QMessageBox::critical(this, "Erreur lors de l'ajout", 
            "L'opération a échoué.\n\nDétails techniques :\n" + err.text());
    }
}

void MainWindow::on_exporter_contrat_clicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter la liste des contrats", "Liste_Contrats.pdf", "PDF (*.pdf)");
    if (filePath.isEmpty()) {
        return;
    }

    Contrat c;
    QSqlQueryModel *contractModel = c.afficher();
    if (!contractModel) {
        QMessageBox::critical(this, "Erreur", "Impossible de charger la liste des contrats pour l'export PDF.");
        return;
    }

    bool ok = c.exporterPdf(filePath, contractModel);
    delete contractModel;

    if (ok) {
        QMessageBox::information(this, "Succès", "La liste des contrats a été exportée en PDF avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'export PDF pour les contrats.");
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

    // Remplissage du dropdown CIN Employé
    QString cin_val = model->data(model->index(row, 2)).toString();
    int cinIdx = ui->combo_CIN_Employe->findText(cin_val);
    if (cinIdx != -1) ui->combo_CIN_Employe->setCurrentIndex(cinIdx);
    
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
    int    cin               = ui->combo_CIN_Employe->currentText().toInt();
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

    if (!checkContratDates()) {
        QMessageBox::warning(this, "Erreur", "La date de début doit être au plus aujourd'hui et la date de fin doit être après ou égale à la date de début.");
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
        setWidgetStyle(ui->ID_Contrat, idOk && Contrat::validerID(id));
    }
    
    if (ui->combo_ID_Client->currentIndex() <= 0) { // "Sélectionner..." ou vide
        ui->combo_ID_Client->setStyleSheet("");
    } else {
        setWidgetStyle(ui->combo_ID_Client, true); // Si sélectionné, c'est forcément valide
    }
}

bool MainWindow::checkContratDates() {
    QDate debut = ui->date_debut->date();
    QDate fin = ui->date_fin->date();

    bool debutOk = Contrat::validerDateDebut(debut);
    bool finOk = Contrat::validerDateFin(fin, debut);

    setWidgetStyle(ui->date_debut, debutOk);
    setWidgetStyle(ui->date_fin, finOk);

    return debutOk && finOk;
}

void MainWindow::validateDates() {
    checkContratDates();
}

void MainWindow::validateFloats() {
    double obj = ui->obj_ach_ann->value();
    double tau = ui->tau_rem_acc->value();

    if (obj == 0.0) {
        ui->obj_ach_ann->setStyleSheet("");
    } else {
        setWidgetStyle(ui->obj_ach_ann, Contrat::validerFloats(obj, tau));
    }

    if (tau == 0.0) {
        ui->tau_rem_acc->setStyleSheet("");
    } else {
        setWidgetStyle(ui->tau_rem_acc, Contrat::validerFloats(obj, tau));
    }
}

void MainWindow::validateDescription() {
    QString text = ui->clause_penale->toPlainText().trimmed();
    if (text.isEmpty()) {
        ui->clause_penale->setStyleSheet("");
    } else {
        setWidgetStyle(ui->clause_penale, Contrat::validerDescription(text));
    }
}

bool MainWindow::eventFilter(QObject *watched, QEvent *event) {
    if (watched == ui->recherche0_contrat) {
        if (event->type() == QEvent::FocusIn) {
            // Réinitialiser le texte si le placeholder était affiché
            if (ui->recherche0_contrat->text().isEmpty()) {
                ui->recherche0_contrat->clear();
            }
            ui->recherche0_contrat->setStyleSheet("QLineEdit { background: white; color: black; border: 1px solid #4CAF50; border-radius: 8px; padding: 5px; } "
                                                 "QLineEdit:focus { background: white; color: black; }");
        } else if (event->type() == QEvent::FocusOut) {
            if (ui->recherche0_contrat->text().isEmpty()) {
                ui->recherche0_contrat->setStyleSheet("QLineEdit { background: #f5f5f5; color: #555; border: 1px solid #c8c8c8; border-radius: 8px; padding: 5px; } "
                                                     "QLineEdit:focus { background: white; color: black; border: 1px solid #4CAF50; }");
            }
        }
    }
    return QMainWindow::eventFilter(watched, event);
}

void MainWindow::populateComboBoxes() {
    // 1. Dropdown Client
    ui->combo_ID_Client->clear();
    ui->combo_ID_Client->setEditable(true);
    ui->combo_ID_Client->addItem("Sélectionner...", 0);

    QSqlQuery qClient("SELECT ID_CLIENT FROM CLIENT ORDER BY ID_CLIENT ASC");
    while (qClient.next()) {
        ui->combo_ID_Client->addItem(qClient.value(0).toString());
    }

    // 2. Dropdown Employé (CIN)
    ui->combo_CIN_Employe->clear();
    ui->combo_CIN_Employe->setEditable(true);
    ui->combo_CIN_Employe->addItem("Sélectionner...", 0);

    QSqlQuery qEmp("SELECT CIN FROM EMPLOYE ORDER BY CIN ASC");
    while (qEmp.next()) {
        ui->combo_CIN_Employe->addItem(qEmp.value(0).toString());
    }
}

void MainWindow::setupContratModelHeaders(QSqlQueryModel *model)
{
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("CIN Employé"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type Exclusivité"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Produits"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date Fin"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Obj. Achat"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Taux Rem."));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Clause Pénale"));
}

void MainWindow::on_recherche0_contrat_textChanged(const QString &arg1)
{
    Contrat c;
    QSqlQueryModel *model = c.rechercher(arg1);
    setupContratModelHeaders(model);
    ui->tab_contrat_2->setModel(model);
}

void MainWindow::on_tri2_contrat_clicked()
{
    QString critere = ui->tri_contrat->currentText();
    QString orderBy;
    if (critere == "ID contrat") {
        orderBy = "ID_CONTRAT";
    } else if (critere == "Type exclusivité") {
        orderBy = "TYPE_EXCLUSIVITE";
    } else if (critere == "Statut contrat") {
        orderBy = "STATUT_CONTRAT";
    } else {
        orderBy = "ID_CONTRAT";
    }

    QSqlQueryModel *model = new QSqlQueryModel();
    QString sql = QString(
        "SELECT ID_CONTRAT, ID_CLIENT, CIN, TYPE_EXCLUSIVITE, PRODUITS_CONCERNES, "
        "DATE_DEBUT, DATE_FIN, OBJECTIF_ACHAT_ANNUEL, TAUX_REMISE_ACCORDE, STATUT_CONTRAT, CLAUSE_PENALE "
        "FROM CONTRAT ORDER BY %1"
    ).arg(orderBy);

    model->setQuery(sql);
    setupContratModelHeaders(model);
    ui->tab_contrat_2->setModel(model);
}

void MainWindow::on_btn_ajouter_4_clicked() {
    if (!controleSaisie()) return; // On s'arrête si c'est faux
    // Récupération des données
    int id = ui->le_id->text().toInt();
    int id_c = ui->le_id_client_2->text().toInt();
    QString type = ui->cb_type->currentText();
    float poids = ui->sb_poids->value();
    float prix = ui->sb_prix->value();
    QDate dc = ui->de_date_c->date();
    QDate dv = ui->de_date_v->date();
    QString statut = ui->cb_statut->currentText();

    Produit P(id, id_c, type, poids, dc, dv, statut, prix);

    if(P.ajouter()) {
        ui->tab_stock->setModel(P.afficher()); // Refresh du tableau
        QMessageBox::information(this, "Succès", "Produit ajouté !");
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout échoué.");
    }
}
void MainWindow::on_btn_supprimer_5_clicked() {
    int id = ui->le_id->text().toInt(); // On récupère l'ID à supprimer
    Produit P;
    if(P.supprimer(id)) {
        ui->tab_stock->setModel(P.afficher());
        QMessageBox::information(this, "Succès", "Produit supprimé !");
    }
}
void MainWindow::on_btn_modifier_4_clicked() {
    if (!controleSaisie()) return;
    Produit P(ui->le_id->text().toInt(), ui->le_id_client_2->text().toInt(),
              ui->cb_type->currentText(), ui->sb_poids->value(),
              ui->de_date_c->date(), ui->de_date_v->date(),
              ui->cb_statut->currentText(), ui->sb_prix->value());
    if(P.modifier()) {
        ui->tab_stock->setModel(P.afficher());
        QMessageBox::information(this, "Succès", "Mise à jour effectuée !");
    }
}
void MainWindow::on_cb_statut_currentIndexChanged(int index)
{
    QString statut = ui->cb_statut->currentText();

    if (statut == "Vendu") {
        ui->le_id_client_2->setEnabled(true);
        ui->de_date_v->setEnabled(true);
        ui->de_date_v->setDate(QDate::currentDate()); // Met la date du jour par défaut
    }
    else if (statut == "Réservé") {
        ui->le_id_client_2->setEnabled(true); // On a besoin de l'ID pour une réservation
        ui->de_date_v->setEnabled(false);   // Pas encore de date de vente
    }
    else { // Cas "Disponible"
        ui->le_id_client_2->setEnabled(false);
        ui->de_date_v->setEnabled(false);
        ui->le_id_client_2->clear(); // On vide l'ID client pour la propreté
    }
}

void MainWindow::on_tab_stock_clicked(const QModelIndex &index)
{
    int row = index.row();
    auto model = ui->tab_stock->model();

    // 1. On récupère d'abord le STATUT (Colonne index 6)
    QString statut_db = model->index(row, 6).data().toString().trimmed();

    // 2. On met à jour le ComboBox du statut
    ui->cb_statut->setCurrentText(statut_db);

    // 3. TRÈS IMPORTANT : On appelle la fonction qui active/désactive les cases
    // Il faut que la case "ID client" soit ACTIVÉE avant de mettre le texte
    on_cb_statut_currentIndexChanged(ui->cb_statut->currentIndex());

    // 4. Maintenant que la case est peut-être dégrisée, on met l'ID Client (Index 1)
    QString id_client_db = model->index(row, 1).data().toString().trimmed();
    ui->le_id_client_2->setText(id_client_db);

    // 5. On remplit le reste normalement
    ui->le_id->setText(model->index(row, 0).data().toString());
    ui->cb_type->setCurrentText(model->index(row, 2).data().toString().trimmed());
    ui->sb_poids->setValue(model->index(row, 3).data().toDouble());
    ui->de_date_c->setDate(model->index(row, 4).data().toDate());
    ui->de_date_v->setDate(model->index(row, 5).data().toDate());
    ui->sb_prix->setValue(model->index(row, 7).data().toDouble());
}
bool MainWindow::controleSaisie() {
    QString id_p = ui->le_id->text();
    double prix = ui->sb_prix->value();
    double poids = ui->sb_poids->value(); // On récupère la valeur du poids
    QString statut = ui->cb_statut->currentText();
    QString id_c = ui->le_id_client_2->text();

    // 1. Vérifier que l'ID fait exactement 8 chiffres
    if (id_p.length() != 8) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID Produit doit contenir exactement 8 chiffres.");
        return false;
    }

    // 2. Vérifier que le poids est bien supérieur à 0
    if (poids <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le poids du produit doit être supérieur à 0 kg.");
        return false;
    }

    // 3. Vérifier que le prix est bien supérieur à 0
    if (prix <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le prix de vente doit être supérieur à 0.00 DT.");
        return false;
    }

    // 4. Sécurité supplémentaire : Si Vendu ou Réservé, l'ID Client ne doit pas être vide
    if (statut != "Disponible" && id_c.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Un ID Client est obligatoire pour un produit Réservé ou Vendu.");
        return false;
    }

    return true; // Tout est valide !
}
