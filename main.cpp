#include "mainwindow.h"
#include "login.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    Connection conn;
    if (!conn.createconnect()) {
        QMessageBox::critical(nullptr, "Erreur", "Connexion Oracle impossible.");
        return -1;
    }

    Login loginWindow;
    if(loginWindow.exec() == QDialog::Accepted) {
        MainWindow w;
        w.show();
        return a.exec();
    }
    return 0;
}
