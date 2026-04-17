#ifndef PRODUIT_H
#define PRODUIT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>
#include <QMap>

class Produit {
    int id_produit, id_client;
    QString type_produit, statut;
    float poids, prix;
    QDate date_creation, date_vente;

public:
    Produit() {}
    Produit(int id_p, int id_c, QString type, float p_kg, QDate d_c, QDate d_v, QString s, float pr);

    // CRUD
    bool ajouter();
    QSqlQueryModel* afficher();
    bool supprimer(int id);
    bool modifier();
    bool modifier(int old_id); // On ajoute old_id
    // Ajoutez ceci dans la partie "public" de votre classe Produit dans produit.h
    bool enregistrerAction(int id_p, QString type_action, QString type_produit, QString details);
    QSqlQueryModel* afficherHistorique();

    // Recherche et Tri
    QSqlQueryModel* chercher(QString valeur, QString critere);
    QSqlQueryModel* rechercherSimple(QString valeur); //1
    QSqlQueryModel* trier(QString critere); //2
    QMap<QString, int> statistiqueParType();      // Pour le nombre de produits
    QMap<QString, double> statistiquePoidsParType(); // Pour le poids total
};

#endif
