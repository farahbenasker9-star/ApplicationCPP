#include "mainwindow.h"
#include "login.h"
#include "connection.h"  // Add this
#include <QApplication>
#include <QMessageBox>   // Add this

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Test database connection before showing login
    Connection c;
    if (!c.createconnect())
    {
        // If connection fails, the error message is already shown in createconnect()
        return 1; // Exit the application
    }

    // 1. On crée la fenêtre de login
    Login loginWindow;

    // 2. On l'affiche et on attend
    if(loginWindow.exec() == QDialog::Accepted)
    {
        // 3. On crée et on affiche la fenêtre principale
        MainWindow w;
        w.show();

        // 4. On lance l'application
        return a.exec();
    }

    return 0;
}
