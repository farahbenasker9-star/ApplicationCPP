#include "smtp.h"
#include <QTextStream>
#include <QDebug>

bool Smtp::envoyerMail(const QString &destinataire, const QString &sujet, const QString &corps)
{
    
    QString expediteur = "ampor988@gmail.com"; 
    QString motDePasse = "llvcdsxshrfevtxh"; 

    QSslSocket socket;
    // Connexion au serveur sécurisé de Google (Port 465)
    socket.connectToHostEncrypted("smtp.gmail.com", 465);
    
    if (!socket.waitForConnected(5000)) {
        qDebug() << "Erreur de connexion SMTP:" << socket.errorString();
        return false;
    }

    QTextStream t(&socket);
    
    // Petite fonction lambda pour lire les réponses du serveur
    auto readResp = [&]() {
        socket.waitForReadyRead(3000);
        return socket.readAll();
    };

    readResp(); // Lecture du message d'accueil de Gmail

    // Protocole SMTP pour s'identifier et envoyer l'e-mail
    t << "EHLO localhost\r\n"; t.flush(); readResp();
    t << "AUTH LOGIN\r\n"; t.flush(); readResp();
    
    // Envoi des identifiants encodés en Base64
    t << QByteArray().append(expediteur.toUtf8()).toBase64() << "\r\n"; t.flush(); readResp();
    t << QByteArray().append(motDePasse.toUtf8()).toBase64() << "\r\n"; t.flush(); readResp();
    
    t << "MAIL FROM:<" << expediteur << ">\r\n"; t.flush(); readResp();
    t << "RCPT TO:<" << destinataire << ">\r\n"; t.flush(); readResp();
    t << "DATA\r\n"; t.flush(); readResp();
    
    // Corps de l'e-mail au format HTML
    t << "From: EcoCycle Admin <" << expediteur << ">\r\n"
      << "To: " << destinataire << "\r\n"
      << "Subject: " << sujet << "\r\n"
      << "MIME-Version: 1.0\r\n"
      << "Content-Type: text/html; charset=utf-8\r\n\r\n"
      << corps << "\r\n.\r\n";
    t.flush(); readResp();

    t << "QUIT\r\n"; t.flush();
    socket.disconnectFromHost();
    return true;
}