#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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


private:
    Ui::MainWindow *ui;
    void navigateToPage(int pageIndex);

};

#endif // MAINWINDOW_H
