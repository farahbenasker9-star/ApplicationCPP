#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>

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
}

MainWindow::~MainWindow()
{
    delete ui;
}

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

