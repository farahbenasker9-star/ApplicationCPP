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
#include <QLabel>
#include <QHeaderView>

#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QStyledItemDelegate>

// ==================== CUSTOM DELEGATE FOR CIN ====================
class CinDelegate : public QStyledItemDelegate
{
public:
    CinDelegate(QObject *parent = nullptr) : QStyledItemDelegate(parent) {}

    QString displayText(const QVariant &value, const QLocale &locale) const override
    {
        bool ok = false;
        double cinDouble = value.toDouble(&ok);
        if (ok) {
            return QString::number(static_cast<qulonglong>(cinDouble)).rightJustified(8, '0');
        }
        return QStyledItemDelegate::displayText(value, locale);
    }
};

// ─── Constructeur ────────────────────────────────────────────────────────────
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QSqlQueryModel(this);

    // Initialisation de l'affichage des poubelles
    ui->tab_poubelle->setModel(tmp_poubelle.afficher());

    // Module poubelle: slots `on_<object>_<signal>` are auto-connected by setupUi.

    // Initialisation du formulaire avec VALEURS PAR DÉFAUT
    clearFormPoubelle();
    
    // S'assurer que l'application démarre bien sur le Dashboard (index 0)
    ui->stackedWidget->setCurrentIndex(0);
    ui->navbar_container->hide();
    
    // Initialisation des gestionnaires de modules
    // ==================== MODULE EQUIPEMENT ====================
    // ── 1. Créer les labels ✕ d'erreur dans infoScrollContent ────────────────
    //    Géométries depuis le .ui (x, y, w, h) :
    //      Eq_CIN          x=160  y=20   w=121  h=41
    //      Eq_type         x=160  y=70   w=117  h=40
    //      Eq_Fab          x=160  y=120  w=121  h=40
    //      Eq_Statut       x=160  y=170  w=151  h=40
    //      Eq_Next_Maint   x=160  y=330  w=161  h=37
    // ─────────────────────────────────────────────────────────────────────────
    QWidget *eqForm = ui->infoScrollArea->widget();

    auto makeEqErr = [&](int x, int y) -> QLabel* {
        QLabel *lbl = new QLabel("✕", eqForm);
        lbl->setGeometry(x, y, 22, 22);
        lbl->setStyleSheet(
            "QLabel { color: #D32F2F; font-weight: bold; font-size: 14px;"
            " background: transparent; border: none; }");
        lbl->setVisible(false);
        return lbl;
    };

    // Position each ✕ just to the right of its widget, vertically centred
    eq_errCIN       = makeEqErr(160 + 121 + 4,  29);   // Eq_CIN
    eq_errType      = makeEqErr(160 + 117 + 4,  79);   // Eq_type
    eq_errFab       = makeEqErr(160 + 121 + 4, 129);   // Eq_Fab
    eq_errStatut    = makeEqErr(160 + 151 + 4, 179);   // Eq_Statut
    eq_errNouveaute = makeEqErr(160 + 151 + 4, 229);   // Eq_Statut_2 (Nouveauté)
    eq_errNextMaint = makeEqErr(160 + 161 + 4, 339);   // Eq_Next_Maint_Date

    // ── 2. Dropdown list colours — applied once; validation helpers preserve this ─
    QString dropFix =
        "QComboBox QAbstractItemView {"
        "  background-color: white; color: #222222;"
        "  border: 1px solid #BDBDBD;"
        "  selection-background-color: #4CAF50; selection-color: white; outline: 0;"
        "}";
    ui->Eq_CIN->setStyleSheet(dropFix);
    ui->Eq_type->setStyleSheet(dropFix);
    ui->Eq_Fab->setStyleSheet(dropFix);
    ui->Eq_Statut->setStyleSheet(dropFix);
    ui->Eq_Statut_2->setStyleSheet(dropFix);

    // ── 3. Affichage initial + CIN combo ─────────────────────────────────────
    equipement_chargerCIN();
    ui->tab_employes_2->setModel(Equipement().afficher());

    // ── 4. Dates par défaut ───────────────────────────────────────────────────
    QDateTime now = QDateTime::currentDateTime();
    ui->Eq_Last_Maint_Date->setDateTime(now);
    ui->Eq_Next_Maint_Date->setDateTime(now);

    // ── 5. Connexions CRUD ────────────────────────────────────────────────────
    connect(ui->btn_ajouter_2,   &QPushButton::clicked, this, &MainWindow::equipement_onAjouterClicked);
    connect(ui->btn_modifier_2,  &QPushButton::clicked, this, &MainWindow::equipement_onModifierClicked);
    connect(ui->btn_supprimer_2, &QPushButton::clicked, this, &MainWindow::equipement_onSupprimerClicked);
    connect(ui->tab_employes_2,  &QTableView::clicked,  this, &MainWindow::equipement_onTabClicked);

    // ── 6. Recherche / tri ────────────────────────────────────────────────────
    connect(ui->le_recherche_2,     &QLineEdit::textChanged,
            this, &MainWindow::equipement_onRechercheTextChanged);
    connect(ui->btn_tri_3,          &QPushButton::clicked,
            this, &MainWindow::equipement_onTriClicked);

    // ── 7. Validation en temps réel ───────────────────────────────────────────
    connect(ui->Eq_CIN,    qOverload<int>(&QComboBox::currentIndexChanged),
            this, &MainWindow::equipement_onCINChanged);
    connect(ui->Eq_Statut, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &MainWindow::equipement_onStatutChanged);
    connect(ui->Eq_Statut_2, qOverload<int>(&QComboBox::currentIndexChanged),
            this, &MainWindow::equipement_onNouveauteChanged);
    connect(ui->Eq_type,   qOverload<int>(&QComboBox::currentIndexChanged),
            this, &MainWindow::equipement_validateType);
    connect(ui->Eq_Fab,    qOverload<int>(&QComboBox::currentIndexChanged),
            this, &MainWindow::equipement_validateFabricant);
    connect(ui->Eq_Next_Maint_Date, &QDateTimeEdit::dateTimeChanged,
            this, &MainWindow::equipement_validateDateSuivMaint);
    equipement_setupStatsUI();
    equipement_refreshStats();

    // ==================== MODULE EMPLOYE ====================
    ui->tab_employes->setModel(tmp_employe.afficher());
    ui->tab_employes->setItemDelegateForColumn(0, new CinDelegate(this));
    ui->tab_employes->setAlternatingRowColors(true);
    ui->tab_employes->resizeColumnsToContents();

    connect(ui->tab_employes,   &QTableView::clicked,  this, &MainWindow::employe_onTabClicked);
    connect(ui->btn_ajouter,    &QPushButton::clicked, this, &MainWindow::employe_onAjouterClicked);
    connect(ui->btn_modifier,   &QPushButton::clicked, this, &MainWindow::employe_onModifierClicked);
    connect(ui->btn_supprimer,  &QPushButton::clicked, this, &MainWindow::employe_onSupprimerClicked);
    connect(ui->le_recherche,   &QLineEdit::textChanged, this, &MainWindow::employe_onRechercheTextChanged);
    connect(ui->btn_tri,        &QPushButton::clicked, this, &MainWindow::employe_onTriClicked);
    connect(ui->btn_pdf,        &QPushButton::clicked, this, &MainWindow::employe_onPdfClicked);
    connect(ui->btn_refresh,    &QPushButton::clicked, this, &MainWindow::employe_onRefreshClicked);
    connect(ui->btn_generer_contrat_emp, &QPushButton::clicked, this, &MainWindow::employe_onGenererContratClicked);
    employe_setupStatsUI();
    employe_refreshStats();

    // ==================== MODULE CLIENT ====================
    ui->tab_clients->setModel(model);
    ui->tab_clients->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab_clients->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tab_clients->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    rafraichirAffichage();

    connect(ui->btn_ajouter_client,  &QPushButton::clicked, this, &MainWindow::onBtnAjouterClicked);
    connect(ui->btn_modifier_client, &QPushButton::clicked, this, &MainWindow::onBtnModifierClicked);
    connect(ui->btn_supprimer_client,&QPushButton::clicked, this, &MainWindow::onBtnSupprimerClicked);
    connect(ui->tab_clients,         &QTableView::clicked,  this, &MainWindow::onTableClicked);
    connect(ui->le_recherche_client, &QLineEdit::textChanged, this, &MainWindow::onRechercheTextChanged);

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
void MainWindow::on_btn_nav_poubelle_clicked()   {
    ui->stackedWidget->setCurrentIndex(7);
    ui->tab_poubelle->setModel(tmp_poubelle.afficher());
    clearFormPoubelle();
}
void MainWindow::on_btn_nav_equipements_clicked(){ ui->stackedWidget->setCurrentIndex(3); }
void MainWindow::on_btn_nav_stock_clicked()      { ui->stackedWidget->setCurrentIndex(4); }

