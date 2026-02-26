#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employe.h"
#include "client.h"
#include "poubelle.h"
#include "contrat.h"
#include "produit.h"

#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

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

    // CRUD Contrat
    void on_ajouter_contrat_clicked();
    void on_tab_contrat_2_clicked(const QModelIndex &index);
    void on_tab_contrat_2_doubleClicked(const QModelIndex &index);
    void on_supprimer_contrat_clicked();
    void on_modifier_contrat_clicked();
    void on_recherche0_contrat_textChanged(const QString &arg1);

    // Validation slots Contrat
    void validateID();
    void validateDates();
    void validateFloats();
    void validateDescription();

    //CRUD PRODUIT
    void on_btn_ajouter_4_clicked();
    void on_btn_supprimer_5_clicked();
    void on_btn_modifier_4_clicked();
    void on_cb_statut_currentIndexChanged(int index);
    void on_tab_stock_clicked(const QModelIndex &index);

private:
    Ui::MainWindow *ui;
    Poubelle tmp_poubelle;
    QSqlQueryModel *model;
    int currentSelectedId = -1; // Track selected contract ID
    void navigateToPage(int pageIndex);
    void clearFormPoubelle();
    bool validerFormulairePoubelle(bool isUpdate);

    // Style & Utils Contrat
    void setWidgetStyle(QWidget* widget, bool isValid);
    void resetValidationStyles();
    void populateComboBoxes();

    // Les gestionnaires de pages
    Employe *employe;
    Client  *client;

    bool controleSaisie();

};

#endif // MAINWINDOW_H
