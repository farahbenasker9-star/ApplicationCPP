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
    on_cb_statut_currentIndexChanged(ui->cb_statut->currentIndex());
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
    ui->stackedWidget->setCurrentIndex(pageIndex);
    if (pageIndex == 2) { // Page Contrat
        contrat->populateComboBoxes();
        ui->tab_contrat_2->setModel(Contrat().afficher());
        contrat->refreshStats();
    }

    if (pageIndex == 4) {
        Produit P;
        ui->tab_produit->setModel(P.afficher());
        chargerIdsClients(); // <--- Mettre à jour la liste des clients ici
    }
}

// =========================================================
// ===                  MODULE PRODUIT                   ===
// =========================================================

void MainWindow::chargerIdsClients() {
    ui->cb_id_client->clear(); // On vide la liste actuelle
    ui->cb_id_client->addItem(""); // Optionnel : un champ vide par défaut

    QSqlQuery query("SELECT ID_CLIENT FROM CLIENT ORDER BY ID_CLIENT ASC");
    while (query.next()) {
        ui->cb_id_client->addItem(query.value(0).toString());
    }
}
void MainWindow::on_btn_ajouter_produit_clicked() {
    if (!controleSaisieProduit()) return; // On s'arrête si c'est faux
    // Récupération des données
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
        ui->tab_produit->setModel(P.afficher()); // Refresh du tableau
        QMessageBox::information(this, "Succès", "Produit ajouté !");
        viderFormulaire();
    } else {
        QMessageBox::critical(this, "Erreur", "Ajout échoué.");
    }
}
void MainWindow::on_btn_supprimer_produit_clicked() {
    int id = ui->le_id->text().toInt(); // On récupère l'ID à supprimer
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

    // Récupération sécurisée de l'ID Client
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

        // Mettre à jour l'ID de référence pour une future modification sans re-cliquer
        id_a_modifier = ui->le_id->text().toInt();
        viderFormulaire();
    } else {
        QMessageBox::critical(this, "Erreur", "La modification a échoué. Vérifiez que le nouvel ID n'est pas déjà utilisé par un autre produit.");
    }
}
void MainWindow::on_cb_statut_currentIndexChanged(int index)
{
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
    else { // Cas "Disponible"
        ui->cb_id_client->setEnabled(false);
        ui->de_date_v->setEnabled(false);

        // IMPORTANT : Ne pas faire .clear() sur une ComboBox sinon elle se vide !
        ui->cb_id_client->setCurrentIndex(-1); // Déselectionne l'élément sans vider la liste
    }
}

void MainWindow::on_tab_produit_clicked(const QModelIndex &index)
{
    int row = index.row();
    auto model = ui->tab_produit->model();
    // ON MÉMORISE L'ID ACTUEL AVANT TOUTE MODIFICATION
    this->id_a_modifier = model->index(row, 0).data().toInt();

    QString statut_db = model->index(row, 6).data().toString().trimmed();
    ui->cb_statut->setCurrentText(statut_db);

    on_cb_statut_currentIndexChanged(ui->cb_statut->currentIndex());

    // Pour la ComboBox client : on cherche l'index correspondant à l'ID du tableau
    QString id_client_db = model->index(row, 1).data().toString().trimmed();
    int idx_client = ui->cb_id_client->findText(id_client_db);
    if (idx_client != -1) {
        ui->cb_id_client->setCurrentIndex(idx_client);
    } else {
        ui->cb_id_client->setCurrentIndex(0); // Vide si non trouvé
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
    QString id_c = ui->cb_id_client->currentText(); // On utilise la ComboBox maintenant

    // 1. Vérification de l'ID (8 chiffres)
    if (id_p.length() != 8) {
        QMessageBox::warning(this, "Erreur de saisie", "L'ID Produit doit contenir exactement 8 chiffres.");
        return false;
    }

    // 2. Vérification du poids et du prix
    if (poids <= 0 || prix <= 0) {
        QMessageBox::warning(this, "Erreur de saisie", "Le poids et le prix doivent être supérieurs à 0.");
        return false;
    }

    // ==================== NOUVEAU CONTROLE DATE DE CREATION ====================
    QDate dateCreation = ui->de_date_c->date();
    QDate aujourdhui = QDate::currentDate();
    QDate limitePassee = aujourdhui.addMonths(-1);

    // Vérification : Pas plus d'un mois dans le passé
    if (dateCreation < limitePassee) {
        QMessageBox::warning(this, "Erreur de date",
                             "La date de création est trop ancienne. Elle doit être comprise entre le "
                                 + limitePassee.toString("dd/MM/yyyy") + " et aujourd'hui.");
        return false;
    }

    // Vérification : Pas de date dans le futur (maximum aujourd'hui)
    if (dateCreation > aujourdhui) {
        QMessageBox::warning(this, "Erreur de date",
                             "La date de création ne peut pas être une date future.");
        return false;
    }
    // ===========================================================================

    // 3. Vérification de l'ID Client (si non disponible)
    if (statut != "Disponible" && id_c.isEmpty()) {
        QMessageBox::warning(this, "Erreur de saisie", "Un ID Client est obligatoire pour un produit Réservé ou Vendu.");
        return false;
    }

    return true; // Si tout est passé, on retourne vrai
}
void MainWindow::viderFormulaire() {
    ui->le_id->clear();
    ui->sb_poids->setValue(0.0);
    ui->sb_prix->setValue(0.0);
    ui->de_date_c->setDate(QDate::currentDate());
    ui->de_date_v->setDate(QDate::currentDate());
    ui->cb_type_2->setCurrentIndex(0);

    // Remettre le statut à "Disponible"
    ui->cb_statut->setCurrentText("Disponible");

    // Forcer manuellement l'appel de la fonction qui bloque les champs
    on_cb_statut_currentIndexChanged(ui->cb_statut->currentIndex());

    // Réinitialiser l'ID de modification
    this->id_a_modifier = -1;
}
