#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employe.h"
#include "client.h" // Ajout de l'en-tête client

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
    // Slot placeholder pour éviter les erreurs d'auto-connexion de Qt
    void on_btn_nav_employes_clicked();

private:
    Ui::MainWindow *ui;
    void navigateToPage(int pageIndex);

    // Les gestionnaires de pages
    Employe *employe;
    Client  *client; // Pointeur pour ton module Client
};

#endif // MAINWINDOW_H
