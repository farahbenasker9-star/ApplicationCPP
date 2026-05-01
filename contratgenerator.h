#ifndef CONTRATGENERATOR_H
#define CONTRATGENERATOR_H

#include <QString>
#include <QDate>

// ═══════════════════════════════════════════════════════════════════════════════
// Classe ContratGenerator - Génère des contrats personnalisés avec articles
// ═══════════════════════════════════════════════════════════════════════════════

class ContratGenerator
{
public:
    struct ContratData {
        // PARTIES CONTRACTANTES - CLIENT
        QString nom_client;
        QString adresse_client;
        QString tel_client;
        QString email_client;
        
        // PRESTATAIRE - ECOCYCLEAPP (Pas d'employé spécifique, c'est l'entreprise)
        // L'employé n'a pas besoin d'être mentionné
        
        // OBJET DU CONTRAT
        QString type_exclusivite;    // "Totale" ou "Partielle"
        QString produits_concernes;
        QString gamme;
        
        // CONDITIONS
        QDate date_debut;
        QDate date_fin;
        double objectif_achat_annuel;
        double taux_remise_accorde;
        QString statut_contrat;
        QString clause_penale;
        
        // MÉTADONNÉES
        int id_contrat = 0;
        int id_client = 0;
        QString lieu_signature = "Tunis";
    };

    ContratGenerator();
    
    // Générer HTML du contrat avec articles
    QString generateContractHTML(const ContratData &data, bool forEmail = false);
    
    // Exporter en PDF
    bool exportToPDF(const ContratData &data, const QString &filePath);
    
private:
    // Templates pour différents types
    QString generateArticlesForContractType(const ContratData &data);
    QString generateExclusivityArticles(const ContratData &data);  // Articles exclusivité
    QString generateCommercialArticles(const ContratData &data);   // Articles commerciaux
    QString generateGeneralArticles(const ContratData &data);      // Articles généraux
    
    // Utilitaires
    QString formatDate(const QDate &date);
    int calculateDuration(const QDate &debut, const QDate &fin);
};

#endif // CONTRATGENERATOR_H
