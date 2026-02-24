#include "mainwindow.h"
#include "login.h"
#include "connection.h" // <--- Inclure ta classe de connexion
#include <QApplication>
#include <QDebug> // Pour le d√bogage

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection c; // Cr√e une instance de ta classe de connexion
    if (c.createconnect()) { // Tente d'√tablir la connexion √ la base de donn√es
        // Si la connexion BDD est r√ussie, on passe √ la page de login
        Login loginDialog;

        if (loginDialog.exec() == QDialog::Accepted) {
            // Si le login est r√ussi, on affiche la fen√™tre principale
            MainWindow w;
            w.show();
            return a.exec();
        } else {
            // Login √chou√ ou annul√, on quitte l'application
            qDebug() << "Connexion utilisateur annul√e ou √chou√e. Application ferm√e.";
            return 0;
        }
    } else {
        // Si la connexion BDD √choue d√®s le d√but, on ne lance rien et on quitte.
        qCritical() << "√chec de la connexion √ la base de donn√es. Application ferm√e.";
        return 1; // Retourne un code d'erreur
    }
}
