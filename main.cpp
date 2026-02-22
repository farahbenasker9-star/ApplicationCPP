#include "mainwindow.h"
#include "login.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Connect to the Oracle database
    Connection conn;
    bool test = conn.createconnect();

    if (test) {
        QMessageBox::information(
            nullptr,
            QObject::tr("DONE"),
            QObject::tr("Connection successful"),
            QMessageBox::Cancel
        );
    } else {
        QMessageBox::critical(
            nullptr,
            QObject::tr("ERROR"),
            QObject::tr("Unable to establish a database connection."),
            QMessageBox::Cancel
        );
        return -1;
    }

    // 2. On crée la fenêtre de login
    Login loginWindow;

    // 3. On l'affiche et on attend. Si le bouton "Sign In" est cliqué,
    // le slot "on_btn_login_clicked" va appeler "accept()",
    // et ".exec()" renverra QDialog::Accepted.
    if(loginWindow.exec() == QDialog::Accepted)
    {
        // 4. On crée et on affiche la fenêtre principale
        MainWindow w;
        w.show();

        // 5. On lance l'application
        return a.exec();
    }

    // Si on ferme le login sans cliquer, le programme s'arrête.
    return 0;
}
