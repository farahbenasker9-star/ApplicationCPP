#include "mainwindow.h"
#include "login.h"
#include "connection.h"
#include <QApplication>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Create connection instance and test the connection
    Connection c;
    bool connectionSuccess = c.createconnect();

    // Show popup based on connection result
    if(connectionSuccess)
    {
        QMessageBox::information(nullptr, "Connection Status",
                                 "Database connection established successfully!");
    }
    else
    {
        QMessageBox::critical(nullptr, "Connection Error",
                              "Failed to connect to the database.\n"
                              "Please check your connection settings.");
        return 1; // Exit if connection fails
    }

    // 1. On crée la fenêtre de login
    Login loginWindow;

    // 2. On l'affiche et on attend. Si le bouton "Sign In" est cliqué,
    // le slot "on_btn_login_clicked" va appeler "accept()",
    // et ".exec()" renverra QDialog::Accepted.
    if(loginWindow.exec() == QDialog::Accepted)
    {
        // 3. On crée et on affiche la fenêtre principale
        MainWindow w;
        w.show();

        // 4. On lance l'application
        return a.exec();
    }

    // Si on ferme le login sans cliquer, le programme s'arrête.
    return 0;
}
