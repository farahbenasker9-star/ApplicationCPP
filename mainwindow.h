#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employe.h"
#include "client.h"
#include "poubelle.h"
#include "contrat.h"
#include "contratgenerator.h"
#include "produit.h"
#include "equipement.h"
#include "mapwidget.h"
#include "chatbot.h"
#include "arduino.h"

#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QModelIndex>
#include <QEvent>
#include <QLabel>
#include <QString>
#include <QDate>
#include <QTextBrowser>
#include <QTextEdit>
#include <QPushButton>
#include <QDialog>
#include <QtCharts/QChartView>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

// ─── Classe PreviewDialog pour afficher le contrat en grand ──────────────────
class PreviewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PreviewDialog(QWidget *parent = nullptr);
    void setContentHtml(const QString &html);

private:
    QTextEdit *textEdit;
};

// ─── Classe MainWindow ────────────────────────────────────────────────────────
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    // ─── Navigation ──────────────────────────────────────────────────────────
    void on_btn_nav_employes_clicked();
    void on_btn_nav_client_clicked();
    void on_btn_nav_contrat_clicked();
    void on_btn_nav_poubelle_clicked();
    void on_btn_nav_equipements_clicked();
    void on_btn_nav_stock_clicked();
    void on_btn_nav_chatbot_clicked();

    void on_btn_dash_poubelle_clicked();
    void on_btn_dash_employe_clicked();
    void on_btn_dash_client_clicked();
    void on_btn_dash_contrat_clicked();
    void on_btn_dash_equipement_clicked();
    void on_btn_dash_stock_clicked();

    // ─── CRUD Poubelle ────────────────────────────────────────────────────────
    void on_btn_ajouter_poubelle_clicked();
    void on_btn_modifier_poubelle_clicked();
    void on_btn_supprimer_poubelle_clicked();
    void on_tab_poubelle_clicked(const QModelIndex &index);
    void on_cb_etat_poubelle_currentIndexChanged(int index);
    void on_btn_tri_poubelle_clicked();
    void on_le_recherche_poubelle_textChanged(const QString &arg1);
    void on_btn_pdf_poubelle_clicked();

    // ─── Module Produit ───────────────────────────────────────────────────────
    void on_btn_ajouter_produit_clicked();
    void on_btn_supprimer_produit_clicked();
    void on_btn_modifier_produit_clicked();
    void on_cb_statut_currentIndexChanged(int index);
    void on_tab_produit_clicked(const QModelIndex &index);
    void viderFormulaire();

    void on_le_recherche_3_textChanged(const QString &arg1);

    void on_btn_trier_2_clicked();

    void on_tabWidget_Stock_currentChanged(int index);

    void on_btn_pdf_produit_clicked();
    void refreshHistorique();
    void renderHistorique(QString typeFiltre, QTextBrowser* targetFeed, QString searchKeyword);


    void on_le_search_ajout_textChanged(const QString &arg1);

    void on_le_search_modif_textChanged(const QString &arg1);

    void on_le_search_suppr_textChanged(const QString &arg1);

    void on_le_search_resv_textChanged(const QString &arg1);

    void on_le_search_vente_textChanged(const QString &arg1);


    // ─── Module Contrat ───────────────────────────────────────────────────────
    void contrat_onAjouterClicked();
    void contrat_onModifierClicked();
    void contrat_onSupprimerClicked();
    void contrat_onExporterClicked();
    void contrat_onTabClicked(const QModelIndex &index);
    void contrat_onTabDoubleClicked(const QModelIndex &index);
    void contrat_onRechercheTextChanged(const QString &arg1);
    void contrat_onTriClicked();
    void contrat_validateID();
    void contrat_validateDates();
    void contrat_validateFloats();
    void contrat_validateDescription();
    void contrat_onGenererClicked();
    void contrat_onImprimerClicked();
    void contrat_onEmailClicked();
    void contrat_updatePreviewClicked();

    // ─── Module Equipement ────────────────────────────────────────────────────
    void equipement_onAjouterClicked();
    void equipement_onModifierClicked();
    void equipement_onSupprimerClicked();
    void equipement_onExporterClicked();
    void equipement_onTabClicked(const QModelIndex &index);
    void equipement_onRechercheTextChanged(const QString &arg1);
    void equipement_onTriClicked();
    void equipement_onCINChanged(int index);
    void equipement_onStatutChanged(int index);
    void equipement_onNouveauteChanged(int index);
    void equipement_validateType();
    void equipement_validateFabricant();
    void equipement_validateStatut();
    void equipement_validateDateSuivMaint();

    // ─── Module Employe ───────────────────────────────────────────────────────
    void employe_onAjouterClicked();
    void employe_onModifierClicked();
    void employe_onSupprimerClicked();
    void employe_onTabClicked(const QModelIndex &index);
    void employe_onRechercheTextChanged(const QString &arg1);
    void employe_onTriClicked();
    void employe_onPdfClicked();
    void employe_onRefreshClicked();
    void employe_onGenererContratClicked();

