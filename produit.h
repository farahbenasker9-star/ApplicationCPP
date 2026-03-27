#ifndef PRODUIT_H
#define PRODUIT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>

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

    // Recherche et Tri
    QSqlQueryModel* chercher(QString valeur, QString critere);
    QSqlQueryModel* trier(QString critere);
};

#endif
