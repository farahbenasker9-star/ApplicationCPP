#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "employe.h"

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
    // Placeholder so Qt's auto-connect (via setupUi) does not warn about
    // a missing slot for btn_nav_employes. Actual navigation is handled
    // by the explicit connect() in the constructor.
    void on_btn_nav_employes_clicked();

private:
    Ui::MainWindow *ui;
    void navigateToPage(int pageIndex);

    // Employee page manager (owns all CRUD / search / sort logic)
    Employe *employe;
};

#endif // MAINWINDOW_H
