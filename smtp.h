#ifndef SMTP_H
#define SMTP_H

#include <QString>
#include <QSslSocket>

class Smtp
{
public:
    // Fonction statique pour envoyer un e-mail facilement
    static bool envoyerMail(const QString &destinataire, const QString &sujet, const QString &corps);
};

#endif // SMTP_H