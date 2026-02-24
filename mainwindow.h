#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employe.h"
#include "client.h" // Ajout de l'en-tête client
#include "poubelle.h"

#include <QSqlQueryModel>

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

private:
    Ui::MainWindow *ui;
    Poubelle tmp_poubelle;
    QSqlQueryModel *model;
    void navigateToPage(int pageIndex);
    void clearFormPoubelle(); // Nouvelle méthode pour vider le formulaire
    bool validerFormulairePoubelle(bool isUpdate);

    // Les gestionnaires de pages
    Employe *employe;
    Client  *client; // Pointeur pour ton module Client
};

#endif // MAINWINDOW_H