private:
    Ui::MainWindow *ui;
    Client *clientObject; // Utilise la classe Client (client.cpp) pour la logique

    Poubelle tmp_poubelle;
    int poubelle_currentSelectedId = -1;
    Employe tmp_employe;

    QSqlQueryModel *model = nullptr;
    int selectedClientId = -1;
    QString currentFilter;

    // Chatbot
    Chatbot *chatbot;
    QPushButton *btn_nav_chatbot;
    bool chatbotVisible;

    void navigateToPage(int pageIndex);
    void clearFormPoubelle( );
    bool validerFormulairePoubelle(bool isUpdate);

    // ─── Module Produit ───────────────────────────────────────────────────────
    void chargerIdsClients();
    int id_a_modifier = -1;
    bool controleSaisieProduit(bool isModification = false);
    void afficherStatistiques();
    QString statut_a_modifier; // Pour stocker le statut avant la modification
    void calculerValorisation();


    // ─── Module Contrat ───────────────────────────────────────────────────────
    int contrat_currentSelectedId = -1;
    QChartView *chartViewContratType    = nullptr;
    QChartView *chartViewContratTypePie = nullptr;
    
    // Éléments onglet "Contrat" (créés en C++)
    QTextEdit *webview_contrat = nullptr;
    QPushButton *btn_generer_contrat = nullptr;
    QPushButton *btn_imprimer_contrat = nullptr;
    QPushButton *btn_email_contrat = nullptr;

    void contrat_setupStatsUI();
    void contrat_refreshStats();
    void contrat_populateComboBoxes();
    void contrat_setupModelHeaders(QSqlQueryModel *m);
    void contrat_setWidgetStyle(QWidget *widget, bool isValid);
    void contrat_resetValidationStyles();
    bool contrat_checkDates();
    ContratGenerator::ContratData collectContratData();

    // ─── Module Equipement ────────────────────────────────────────────────────
    int     equipement_currentSelectedId = -1;
    bool    equipement_sortAscending     = true;
    QString equipement_sortColumn        = "EQ.EQUIPMENT_ID";

    // ✕ error indicator labels (created once in constructor, parented to infoScrollContent)
    QLabel *eq_errCIN        = nullptr;
    QLabel *eq_errType       = nullptr;
    QLabel *eq_errFab        = nullptr;
    QLabel *eq_errStatut     = nullptr;
    QLabel *eq_errNouveaute  = nullptr;
    QLabel *eq_errNextMaint  = nullptr;

    void equipement_chargerCIN();
    void equipement_clearForm();
    void equipement_rafraichirTable();
    void equipement_setWidgetStyle(QWidget *widget, QLabel *errLabel, bool isValid);
    void equipement_clearWidgetStyle(QWidget *widget, QLabel *errLabel);
    void equipement_resetValidationStyles();
    bool equipement_validerFormulaire();
    void equipement_setupModelHeaders(QSqlQueryModel *m);
    QChartView *chartViewEquipType   = nullptr;
    QChartView *chartViewEquipStatut = nullptr;

    void equipement_setupStatsUI();
    void equipement_refreshStats();

    // ─── Module Employe ───────────────────────────────────────────────────────
    void employe_setupStatsUI();
    void employe_refreshStats();
    void employe_clearForm();

    QChartView *employe_chartViewGenre   = nullptr;
    QChartView *employe_chartViewPoste   = nullptr;
    QChartView *employe_chartViewSalaire = nullptr;
    // ─── Module Poubelle Stats ────────────────────────────────────────────────
    void poubelle_setupStatsUI();
    void poubelle_refreshStats();
    void poubelle_setupMap();
    void poubelle_refreshMap();
    void poubelle_setupPredictionAI();
    void poubelle_refreshPredictionAI();
    QChartView *poubelle_chartViewRemplissage = nullptr;
    MapWidget *poubelle_mapView = nullptr;

    // ─── Module Arduino RFID ──────────────────────────────────────────────────
    Arduino *arduino;
    void setupArduino();

private slots:
    void handle_rfid_scan(const QString &uid);
};

#endif // MAINWINDOW_H