void MainWindow::on_btn_dash_poubelle_clicked()  {
    ui->stackedWidget->setCurrentIndex(7);
    ui->tab_poubelle->setModel(tmp_poubelle.afficher());
    clearFormPoubelle();
}
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
    if (pageIndex == 3) { // Page Equipement
        equipement_chargerCIN();
        equipement_rafraichirTable();
        equipement_refreshStats();
    }
    if (pageIndex == 4) {
        Produit P;
        ui->tab_produit->setModel(P.afficher());
        chargerIdsClients();
    }
    if (pageIndex == 7) {
        ui->tab_poubelle->setModel(tmp_poubelle.afficher());
        clearFormPoubelle();
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

    int remplissage = (etat == "EN_STOCK" || etat == "EN STOCK") ? 0 : ui->sb_remplissage->value();
    int batterie = (etat == "EN_STOCK" || etat == "EN STOCK") ? 100 : ui->sb_batterie->value();
    QString statut_capteur = ui->cb_statut_capteur->currentText();
    QDate date_inst = (etat == "EN_STOCK" || etat == "EN STOCK") ? QDate::currentDate() : ui->de_installation->date();
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
    if (poubelle_currentSelectedId == -1) {
        QMessageBox::warning(this, "Selection", "Veuillez sélectionner une poubelle à modifier dans le tableau.");
        return;
    }

    if (!validerFormulairePoubelle(true)) return;

    int id = ui->le_id_poubelle->text().toInt();
    if (id != poubelle_currentSelectedId && tmp_poubelle.existe(id)) {
        QMessageBox::warning(this, "Erreur", "ID existant. Veuillez saisir un ID unique.");
        return;
    }

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

    if(p.modifier(poubelle_currentSelectedId)) {
        QMessageBox::information(this, "Succès", "Poubelle modifiée");
        ui->tab_poubelle->setModel(tmp_poubelle.afficher());
        clearFormPoubelle();
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de la modification.\nDétail : " + p.getLastError());
    }
}

void MainWindow::on_tab_poubelle_clicked(const QModelIndex &index)
{
    int row = index.row();
    QAbstractItemModel *model = ui->tab_poubelle->model();
    poubelle_currentSelectedId = model->data(model->index(row, 0)).toInt();
    ui->le_id_poubelle->setText(model->data(model->index(row, 0)).toString());
    ui->le_id_poubelle->setEnabled(true);
    ui->le_type_dechet->setText(model->data(model->index(row, 1)).toString());
    ui->le_adresse_poubelle->setText(model->data(model->index(row, 2)).toString());
    ui->sb_capacite->setValue(model->data(model->index(row, 3)).toInt());
    ui->cb_etat_poubelle->setCurrentText(model->data(model->index(row, 4)).toString());
    ui->sb_remplissage->setValue(model->data(model->index(row, 5)).toInt());
    ui->sb_batterie->setValue(model->data(model->index(row, 6)).toInt());
    ui->cb_statut_capteur->setCurrentText(model->data(model->index(row, 7)).toString());
    ui->de_installation->setDate(model->data(model->index(row, 8)).toDate());
    ui->de_collecte->setDate(model->data(model->index(row, 9)).toDate());

    // Ensure editable/locked fields always match the selected state.
    on_cb_etat_poubelle_currentIndexChanged(ui->cb_etat_poubelle->currentIndex());
}

void MainWindow::on_cb_etat_poubelle_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    QString etat = ui->cb_etat_poubelle->currentText().trimmed().toUpper();
    bool estInstalle = (etat == "INSTALLE" || etat == "INSTALLÉ");

    // These fields stay editable regardless of state.
    ui->le_adresse_poubelle->setEnabled(true);
    ui->le_type_dechet->setEnabled(true);
    ui->sb_capacite->setEnabled(true);
    ui->sb_capacite->setMinimum(estInstalle ? 1 : 0);
    if (estInstalle && ui->sb_capacite->value() < 1) {
        ui->sb_capacite->setValue(1);
    }

    // Only editable when the bin is installed.
    ui->sb_remplissage->setEnabled(estInstalle);
    ui->sb_batterie->setEnabled(estInstalle);
    ui->de_installation->setEnabled(estInstalle);
    ui->de_collecte->setEnabled(estInstalle);
    ui->cb_statut_capteur->setEnabled(estInstalle);

    if (!estInstalle) {
        // Keep stable defaults while locked in EN_STOCK mode.
        ui->sb_remplissage->setValue(0);
        ui->sb_batterie->setValue(100);
        ui->cb_statut_capteur->setCurrentIndex(-1);
        ui->de_installation->setDate(QDate::currentDate());
        ui->de_collecte->setDate(QDate::currentDate().addDays(7));
    }
}

void MainWindow::clearFormPoubelle()
{
    poubelle_currentSelectedId = -1;
    ui->le_id_poubelle->clear();
    ui->le_id_poubelle->setEnabled(true);
    ui->le_adresse_poubelle->clear();
    ui->le_type_dechet->clear();
    ui->sb_capacite->setValue(0);
    ui->cb_etat_poubelle->setCurrentIndex(0);
    ui->sb_remplissage->setValue(0);
    ui->sb_batterie->setValue(100);
    ui->cb_statut_capteur->setCurrentIndex(-1);
    ui->de_installation->setDate(QDate::currentDate());
    ui->de_collecte->setDate(QDate::currentDate().addDays(7));

    // Force lock/unlock according to default state even if index did not change.
    on_cb_etat_poubelle_currentIndexChanged(ui->cb_etat_poubelle->currentIndex());
}

bool MainWindow::validerFormulairePoubelle(bool isUpdate)
{
    QString idStr = ui->le_id_poubelle->text();
    QString adresse = ui->le_adresse_poubelle->text();
    QString type = ui->le_type_dechet->text();
    QString etat = ui->cb_etat_poubelle->currentText().trimmed().toUpper();
    bool estInstallee = (etat == "INSTALLE" || etat == "INSTALLÉ");
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

    QDate dateInst = ui->de_installation->date();
    QDate dateColl = ui->de_collecte->date();
    QDate today = QDate::currentDate();

    if (estInstallee) {
        if (dateColl < today) {
            QMessageBox::warning(this, "Erreur", "La date de collecte ne peut pas être dans le passé.");
            return false;
        }
        if (dateColl <= dateInst) {
            QMessageBox::warning(this, "Erreur", "La date de collecte doit être strictement supérieure à la date d'installation.");
            return false;
        }
        if (dateColl.year() < 2026 || dateColl.year() > 2026) {
            QMessageBox::warning(this, "Erreur", "La date de collecte ne doit pas être en dehors de l'année 2026.");
            return false;
        }
    }
    
    if (estInstallee) {
        if (ui->sb_capacite->value() <= 0) {
            QMessageBox::warning(this, "Erreur", "La capacite doit etre strictement superieure a 0 pour une poubelle installee.");
            return false;
        }
        if (ui->de_installation->date() > QDate::currentDate()) {
            QMessageBox::warning(this, "Erreur", "La date d'installation ne peut pas etre dans le futur.");
            return false;
        }
    }
    return true;
}

