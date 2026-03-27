#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employe.h"
#include "client.h"
#include "poubelle.h"
#include "contrat.h"
#include "produit.h"
#include "equipement.h"

#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QModelIndex>
#include <QEvent>

#include <QtCharts/QChartView>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private slots:
    // Navigation
    void on_btn_nav_employes_clicked();
    void on_btn_nav_client_clicked();
    void on_btn_nav_contrat_clicked();
    void on_btn_nav_poubelle_clicked();
    void on_btn_nav_equipements_clicked();
    void on_btn_nav_stock_clicked();

    void on_btn_dash_poubelle_clicked();
    void on_btn_dash_employe_clicked();
    void on_btn_dash_client_clicked();
    void on_btn_dash_contrat_clicked();
    void on_btn_dash_equipement_clicked();
    void on_btn_dash_stock_clicked();

    // CRUD Poubelle
    void on_btn_ajouter_poubelle_clicked();
    void on_btn_modifier_poubelle_clicked();
    void on_btn_supprimer_poubelle_clicked();
    void on_tab_poubelle_clicked(const QModelIndex &index);
    void on_cb_etat_poubelle_currentIndexChanged(int index);
    void on_btn_tri_poubelle_clicked();
    void on_le_recherche_poubelle_textChanged(const QString &arg1);

    // === MODULE PRODUIT ===
    void on_btn_ajouter_produit_clicked();
    void on_btn_supprimer_produit_clicked();
    void on_btn_modifier_produit_clicked();
    void on_cb_statut_currentIndexChanged(int index);
    void on_tab_produit_clicked(const QModelIndex &index);
    void viderFormulaire();

    // === MODULE CONTRAT ===
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

private:
    Ui::MainWindow *ui;
    Poubelle tmp_poubelle;
    QSqlQueryModel *model;

    void navigateToPage(int pageIndex);
    void clearFormPoubelle();
    bool validerFormulairePoubelle(bool isUpdate);


    // Gestionnaires de pages (même pattern partout)
    Employe    *employe;
    Client     *client;
    Equipement *equipement;

    // Produit
    bool controleSaisieProduit();
    void chargerIdsClients();
    int id_a_modifier = -1;

    // === CONTRAT ===
    int contrat_currentSelectedId = -1;
    QChartView *chartViewContratType    = nullptr;
    QChartView *chartViewContratTypePie = nullptr;

    void contrat_setupStatsUI();
    void contrat_refreshStats();
    void contrat_populateComboBoxes();
    void contrat_setupModelHeaders(QSqlQueryModel *m);
    void contrat_setWidgetStyle(QWidget *widget, bool isValid);
    void contrat_resetValidationStyles();
    bool contrat_checkDates();
};

#endif // MAINWINDOW_H
