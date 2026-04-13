#ifndef POUBELLE_H
#define POUBELLE_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

class Poubelle
{
    int id_poubelle;
    QString type_dechet;
    int capacite_max;
    int niveau_remplissage;
    int etat_batterie;
    QString statut_capteur;
    QString etat_deployement;
    QDate date_derniere_collecte;
    QDate date_installation;
    QString adresse;

public:
    // Constructeurs
    Poubelle();
    Poubelle(int id, QString type, QString addr, int cap, QString etat, 
             int remplissage, int batterie, QString capteur,
             QDate installation, QDate collecte);

    // Getters
    int getId() { return id_poubelle; }
    QString getType() { return type_dechet; }
    QString getAdresse() { return adresse; }
    int getCapacite() { return capacite_max; }
    QString getEtat() { return etat_deployement; }
    int getNiveauRemplissage() { return niveau_remplissage; }
    int getEtatBatterie() { return etat_batterie; }
    QString getStatutCapteur() { return statut_capteur; }
    QDate getDateInstallation() { return date_installation; }
    QDate getDateCollecte() { return date_derniere_collecte; }

    // Setters
    void setId(int id) { id_poubelle = id; }
    void setType(QString t) { type_dechet = t; }
    void setAdresse(QString a) { adresse = a; }
    void setCapacite(int c) { capacite_max = c; }
    void setEtat(QString e) { etat_deployement = e; }
    void setNiveauRemplissage(int r) { niveau_remplissage = r; }
    void setEtatBatterie(int b) { etat_batterie = b; }
    void setStatutCapteur(QString s) { statut_capteur = s; }
    void setDateInstallation(QDate d) { date_installation = d; }
    void setDateCollecte(QDate d) { date_derniere_collecte = d; }

    // Méthodes CRUD
    bool ajouter();
    QSqlQueryModel * afficher();
    bool supprimer(int id);
    bool modifier(int oldId);
    bool existe(int id);
    QSqlQueryModel* trier(QString critere);
    QSqlQueryModel* rechercher(QString critere, QString val);
    
    // Pour remplir le formulaire lors de la modification
    static Poubelle getPoubelle(int id);
    void exporterPDF(int id);

    // Récupérer la dernière erreur
    QString getLastError() { return lastError; }

private:
    QString lastError;
};

#endif // POUBELLE_H