void MainWindow::on_btn_tri_poubelle_clicked()
{
    QString critere = ui->cb_tri_poubelle->currentText().trimmed();
    QString tri;
    
    if (critere == "Niveau de remplissage") tri = "NIVEAU_REMPLISSAGE DESC";
    else if (critere == "Etat batterie") tri = "ETAT_BATTERIE";
    else if (critere == "Adresse") tri = "ADRESSE";
    else {
        // Fallback si le texte ne correspond pas exactement (ex: espaces invisibles)
        int index = ui->cb_tri_poubelle->currentIndex();
        if (index == 0) tri = "NIVEAU_REMPLISSAGE DESC";
        else if (index == 1) tri = "ETAT_BATTERIE";
        else if (index == 2) tri = "ADRESSE";
    }

    ui->tab_poubelle->setModel(tmp_poubelle.trier(tri));
}

void MainWindow::on_le_recherche_poubelle_textChanged(const QString &arg1)
{
    QString critere = ui->comboBox_client_2->currentText();
    ui->tab_poubelle->setModel(tmp_poubelle.rechercher(critere, arg1));
}

void MainWindow::on_btn_pdf_poubelle_clicked()
{
    int row = ui->tab_poubelle->currentIndex().row();
    if (row == -1) {
        QMessageBox::warning(this, "Sélection", "Veuillez sélectionner une poubelle dans le tableau pour générer le PDF.");
        return;
    }
    int id = ui->tab_poubelle->model()->data(ui->tab_poubelle->model()->index(row, 0)).toInt();
    tmp_poubelle.exporterPDF(id);
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
    chartViewContratType->setMaximumWidth(355);

    // Camembert
    chartViewContratTypePie = new QChartView();
    chartViewContratTypePie->setRenderHint(QPainter::Antialiasing);
    chartViewContratTypePie->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartViewContratTypePie->setMinimumHeight(160);
    chartViewContratTypePie->setMaximumHeight(295);
    chartViewContratTypePie->setMinimumWidth(0);
    chartViewContratType->setMaximumWidth(355);

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
    chartViewContratTypePie->setMaximumWidth(355);
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
    bool hasCustomId = !ui->ID_Contrat->text().trimmed().isEmpty();
    bool idOk = false;
    int newId = ui->ID_Contrat->text().trimmed().toInt(&idOk);
    if (hasCustomId && (!idOk || !Contrat::validerID(newId))) {
        QMessageBox::warning(this, "ID invalide", "Veuillez saisir un ID de contrat numérique valide (supérieur à 0).");
        return;
    }
    if (!hasCustomId) {
        QSqlQuery q;
        if (!q.exec("SELECT NVL(MAX(ID_CONTRAT), 0) + 1 FROM CONTRAT") || !q.next()) {
            QMessageBox::critical(this, "Erreur lors de l'ajout",
                                  "Impossible de générer automatiquement l'ID du contrat.\n\nDétails techniques :\n"
                                  + q.lastError().text());
            return;
        }
        newId = q.value(0).toInt();
        ui->ID_Contrat->setText(QString::number(newId));
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
    c.setId_contrat(newId);

    if (c.ajouter()) {
        QMessageBox::information(this, "Succès", "Le contrat a été ajouté avec succès !");
        ui->ID_Contrat->clear();
        ui->combo_ID_Client->setCurrentIndex(0);
        ui->combo_CIN_Employe->setCurrentIndex(0);
        ui->tab_contrat_2->setModel(Contrat().afficher());
        contrat_resetValidationStyles();
        contrat_refreshStats();
    } else {
        QString err = c.getLastError().trimmed();
        if (err.isEmpty()) err = "Erreur SQL inconnue (aucun détail retourné par le pilote).";
        QMessageBox::critical(this, "Erreur lors de l'ajout",
                              "L'opération a échoué.\n\nDétails techniques :\n" + err);
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
        QString err = c.getLastError().trimmed();
        if (err.isEmpty()) err = "Erreur SQL inconnue (aucun détail retourné par le pilote).";
        QMessageBox::critical(this, "Erreur lors de la modification",
                              "L'opération a échoué.\n\nDétails techniques :\n" + err);
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
// ===                MODULE EQUIPEMENT                  ===
// =========================================================

// ─── Remplir la combobox CIN depuis EMPLOYE ───────────────────────────────────
void MainWindow::equipement_chargerCIN()
{
    ui->Eq_CIN->blockSignals(true);
    ui->Eq_CIN->clear();
    ui->Eq_CIN->addItem("-", QVariant());   // index 0 → aucun employé assigné

    QSqlQuery q;
    q.prepare("SELECT CIN, NOM, PRENOM FROM EMPLOYE ORDER BY NOM, PRENOM");
    if (q.exec()) {
        while (q.next()) {
            QString cin    = q.value(0).toString();
            QString nom    = q.value(1).toString().trimmed();
            QString prenom = q.value(2).toString().trimmed();
            ui->Eq_CIN->addItem(QString("%1 %2 (%3)").arg(nom, prenom, cin), cin);
        }
    } else {
        qDebug() << "equipement_chargerCIN error:" << q.lastError().text();
    }

    ui->Eq_CIN->blockSignals(false);
}

// ─── Vider le formulaire ──────────────────────────────────────────────────────
void MainWindow::equipement_clearForm()
{
    ui->Eq_CIN->setCurrentIndex(0);
    ui->Eq_type->setCurrentIndex(0);
    ui->Eq_Fab->setCurrentIndex(0);
    ui->Eq_Statut->setCurrentIndex(0);
    ui->Eq_Statut_2->setCurrentIndex(0);  // Nouveauté → back to "-"

    // Re-enable date fields and restore defaults
    ui->Eq_Last_Maint_Date->setEnabled(true);
    ui->Eq_Next_Maint_Date->setEnabled(true);
    QDateTime now = QDateTime::currentDateTime();
    ui->Eq_Last_Maint_Date->setDateTime(now);
    ui->Eq_Next_Maint_Date->setDateTime(now);

    ui->Notes->clear();

    equipement_currentSelectedId = -1;
    equipement_resetValidationStyles();
}

// ─── Rafraîchir le tableau ────────────────────────────────────────────────────
void MainWindow::equipement_rafraichirTable()
{
    Equipement eq;
    QSqlQueryModel *m = eq.afficher();
    equipement_setupModelHeaders(m);
    ui->tab_employes_2->setModel(m);
    ui->tab_employes_2->resizeColumnsToContents();
    ui->tab_employes_2->horizontalHeader()->setStretchLastSection(true);
}

// ─── En-têtes du modèle ───────────────────────────────────────────────────────
void MainWindow::equipement_setupModelHeaders(QSqlQueryModel *m)
{
    m->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    m->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
    m->setHeaderData(2, Qt::Horizontal, QObject::tr("Employé"));
    m->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    m->setHeaderData(4, Qt::Horizontal, QObject::tr("Fabricant"));
    m->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    m->setHeaderData(6, Qt::Horizontal, QObject::tr("Dern. Maint."));
    m->setHeaderData(7, Qt::Horizontal, QObject::tr("Proch. Maint."));
    m->setHeaderData(8, Qt::Horizontal, QObject::tr("Notes"));
}

// ─── Style validation ─────────────────────────────────────────────────────────
// The dropdown-fix rule must be present in EVERY stylesheet applied to a QComboBox.
// Calling setStyleSheet() on a combobox replaces the entire stylesheet, which
// would strip the QAbstractItemView rule and make the popup list transparent.
static const QString EQ_DROPDOWN_FIX =
    "QComboBox QAbstractItemView {"
    "  background-color: white; color: #222222;"
    "  border: 1px solid #BDBDBD;"
    "  selection-background-color: #4CAF50; selection-color: white; outline: 0;"
    "}";

// Applies red/green border AND shows/hides the ✕ label beside the field.
void MainWindow::equipement_setWidgetStyle(QWidget *widget, QLabel *errLabel, bool isValid)
{
    QString base = isValid
        ? "QComboBox, QDateTimeEdit { border: 2px solid #43A047; border-radius: 5px;"
          " background-color: rgba(67,160,71,15); }"
        : "QComboBox, QDateTimeEdit { border: 2px solid #D32F2F; border-radius: 5px;"
          " background-color: rgba(211,47,47,15); }";

    widget->setStyleSheet(base + EQ_DROPDOWN_FIX);
    errLabel->setVisible(!isValid);
}

// Clears border AND hides ✕ — used when a field is back to its blank/default state.
void MainWindow::equipement_clearWidgetStyle(QWidget *widget, QLabel *errLabel)
{
    // Restore only the dropdown fix so the popup list stays readable.
    widget->setStyleSheet(EQ_DROPDOWN_FIX);
    errLabel->setVisible(false);
}

void MainWindow::equipement_resetValidationStyles()
{
    equipement_clearWidgetStyle(ui->Eq_CIN,             eq_errCIN);
    equipement_clearWidgetStyle(ui->Eq_type,            eq_errType);
    equipement_clearWidgetStyle(ui->Eq_Fab,             eq_errFab);
    equipement_clearWidgetStyle(ui->Eq_Statut,          eq_errStatut);
    equipement_clearWidgetStyle(ui->Eq_Statut_2,        eq_errNouveaute);
    equipement_clearWidgetStyle(ui->Eq_Next_Maint_Date, eq_errNextMaint);
}

// ─── Validation globale du formulaire (appelée au moment de soumettre) ────────
bool MainWindow::equipement_validerFormulaire()
{
    bool ok = true;

    // Type
    bool typeErr = (ui->Eq_type->currentIndex() == 0);
    equipement_setWidgetStyle(ui->Eq_type, eq_errType, !typeErr);
    if (typeErr) ok = false;

    // Fabricant
    bool fabErr = (ui->Eq_Fab->currentIndex() == 0);
    equipement_setWidgetStyle(ui->Eq_Fab, eq_errFab, !fabErr);
    if (fabErr) ok = false;

    // Statut
    bool statutErr = (ui->Eq_Statut->currentIndex() == 0);
    equipement_setWidgetStyle(ui->Eq_Statut, eq_errStatut, !statutErr);
    if (statutErr) ok = false;

    // Nouveauté — must be chosen
    bool nouveauteErr = (ui->Eq_Statut_2->currentIndex() == 0);
    equipement_setWidgetStyle(ui->Eq_Statut_2, eq_errNouveaute, !nouveauteErr);
    if (nouveauteErr) ok = false;

    // Cohérence CIN / Statut
    QString cinVal = ui->Eq_CIN->currentData().toString();
    QString statut = ui->Eq_Statut->currentText();
    if (!statutErr) {
        bool coherent = Equipement::validerCoherenceCinStatut(cinVal, statut);
        equipement_setWidgetStyle(ui->Eq_CIN,    eq_errCIN,    coherent);
        equipement_setWidgetStyle(ui->Eq_Statut, eq_errStatut, coherent);
        if (!coherent) ok = false;
    } else {
        equipement_clearWidgetStyle(ui->Eq_CIN, eq_errCIN);
    }

    // Date prochaine maintenance — only required for "Ancien"
    if (ui->Eq_Statut_2->currentText() == "Ancien") {
        bool dateOk = Equipement::validerDateSuivMaint(ui->Eq_Next_Maint_Date->dateTime());
        equipement_setWidgetStyle(ui->Eq_Next_Maint_Date, eq_errNextMaint, dateOk);
        if (!dateOk) ok = false;
    } else {
        equipement_clearWidgetStyle(ui->Eq_Next_Maint_Date, eq_errNextMaint);
    }

    return ok;
}

// ─── Validation en temps réel ─────────────────────────────────────────────────
// Rule: index 0 = blank/not chosen → reset to neutral (no colour, no ✕).
//       index > 0 = chosen          → show green (valid by definition for these combos).
void MainWindow::equipement_validateType()
{
    if (ui->Eq_type->currentIndex() == 0)
        equipement_clearWidgetStyle(ui->Eq_type, eq_errType);
    else
        equipement_setWidgetStyle(ui->Eq_type, eq_errType, true);
}

void MainWindow::equipement_validateFabricant()
{
    if (ui->Eq_Fab->currentIndex() == 0)
        equipement_clearWidgetStyle(ui->Eq_Fab, eq_errFab);
    else
        equipement_setWidgetStyle(ui->Eq_Fab, eq_errFab, true);
}

void MainWindow::equipement_validateStatut()
{
    if (ui->Eq_Statut->currentIndex() == 0)
        equipement_clearWidgetStyle(ui->Eq_Statut, eq_errStatut);
    else
        equipement_setWidgetStyle(ui->Eq_Statut, eq_errStatut, true);
}

void MainWindow::equipement_validateDateSuivMaint()
{
    bool ok = Equipement::validerDateSuivMaint(ui->Eq_Next_Maint_Date->dateTime());
    equipement_setWidgetStyle(ui->Eq_Next_Maint_Date, eq_errNextMaint, ok);
}

// ─── Suggestion automatique de statut quand le CIN change ────────────────────
void MainWindow::equipement_onCINChanged(int index)
{
    // Only auto-suggest if statut is still at blank position
    if (ui->Eq_Statut->currentIndex() == 0) {
        // blockSignals so the Statut change doesn't trigger onStatutChanged mid-update
        ui->Eq_Statut->blockSignals(true);
        ui->Eq_Statut->setCurrentText(index == 0 ? "Disponible" : "Non Disponible");
        ui->Eq_Statut->blockSignals(false);
    }
    // Re-run full validation so CIN+Statut coherence border updates immediately
    equipement_validerFormulaire();
}

void MainWindow::equipement_onStatutChanged(int /*index*/)
{
    equipement_validerFormulaire();
}

// ─── Nouveauté changed : enable/disable date fields accordingly ───────────────
void MainWindow::equipement_onNouveauteChanged(int index)
{
    QString choix = ui->Eq_Statut_2->currentText();

    if (choix == "Vierge") {
        // Brand new — last maintenance is irrelevant, next is auto-set to +1 month
        ui->Eq_Last_Maint_Date->setEnabled(false);
        ui->Eq_Last_Maint_Date->setDateTime(QDateTime());   // clear / empty
        ui->Eq_Next_Maint_Date->setEnabled(false);
        ui->Eq_Next_Maint_Date->setDateTime(
            QDateTime::currentDateTime().addMonths(1));
        // Date field is auto-managed — clear any error style
        equipement_clearWidgetStyle(ui->Eq_Next_Maint_Date, eq_errNextMaint);

    } else if (choix == "Ancien") {
        // Existing equipment — both dates are required and editable
        ui->Eq_Last_Maint_Date->setEnabled(true);
        ui->Eq_Next_Maint_Date->setEnabled(true);
        // Re-validate the next-maintenance date immediately
        equipement_validateDateSuivMaint();

    } else {
        // Back to "-" — restore defaults and clear styles
        ui->Eq_Last_Maint_Date->setEnabled(true);
        ui->Eq_Next_Maint_Date->setEnabled(true);
        QDateTime now = QDateTime::currentDateTime();
        ui->Eq_Last_Maint_Date->setDateTime(now);
        ui->Eq_Next_Maint_Date->setDateTime(now);
        equipement_clearWidgetStyle(ui->Eq_Next_Maint_Date, eq_errNextMaint);
    }

    // Always re-validate the Nouveauté field itself
    if (index == 0)
        equipement_clearWidgetStyle(ui->Eq_Statut_2, eq_errNouveaute);
    else
        equipement_setWidgetStyle(ui->Eq_Statut_2, eq_errNouveaute, true);
}

// ─── Clic sur une ligne du tableau ───────────────────────────────────────────
void MainWindow::equipement_onTabClicked(const QModelIndex &index)
{
    if (!index.isValid()) return;

    int row = index.row();
    auto *m = ui->tab_employes_2->model();

    // Colonne 0 → EQUIPMENT_ID
    equipement_currentSelectedId = m->data(m->index(row, 0)).toInt();

    // Récupérer le détail complet depuis la DB
    QSqlQuery q;
    q.prepare(
        "SELECT CIN, EQUIPMENT_TYPE, FABRICANT, STATUT, "
        "       DATE_DER_MAINT, DATE_SUIV_MAINT, NOTES "
        "FROM EQUIPMENT WHERE EQUIPMENT_ID = :id"
    );
    q.bindValue(":id", equipement_currentSelectedId);

    if (!q.exec() || !q.next()) {
        qDebug() << "equipement_onTabClicked error:" << q.lastError().text();
        return;
    }

    // CIN : retrouver l'entrée combo dont la userData correspond
    QString cinDB = q.value(0).toString();
    int cinIdx = 0;
    for (int i = 1; i < ui->Eq_CIN->count(); ++i) {
        if (ui->Eq_CIN->itemData(i).toString() == cinDB) { cinIdx = i; break; }
    }
    ui->Eq_CIN->blockSignals(true);
    ui->Eq_CIN->setCurrentIndex(cinIdx);
    ui->Eq_CIN->blockSignals(false);

    ui->Eq_type->setCurrentText(q.value(1).toString().trimmed());
    ui->Eq_Fab->setCurrentText(q.value(2).toString().trimmed());
    ui->Eq_Statut->setCurrentText(q.value(3).toString().trimmed());

    QDateTime lastMaint = q.value(4).toDateTime();
    QDateTime nextMaint = q.value(5).toDateTime();
    if (lastMaint.isValid()) ui->Eq_Last_Maint_Date->setDateTime(lastMaint);
    if (nextMaint.isValid()) ui->Eq_Next_Maint_Date->setDateTime(nextMaint);

    ui->Notes->setPlainText(q.value(6).toString());

    equipement_resetValidationStyles();
}

// ─── Recherche en temps réel ──────────────────────────────────────────────────
void MainWindow::equipement_onRechercheTextChanged(const QString &arg1)
{
    Equipement eq;
    QSqlQueryModel *m = eq.rechercher(arg1);
    equipement_setupModelHeaders(m);
    ui->tab_employes_2->setModel(m);
    ui->tab_employes_2->resizeColumnsToContents();
    ui->tab_employes_2->horizontalHeader()->setStretchLastSection(true);
}

// ─── Tri ──────────────────────────────────────────────────────────────────────
void MainWindow::equipement_onTriClicked()
{
    QString champ = ui->comboBox_2->currentText();
    if      (champ == "Fabricant") equipement_sortColumn = "EQ.FABRICANT";
    else if (champ == "Statut")    equipement_sortColumn = "EQ.STATUT";
    else                           equipement_sortColumn = "EQ.EQUIPMENT_TYPE";

    equipement_sortAscending = !equipement_sortAscending;
    ui->btn_tri_3->setText(equipement_sortAscending ? "Trier ↑" : "Trier ↓");

    QString direction = equipement_sortAscending ? "ASC" : "DESC";
    QString text      = ui->le_recherche_2->text().trimmed();

    QString whereClause;
    if (!text.isEmpty()) {
        whereClause = QString(
            "WHERE UPPER(EQ.FABRICANT)      LIKE UPPER('%%%1%') "
            "OR    UPPER(EQ.EQUIPMENT_TYPE) LIKE UPPER('%%%1%') "
            "OR    UPPER(EQ.STATUT)         LIKE UPPER('%%%1%') "
            "OR    UPPER(E.NOM)             LIKE UPPER('%%%1%') "
            "OR    UPPER(E.PRENOM)          LIKE UPPER('%%%1%')"
        ).arg(text);
    }

    QSqlQueryModel *m = new QSqlQueryModel();
    m->setQuery(QString(
        "SELECT EQ.EQUIPMENT_ID, EQ.CIN, "
        "       E.NOM || ' ' || E.PRENOM AS EMPLOYE, "
        "       EQ.EQUIPMENT_TYPE, EQ.FABRICANT, EQ.STATUT, "
        "       EQ.DATE_DER_MAINT, EQ.DATE_SUIV_MAINT, EQ.NOTES "
        "FROM   EQUIPMENT EQ "
        "LEFT JOIN EMPLOYE E ON EQ.CIN = E.CIN "
        "%1 "
        "ORDER BY %2 %3"
    ).arg(whereClause, equipement_sortColumn, direction));

    equipement_setupModelHeaders(m);
    ui->tab_employes_2->setModel(m);
    ui->tab_employes_2->resizeColumnsToContents();
    ui->tab_employes_2->horizontalHeader()->setStretchLastSection(true);
}

// ─── Ajouter un équipement ────────────────────────────────────────────────────
void MainWindow::equipement_onAjouterClicked()
{
    if (!equipement_validerFormulaire()) {
        QMessageBox::warning(this, "Saisie invalide",
                             "Veuillez corriger les champs en rouge avant de continuer.");
        return;
    }

    QString cinVal    = ui->Eq_CIN->currentData().toString();
    bool    isVierge  = (ui->Eq_Statut_2->currentText() == "Vierge");
    QDateTime lastDT  = isVierge ? QDateTime() : ui->Eq_Last_Maint_Date->dateTime();

    Equipement eq(cinVal,
                  ui->Eq_type->currentText(),
                  ui->Eq_Fab->currentText(),
                  ui->Eq_Statut->currentText(),
                  lastDT,
                  ui->Eq_Next_Maint_Date->dateTime(),
                  ui->Notes->toPlainText());

    if (eq.ajouter()) {
        QMessageBox::information(this, "Succès", "Équipement ajouté avec succès !");
        equipement_clearForm();
        equipement_rafraichirTable();
    } else {
        QString err = eq.getLastError().trimmed();
        if (err.isEmpty()) err = "Erreur SQL inconnue (aucun détail retourné par le pilote).";
        QMessageBox::critical(this, "Erreur DB",
                              "L'ajout de l'équipement a échoué.\n\nDétails techniques :\n" + err);
    }
}

// ─── Modifier un équipement ───────────────────────────────────────────────────
void MainWindow::equipement_onModifierClicked()
{
    if (equipement_currentSelectedId == -1) {
        QMessageBox::warning(this, "Aucune sélection",
                             "Veuillez sélectionner un équipement dans le tableau.");
        return;
    }

    if (!equipement_validerFormulaire()) {
        QMessageBox::warning(this, "Saisie invalide",
                             "Veuillez corriger les champs en rouge avant de continuer.");
        return;
    }

    QString cinVal   = ui->Eq_CIN->currentData().toString();
    bool    isVierge = (ui->Eq_Statut_2->currentText() == "Vierge");
    QDateTime lastDT = isVierge ? QDateTime() : ui->Eq_Last_Maint_Date->dateTime();

    Equipement eq(cinVal,
                  ui->Eq_type->currentText(),
                  ui->Eq_Fab->currentText(),
                  ui->Eq_Statut->currentText(),
                  lastDT,
                  ui->Eq_Next_Maint_Date->dateTime(),
                  ui->Notes->toPlainText());

    if (eq.modifier(equipement_currentSelectedId)) {
        QMessageBox::information(this, "Succès", "Équipement modifié avec succès !");
        equipement_clearForm();
        equipement_rafraichirTable();
    } else {
        QString err = eq.getLastError().trimmed();
        if (err.isEmpty()) err = "Erreur SQL inconnue (aucun détail retourné par le pilote).";
        QMessageBox::critical(this, "Erreur DB",
                              "La modification a échoué.\n\nDétails techniques :\n" + err);
    }
}

// ─── Supprimer un équipement ──────────────────────────────────────────────────
void MainWindow::equipement_onSupprimerClicked()
{
    if (equipement_currentSelectedId == -1) {
        QMessageBox::warning(this, "Aucune sélection",
                             "Veuillez sélectionner un équipement à supprimer.");
        return;
    }

    if (QMessageBox::question(this, "Confirmer suppression",
            QString("Supprimer l'équipement ID %1 ?").arg(equipement_currentSelectedId),
            QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) return;

    Equipement eq;
    if (eq.supprimer(equipement_currentSelectedId)) {
        QMessageBox::information(this, "Succès", "Équipement supprimé avec succès !");
        equipement_clearForm();
        equipement_rafraichirTable();
    } else {
        QMessageBox::critical(this, "Erreur DB", "La suppression a échoué.");
    }
}

// ─── Exporter PDF ─────────────────────────────────────────────────────────────
void MainWindow::equipement_onExporterClicked()
{
    QString filePath = QFileDialog::getSaveFileName(this,
        "Exporter la liste des équipements", "Liste_Equipements.pdf", "PDF (*.pdf)");
    if (filePath.isEmpty()) return;

    Equipement eq;
    QSqlQueryModel *m = eq.afficher();
    if (!m) {
        QMessageBox::critical(this, "Erreur",
            "Impossible de charger la liste des équipements pour l'export PDF.");
        return;
    }

    bool ok = eq.exporterPdf(filePath, m);
    delete m;

    if (ok) {
        QMessageBox::information(this, "Succès",
            "La liste des équipements a été exportée en PDF avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Échec de l'export PDF.");
    }
}
void MainWindow::equipement_setupStatsUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->gb_stats_equipement);
    mainLayout->setSpacing(8);
    mainLayout->setContentsMargins(8, 24, 8, 8);

    // Bar chart – Répartition par type
    chartViewEquipType = new QChartView();
    chartViewEquipType->setRenderHint(QPainter::Antialiasing);
    chartViewEquipType->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartViewEquipType->setMinimumHeight(140);
    chartViewEquipType->setMaximumWidth(300);

    // Pie/donut – Répartition par statut
    chartViewEquipStatut = new QChartView();
    chartViewEquipStatut->setRenderHint(QPainter::Antialiasing);
    chartViewEquipStatut->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartViewEquipStatut->setMinimumHeight(140);
    chartViewEquipType->setMaximumWidth(300);

    mainLayout->addWidget(chartViewEquipType,    1);
    mainLayout->addWidget(chartViewEquipStatut,  1);

    ui->gb_stats_equipement->setLayout(mainLayout);
}
void MainWindow::equipement_refreshStats()
{
    QSqlQuery q;

    // ── 1. Histogramme : Nombre d'équipements par type ───────────────────────
    QBarSeries *barSeries = new QBarSeries();
    barSeries->setBarWidth(0.55);
    barSeries->setLabelsVisible(true);
    barSeries->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    barSeries->setLabelsFormat("@value");

    // Fixed palette – one colour per bar set
    QList<QColor> barColors = {
        QColor("#1b5e20"), QColor("#e53935"), QColor("#1565c0"),
        QColor("#f57f17"), QColor("#6a1b9a"), QColor("#00695c")
    };

    QStringList typeCategories;
    int maxBarVal = 1;
    int colorIdx  = 0;

    if (q.exec("SELECT EQUIPMENT_TYPE, COUNT(*) FROM EQUIPMENT "
               "GROUP BY EQUIPMENT_TYPE ORDER BY COUNT(*) DESC"))
    {
        while (q.next()) {
            QString typeName = q.value(0).toString().trimmed();
            int     cnt      = q.value(1).toInt();
            if (typeName.isEmpty()) typeName = "Inconnu";

            QBarSet *bs = new QBarSet(typeName);
            bs->setColor(barColors[colorIdx % barColors.size()]);
            bs->setBorderColor(bs->color().darker(130));
            bs->setLabelColor(Qt::black);
            *bs << cnt;

            barSeries->append(bs);
            typeCategories << typeName;
            if (cnt > maxBarVal) maxBarVal = cnt;
            ++colorIdx;
        }
    }

    QChart *chartType = new QChart();
    chartType->setTitle("Équipements par Type");
    chartType->setTitleFont(QFont("Arial", 9, QFont::Bold));
    chartType->setAnimationOptions(QChart::SeriesAnimations);
    chartType->setMargins(QMargins(4, 4, 8, 0));
    chartType->setBackgroundRoundness(0);
    chartType->legend()->setVisible(true);
    chartType->legend()->setAlignment(Qt::AlignBottom);
    chartType->legend()->setFont(QFont("Arial", 7));
    chartType->addSeries(barSeries);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(QStringList{""});    // single grouped category
    axisX->setLabelsFont(QFont("Arial", 7));
    axisX->setGridLineVisible(false);
    chartType->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setMin(0);
    axisY->setMax(maxBarVal + 2);
    axisY->setLabelFormat("%i");
    axisY->setLabelsFont(QFont("Arial", 7));
    axisY->setTickCount(qMin(maxBarVal + 3, 7));
    chartType->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    chartViewEquipType->setChart(chartType);

    // ── 2. Donut : Répartition par statut ────────────────────────────────────
    QChart *chartPie = new QChart();
    chartPie->setTitle("Disponibilité des équipements");
    chartPie->setTitleFont(QFont("Arial", 8, QFont::Bold));
    chartPie->setMargins(QMargins(0, 4, 0, 0));
    chartPie->setBackgroundRoundness(0);
    chartPie->setAnimationOptions(QChart::SeriesAnimations);

    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setPieStartAngle(30);
    pieSeries->setHoleSize(0.3);
    pieSeries->setPieSize(0.62);

    QList<QColor> pieColors = {
        QColor("#43a047"), QColor("#e53935"),
        QColor("#fb8c00"), QColor("#1565c0"),
        QColor("#6a1b9a")
    };
    int ci = 0;

    if (q.exec("SELECT STATUT, COUNT(*) FROM EQUIPMENT GROUP BY STATUT"))
    {
        while (q.next()) {
            QString statut = q.value(0).toString().trimmed();
            int     count  = q.value(1).toInt();
            if (statut.isEmpty()) statut = "Indéfini";

            QPieSlice *slice = pieSeries->append(
                statut + " (" + QString::number(count) + ")", count);
            slice->setLabelVisible(true);
            slice->setLabelFont(QFont("Arial", 7));
            slice->setColor(pieColors[ci % pieColors.size()]);
            ++ci;
        }
    }

    chartPie->addSeries(pieSeries);
    chartPie->legend()->setAlignment(Qt::AlignRight);
    chartPie->legend()->setFont(QFont("Arial", 7));

    chartViewEquipStatut->setChart(chartPie);
}

// =========================================================
// ===                  MODULE EMPLOYE                   ===
// =========================================================

void MainWindow::employe_clearForm()
{
    ui->le_cin->clear();
    ui->le_nom->clear();
    ui->le_prenom->clear();
    ui->cb_poste->setCurrentIndex(0);
    ui->le_salaire->clear();
    ui->de_embauche->setDate(QDate::currentDate());
    ui->le_badge->clear();
    ui->le_age->clear();
    ui->cb_genre->setCurrentIndex(0);
}

void MainWindow::employe_onAjouterClicked()
{
    QString cin      = ui->le_cin->text();
    QString nom      = ui->le_nom->text();
    QString prenom   = ui->le_prenom->text();
    QString poste    = ui->cb_poste->currentText();
    double  salaire  = ui->le_salaire->text().replace(",", ".").toDouble();
    QDate   dateEmb  = ui->de_embauche->date();
    QString idBadge  = ui->le_badge->text();
    int     age      = ui->le_age->text().toInt();
    QString genre    = ui->cb_genre->currentText();

    QString errorMessage;
    if (!Employe::validerFormulaire(cin, nom, prenom, idBadge, salaire, errorMessage)) {
        QMessageBox::warning(this, "Erreur de saisie", errorMessage);
        return;
    }

    if (tmp_employe.ajouter(cin, nom, prenom, poste, salaire, dateEmb, idBadge, age, genre)) {
        QMessageBox::information(this, "Succès", "Employé ajouté avec succès.");
        ui->tab_employes->setModel(tmp_employe.afficher());
        employe_clearForm();
        employe_refreshStats();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de l'ajout.");
    }
}

void MainWindow::employe_onModifierClicked()
{
    QString cin      = ui->le_cin->text();
    QString nom      = ui->le_nom->text();
    QString prenom   = ui->le_prenom->text();
    QString poste    = ui->cb_poste->currentText();
    double  salaire  = ui->le_salaire->text().replace(",", ".").toDouble();
    QDate   dateEmb  = ui->de_embauche->date();
    QString idBadge  = ui->le_badge->text();
    int     age      = ui->le_age->text().toInt();
    QString genre    = ui->cb_genre->currentText();

    QString errorMessage;
    if (!Employe::validerFormulaire(cin, nom, prenom, idBadge, salaire, errorMessage)) {
        QMessageBox::warning(this, "Erreur de saisie", errorMessage);
        return;
    }

    if (tmp_employe.modifier(cin, nom, prenom, poste, salaire, dateEmb, idBadge, age, genre)) {
        QMessageBox::information(this, "Succès", "Employé modifié avec succès.");
        ui->tab_employes->setModel(tmp_employe.afficher());
        employe_clearForm();
        employe_refreshStats();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la modification.");
    }
}

void MainWindow::employe_onSupprimerClicked()
{
    QString cin = ui->le_cin->text();

    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Avertissement", "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    QString erreurInfo;
    if (QMessageBox::question(this, "Confirmation", "Voulez-vous vraiment supprimer cet employé ?", QMessageBox::Yes|QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    if (tmp_employe.supprimer(cin, erreurInfo)) {
        QMessageBox::information(this, "Succès", "Employé supprimé avec succès.");
        employe_clearForm();
        ui->tab_employes->setModel(tmp_employe.afficher());
        employe_refreshStats();
    } else {
        QMessageBox::critical(this, "Erreur", "Erreur lors de la suppression.\n" + erreurInfo);
    }
}

void MainWindow::employe_onTabClicked(const QModelIndex &index)
{
    int row = index.row();
    QAbstractItemModel *model = ui->tab_employes->model();
    
    QVariant cinData = model->data(model->index(row, 0));
    bool ok = false;
    double cinDouble = cinData.toDouble(&ok);
    QString cinStr = ok ? QString::number(static_cast<qulonglong>(cinDouble)).rightJustified(8, '0') : cinData.toString();
    
    ui->le_cin->setText(cinStr);
    ui->le_nom->setText(model->data(model->index(row, 1)).toString());
    ui->le_prenom->setText(model->data(model->index(row, 2)).toString());
    ui->cb_poste->setCurrentText(model->data(model->index(row, 3)).toString());
    ui->le_salaire->setText(model->data(model->index(row, 4)).toString());
    ui->de_embauche->setDate(model->data(model->index(row, 5)).toDate());
    ui->le_badge->setText(model->data(model->index(row, 6)).toString());
    ui->le_age->setText(model->data(model->index(row, 7)).toString());
    ui->cb_genre->setCurrentText(model->data(model->index(row, 8)).toString());
}

void MainWindow::employe_onRechercheTextChanged(const QString &arg1)
{
    ui->tab_employes->setModel(tmp_employe.rechercher(arg1));
}

void MainWindow::employe_onTriClicked()
{
    QString currentSort = ui->comboBox->currentText();
    ui->tab_employes->setModel(tmp_employe.trier(currentSort));
}

void MainWindow::employe_onPdfClicked()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Exporter la liste", "Liste_Employes.pdf", "PDF (*.pdf)");
    if (filePath.isEmpty()) return;

    QSqlTableModel* m = tmp_employe.afficher();
    if (tmp_employe.exporterPDF(filePath, m)) {
        QMessageBox::information(this, "Succès", "La liste des employés a été exportée en PDF avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur", "Echec de l'export PDF.");
    }
    delete m;
}

void MainWindow::employe_onRefreshClicked()
{
    ui->tab_employes->setModel(tmp_employe.afficher());
    employe_refreshStats();
}

void MainWindow::employe_setupStatsUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->gb_stats);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    QHBoxLayout *pieLayout = new QHBoxLayout();

    employe_chartViewGenre = new QChartView();
    employe_chartViewGenre->setRenderHint(QPainter::Antialiasing);
    employe_chartViewGenre->setMinimumHeight(300);

    employe_chartViewPoste = new QChartView();
    employe_chartViewPoste->setRenderHint(QPainter::Antialiasing);
    employe_chartViewPoste->setMinimumHeight(300);

    pieLayout->addWidget(employe_chartViewGenre);
    pieLayout->addWidget(employe_chartViewPoste);
    mainLayout->addLayout(pieLayout);

    employe_chartViewSalaire = new QChartView();
    employe_chartViewSalaire->setRenderHint(QPainter::Antialiasing);
    employe_chartViewSalaire->setMinimumHeight(350);
    mainLayout->addWidget(employe_chartViewSalaire);

    ui->gb_stats->setLayout(mainLayout);
}

void MainWindow::employe_refreshStats()
{
    QChart *cGenre = tmp_employe.createGenreChart();
    QChart *cPoste = tmp_employe.createPosteChart();
    QChart *cSalaire = tmp_employe.createSalaireChart();
    
    QChart *oldCGenre = employe_chartViewGenre->chart();
    QChart *oldCPoste = employe_chartViewPoste->chart();
    QChart *oldCSalaire = employe_chartViewSalaire->chart();

    employe_chartViewGenre->setChart(cGenre);
    employe_chartViewPoste->setChart(cPoste);
    employe_chartViewSalaire->setChart(cSalaire);
    
    // Memory cleanup
    if(oldCGenre) delete oldCGenre;
    if(oldCPoste) delete oldCPoste;
    if(oldCSalaire) delete oldCSalaire;
}

void MainWindow::employe_onGenererContratClicked()
{
    // 1. Vérifier si un employé a été sélectionné dans le tableau 
    // (s'il est sélectionné, le champ CIN est rempli)
    QString cin = ui->le_cin->text();
    if (cin.isEmpty()) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez d'abord sélectionner un employé dans le tableau pour générer son contrat.");
        return;
    }

    // 2. Récupérer les données de l'employé depuis les champs existants
    QString nom     = ui->le_nom->text();
    QString prenom  = ui->le_prenom->text();
    QString poste   = ui->cb_poste->currentText();
    double  salaire = ui->le_salaire->text().replace(",", ".").toDouble();
    QDate   dateEmb = ui->de_embauche->date();

    // 3. Récupérer les données spécifiques au contrat depuis la groupBox gb_contrat
    QString typeContrat = ui->cb_type_contrat_emp->currentText();
    int periodeEssai    = ui->sb_periode_essai_emp->value();
    QString lieuTravail = ui->le_lieu_travail_emp->text();

    if (lieuTravail.isEmpty()) {
        QMessageBox::warning(this, "Champ vide", "Veuillez préciser le lieu de travail.");
        return;
    }

    // 4. Demander où enregistrer le PDF
    QString nomFichierDefaut = QString("Contrat_%1_%2_%3.pdf").arg(typeContrat, nom, prenom);
    QString filePath = QFileDialog::getSaveFileName(this, "Enregistrer le contrat", nomFichierDefaut, "PDF (*.pdf)");
    
    if (filePath.isEmpty()) return; // L'utilisateur a annulé

    // 5. Générer le document
    if (tmp_employe.genererContratPDF(filePath, cin, nom, prenom, poste, salaire, dateEmb, typeContrat, periodeEssai, lieuTravail)) {
        QMessageBox::information(this, "Succès", "Le contrat de travail a été généré et sauvegardé avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "Une erreur est survenue lors de la création du PDF.");
    }
}

// =========================================================
// ===                  MODULE CLIENT                    ===
// =========================================================

void MainWindow::rafraichirAffichage()
{
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

bool MainWindow::verifierSaisie()
{
    QString id = ui->le_id_client->text();
    QString nom = ui->le_nom_client->text();
    QString tel = ui->le_tel_client->text();
    QString adr = ui->le_adresse_client->text();
    QString responsable = ui->le_responsable_client->text();
    QString cp = ui->dsb_codepostal_client->text();

    if (id.isEmpty() || nom.isEmpty() || tel.isEmpty() || adr.isEmpty() || responsable.isEmpty() || cp.isEmpty()) {
        QMessageBox::warning(this, "Champs vides", "Veuillez remplir tous les champs du formulaire.");
        return false;
    }

    QRegularExpression rxNom("^[A-Za-zÀ-ÿ\\s]+$");
    if (!rxNom.match(nom).hasMatch()) {
        QMessageBox::warning(this, "Erreur Nom", "Le nom ne doit contenir que des lettres.");
        return false;
    }

    QRegularExpression rxTel("^[0-9]{8}$");
    if (!rxTel.match(tel).hasMatch()) {
        QMessageBox::warning(this, "Erreur Téléphone", "Le numéro de téléphone doit contenir 8 chiffres.");
        return false;
    }

    if (id.toInt() <= 0) {
        QMessageBox::warning(this, "Erreur ID", "L'ID doit être un nombre positif.");
        return false;
    }

    QRegularExpression rxCP("^[0-9]+$");
    if (!rxCP.match(cp).hasMatch()) {
        QMessageBox::warning(this, "Erreur Code Postal", "Le code postal doit être numérique.");
        return false;
    }

    return true;
}

bool MainWindow::reaffecterIdClientDansRelations(int oldId, int newId)
{
    QSqlQuery q(QSqlDatabase::database());

    q.prepare("UPDATE CONTRAT SET ID_CLIENT = :newId WHERE ID_CLIENT = :oldId");
    q.bindValue(":newId", newId);
    q.bindValue(":oldId", oldId);
    if (!q.exec()) {
        QMessageBox::critical(this, "Erreur de base de données", "Échec de la mise à jour des contrats :\n" + q.lastError().text());
        return false;
    }

    q.prepare("UPDATE PRODUIT SET ID_CLIENT = :newId WHERE ID_CLIENT = :oldId");
    q.bindValue(":newId", newId);
    q.bindValue(":oldId", oldId);
    if (!q.exec()) {
        QMessageBox::critical(this, "Erreur de base de données", "Échec de la mise à jour des produits :\n" + q.lastError().text());
        return false;
    }

    return true;
}

void MainWindow::onTableClicked(const QModelIndex &index)
{
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

void MainWindow::onRechercheTextChanged(const QString &text)
{
    if (text.isEmpty()) {
        currentFilter.clear();
    } else {
        currentFilter = QString("UPPER(NOM_CLIENT) LIKE UPPER('%%1%')").arg(text);
    }
    rafraichirAffichage();
}

void MainWindow::onBtnAjouterClicked() {
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

void MainWindow::onBtnModifierClicked() {
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

    if (newId == selectedClientId) {
        query.prepare("UPDATE CLIENT SET NOM_CLIENT=:nom, VILLE=:ville, CODE_POSTAL=:cp, NUM_TEL=:tel, PDG=:pdg, ADRESSE=:adr WHERE ID_CLIENT=:id");
        query.bindValue(":id", selectedClientId);
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
    } else {
        // Oracle does not support ON UPDATE CASCADE; migrate to a new row then move child records.
        query.prepare("SELECT COUNT(*) FROM CLIENT WHERE ID_CLIENT = :id");
        query.bindValue(":id", newId);
        if (!query.exec() || !query.next()) {
            db.rollback();
            QMessageBox::critical(nullptr, "Erreur de base de données",
                                  "Impossible de vérifier le nouvel ID client :\n" + query.lastError().text());
            return;
        }
        if (query.value(0).toInt() > 0) {
            db.rollback();
            QMessageBox::warning(nullptr, "ID déjà utilisé",
                                 "Ce nouvel ID client existe déjà. Veuillez choisir un autre ID.");
            return;
        }

        int pointsFidelite = 0;
        query.prepare("SELECT POINT_DE_FIDELITE FROM CLIENT WHERE ID_CLIENT = :oldId");
        query.bindValue(":oldId", selectedClientId);
        if (!query.exec() || !query.next()) {
            db.rollback();
            QMessageBox::critical(nullptr, "Erreur de base de données",
                                  "Impossible de lire le client actuel :\n" + query.lastError().text());
            return;
        }
        pointsFidelite = query.value(0).toInt();

        query.prepare("INSERT INTO CLIENT (ID_CLIENT, NOM_CLIENT, VILLE, CODE_POSTAL, NUM_TEL, PDG, ADRESSE, POINT_DE_FIDELITE) "
                      "VALUES (:id, :nom, :ville, :cp, :tel, :pdg, :adr, :points)");
        query.bindValue(":id", newId);
        query.bindValue(":nom", ui->le_nom_client->text());
        query.bindValue(":ville", ui->cb_ville_client->currentText());
        query.bindValue(":cp", ui->dsb_codepostal_client->text());
        query.bindValue(":tel", ui->le_tel_client->text());
        query.bindValue(":pdg", ui->le_responsable_client->text());
        query.bindValue(":adr", ui->le_adresse_client->text());
        query.bindValue(":points", pointsFidelite);

        if (!query.exec()) {
            db.rollback();
            QMessageBox::critical(nullptr, "Erreur de base de données",
                                  "Impossible de créer le client avec le nouvel ID :\n" + query.lastError().text());
            return;
        }

        if (!reaffecterIdClientDansRelations(selectedClientId, newId)) {
            db.rollback();
            return;
        }

        query.prepare("DELETE FROM CLIENT WHERE ID_CLIENT = :oldId");
        query.bindValue(":oldId", selectedClientId);
        if (!query.exec()) {
            db.rollback();
            QMessageBox::critical(nullptr, "Erreur de base de données",
                                  "Impossible de supprimer l'ancien client :\n" + query.lastError().text());
            return;
        }
    }

    if (!db.commit()) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible de valider la transaction :\n" + db.lastError().text());
        return;
    }

    selectedClientId = newId;
    QMessageBox::information(nullptr, "Succès", "Client modifié avec succès.");
    rafraichirAffichage();
}

void MainWindow::onBtnSupprimerClicked() {
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
