#include "emailcontrat.h"
#include <QSslSocket>
#include <QTextStream>
#include <QDebug>
#include <QFile>
#include <QDateTime>

// ═══════════════════════════════════════════════════════════════════════════
// VOS IDENTIFIANTS – À REMPLIR (voir instructions ci-dessous)
// ═══════════════════════════════════════════════════════════════════════════
static const QString EC_FROM     = "dhrifmeriem1231230@gmail.com";   // ← votre Gmail
static const QString EC_PASSWORD = "qrsx xpxv jswu nzyw";  // ← mot de passe app 16 car.

// ═══════════════════════════════════════════════════════════════════════════

bool EmailContrat::envoyerContrat(const QString &destinataire,
                                  const QString &sujet,
                                  const QString &htmlContrat,
                                  QString &erreurMsg)
{
    erreurMsg.clear();

    QSslSocket socket;
    socket.connectToHostEncrypted("smtp.gmail.com", 465);

    if (!socket.waitForConnected(8000)) {
        erreurMsg = "Connexion échouée : " + socket.errorString();
        return false;
    }
    if (!socket.waitForEncrypted(8000)) {
        erreurMsg = "Erreur SSL : " + socket.errorString();
        return false;
    }

    QTextStream t(&socket);
    auto readResp = [&]() -> QByteArray {
        socket.waitForReadyRead(5000);
        return socket.readAll();
    };
    auto send = [&](const QString &cmd) {
        t << cmd; t.flush();
    };

    readResp();
    send("EHLO localhost\r\n");                              readResp();
    send("AUTH LOGIN\r\n");                                  readResp();
    send(EC_FROM.toUtf8().toBase64()     + "\r\n");         readResp();
    send(EC_PASSWORD.toUtf8().toBase64() + "\r\n");         readResp();
    send("MAIL FROM:<" + EC_FROM         + ">\r\n");        readResp();
    send("RCPT TO:<"   + destinataire    + ">\r\n");        readResp();
    send("DATA\r\n");                                        readResp();

    // ─── CONSTRUCTION DU MESSAGE MULTIPART ───
    QString boundary = "boundary_ecocycle_limit_" + QString::number(QDateTime::currentMSecsSinceEpoch());
    
    // Fonction helper pour charger une image en base64 pour MIME
    auto getImageBase64 = [](const QString &resPath) -> QByteArray {
        QFile file(resPath);
        if (file.open(QIODevice::ReadOnly)) {
            return file.readAll().toBase64();
        }
        return QByteArray();
    };

    QByteArray logoData      = getImageBase64(":/new/prefix1/images/lougo2.png");
    QByteArray signatureData = getImageBase64(":/new/prefix1/images/signa2.png");

    QString message = 
        "From: EcoCycle <" + EC_FROM + ">\r\n"
        "To: " + destinataire + "\r\n"
        "Subject: " + sujet + "\r\n"
        "MIME-Version: 1.0\r\n"
        "Content-Type: multipart/related; boundary=\"" + boundary + "\"\r\n"
        "\r\n"
        "--" + boundary + "\r\n"
        "Content-Type: text/html; charset=utf-8\r\n"
        "Content-Transfer-Encoding: 7bit\r\n"
        "\r\n"
        + htmlContrat + "\r\n\r\n";

    // Ajout du LOGO
    if (!logoData.isEmpty()) {
        message += "--" + boundary + "\r\n"
                   "Content-Type: image/png\r\n"
                   "Content-Transfer-Encoding: base64\r\n"
                   "Content-ID: <logo>\r\n"
                   "Content-Disposition: inline\r\n"
                   "\r\n"
                   + QString::fromLatin1(logoData) + "\r\n\r\n";
    }

    // Ajout de la SIGNATURE
    if (!signatureData.isEmpty()) {
        message += "--" + boundary + "\r\n"
                   "Content-Type: image/png\r\n"
                   "Content-Transfer-Encoding: base64\r\n"
                   "Content-ID: <signature>\r\n"
                   "Content-Disposition: inline\r\n"
                   "\r\n"
                   + QString::fromLatin1(signatureData) + "\r\n\r\n";
    }

    message += "--" + boundary + "--\r\n.\r\n";

    send(message);
    QByteArray resp = readResp();

    send("QUIT\r\n");
    socket.disconnectFromHost();

    bool ok = resp.contains("250");
    if (!ok) erreurMsg = "Serveur Gmail a refusé le message.";
    
    return ok;
}
