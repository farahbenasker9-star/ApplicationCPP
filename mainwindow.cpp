#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>
#include <QFileDialog>
#include <QPrinter>
#include <QTextDocument>
#include <QDateTime>
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

    // ==================== CONTRAT ====================
    contrat = new ContratManager(ui, this);
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
        contrat->populateComboBoxes();
        ui->tab_contrat_2->setModel(Contrat().afficher());
        contrat->refreshStats();
    }
    ui->stackedWidget->setCurrentIndex(pageIndex);
    if (pageIndex == 4) {
        Produit P;
        ui->tab_stock->setModel(P.afficher());
    }
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
    Q_UNUSED(index)
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
