#ifndef EMAILCONTRAT_H
#define EMAILCONTRAT_H

#include <QString>

// ═══════════════════════════════════════════════════════════════════════════
// EmailContrat – Service d'envoi d'email pour les contrats EcoCycle
// Utilise Gmail SMTP (SSL port 465)
// ═══════════════════════════════════════════════════════════════════════════
class EmailContrat
{
public:
    // Envoie le contrat HTML par email
    // Retourne true si succès, false sinon + rempli erreurMsg
    static bool envoyerContrat(const QString &destinataire,
                               const QString &sujet,
                               const QString &htmlContrat,
                               QString &erreurMsg);
};

#endif // EMAILCONTRAT_H
