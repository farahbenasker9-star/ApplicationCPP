#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include "contrat.h"

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
    void on_btn_nav_employes_clicked();
    void on_ajouter_contrat_clicked(); 
    void on_tab_contrat_2_clicked(const QModelIndex &index);
    void on_tab_contrat_2_doubleClicked(const QModelIndex &index);
    void on_supprimer_contrat_clicked();
    void on_modifier_contrat_clicked();
    
    // Validation slots
    void validateID();
    void validateDates();
    void validateFloats();
    void validateDescription();

private:
    Ui::MainWindow *ui;
    int currentSelectedId = -1; // Track selected contract ID
    void navigateToPage(int pageIndex);
    bool connecterBD();   // Connexion à la base Oracle
    void setWidgetStyle(QWidget* widget, bool isValid);
    void resetValidationStyles();
    void populateComboBoxes(); // New: Populate Clients and Employees
};

#endif // MAINWINDOW_H
