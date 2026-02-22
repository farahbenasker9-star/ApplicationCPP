#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel> // Required for QSqlTableModel

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
    // On garde juste la navigation de base pour le test
    void on_btn_nav_employes_clicked();

    // CRUD Slots for Employees
    void on_tab_employes_clicked(const QModelIndex &index);
    void on_btn_ajouter_clicked();
    void on_btn_modifier_clicked();
    void on_btn_supprimer_clicked();

    // Search and Sort Slots for Employees
    void on_le_recherche_textChanged(const QString &arg1);
    void on_btn_tri_clicked();

private:
    Ui::MainWindow *ui;
    void navigateToPage(int pageIndex);
    
    // Pointer to our SQL Table Model
    QSqlTableModel *employeModel;
    
    // Method to setup and display the employees
    void afficherEmployes();
};

#endif // MAINWINDOW_H
