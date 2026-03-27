#include "produit.h"

Produit::Produit(int id_p, int id_c, QString type, float p_kg, QDate d_c, QDate d_v, QString s, float pr) {
    this->id_produit = id_p; this->id_client = id_c; this->type_produit = type;
    this->poids = p_kg; this->date_creation = d_c; this->date_vente = d_v;
    this->statut = s; this->prix = pr;
}

bool Produit::ajouter() {
    QSqlQuery query;
    query.prepare("INSERT INTO PRODUIT (ID_PRODUIT, ID_CLIENT, TYPE_PRODUIT, POIDS_KG, DATE_DE_CREATION, DATE_DE_VENTE, STATUT, PRIX_VENTE_ESTIME) "
                  "VALUES (:idp, :idc, :type, :poids, :dc, :dv, :statut, :prix)");

    // 1. Les valeurs qui ne changent jamais
    query.bindValue(":idp", id_produit);
    query.bindValue(":type", type_produit);
    query.bindValue(":poids", poids);
    query.bindValue(":dc", date_creation);
    query.bindValue(":statut", statut);
    query.bindValue(":prix", prix);

    // 2. Logique conditionnelle pour la Date de Vente et l'ID Client
    if (statut == "Vendu") {
        query.bindValue(":dv", date_vente);
        query.bindValue(":idc", id_client);
    }
    else if (statut == "Réservé" || statut == "Reservé") {
        query.bindValue(":dv", QVariant(QMetaType::fromType<QDate>())); // Force NULL
        query.bindValue(":idc", id_client); // On garde le client pour la réservation
    }
    else { // Disponible
        query.bindValue(":dv", QVariant(QMetaType::fromType<QDate>())); // Force NULL
        query.bindValue(":idc", QVariant(QMetaType::fromType<int>()));   // Force NULL
    }

    return query.exec();
}

QSqlQueryModel* Produit::afficher() {
    QSqlQueryModel* model = new QSqlQueryModel();
    // Utilise TO_CHAR pour l'ID_PRODUIT et l'ID_CLIENT pour éviter le format E+07
    model->setQuery("SELECT TO_CHAR(ID_PRODUIT) as ID_Produit, TO_CHAR(ID_CLIENT) as ID_CLIENT, TYPE_PRODUIT, POIDS_KG, "
                    "DATE_DE_CREATION, DATE_DE_VENTE, STATUT, PRIX_VENTE_ESTIME "
                    "FROM PRODUIT");
    return model;
}

bool Produit::supprimer(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM PRODUIT WHERE ID_PRODUIT = :id");
    query.bindValue(":id", id);
    return query.exec();
}

/*bool Produit::modifier() {
    QSqlQuery query;

    // 1. On prépare la requête SQL de mise à jour
    query.prepare("UPDATE PRODUIT SET ID_CLIENT=:idc, TYPE_PRODUIT=:type, POIDS_KG=:poids, "
                  "DATE_DE_CREATION=:dc, DATE_DE_VENTE=:dv, STATUT=:statut, PRIX_VENTE_ESTIME=:prix "
                  "WHERE ID_PRODUIT=:idp");

    // 2. On lie les valeurs qui ne changent pas de logique
    query.bindValue(":idp", id_produit);
    query.bindValue(":type", type_produit);
    query.bindValue(":poids", poids);
    query.bindValue(":dc", date_creation);
    query.bindValue(":statut", statut);
    query.bindValue(":prix", prix);

    // 3. Logique conditionnelle pour ID_CLIENT et DATE_DE_VENTE
    // On utilise .trimmed() pour être sûr de la comparaison de texte
    QString st = statut.trimmed();

    if (st == "Vendu") {
        query.bindValue(":dv", date_vente);
        query.bindValue(":idc", id_client);
    }
    else if (st == "Réservé" || st == "Reservé") {
        // Pour Réservé : On garde l'ID Client mais on force la Date à NULL (vide)
        query.bindValue(":dv", QVariant(QMetaType::fromType<QDate>()));
        query.bindValue(":idc", id_client);
    }
    else {
        // Pour Disponible : On force tout à NULL (vide)
        query.bindValue(":dv", QVariant(QMetaType::fromType<QDate>()));
        query.bindValue(":idc", QVariant(QMetaType::fromType<int>()));
    }

    // 4. Exécution de la requête
    return query.exec();
}*/
bool Produit::modifier(int old_id) {
    QSqlQuery query;
    query.prepare("UPDATE PRODUIT SET ID_PRODUIT=:new_id, ID_CLIENT=:idc, TYPE_PRODUIT=:type, "
                  "POIDS_KG=:poids, DATE_DE_CREATION=:dc, DATE_DE_VENTE=:dv, STATUT=:statut, PRIX_VENTE_ESTIME=:prix "
                  "WHERE ID_PRODUIT=:old_id");

    query.bindValue(":new_id", id_produit);
    query.bindValue(":old_id", old_id);
    query.bindValue(":type", type_produit);
    query.bindValue(":poids", poids);
    query.bindValue(":dc", date_creation);
    query.bindValue(":statut", statut);
    query.bindValue(":prix", prix);

    QString st = statut.trimmed();

    if (st == "Vendu") {
        query.bindValue(":dv", date_vente);
        query.bindValue(":idc", id_client);
    }
    else if (st == "Réservé" || st == "Reservé") {
        query.bindValue(":dv", QVariant(QMetaType::fromType<QDate>())); // NULL
        query.bindValue(":idc", id_client);
    }
    else { // Disponible
        query.bindValue(":dv", QVariant(QMetaType::fromType<QDate>())); // NULL
        query.bindValue(":idc", QVariant(QMetaType::fromType<int>()));   // NULL (très important !)
    }

    return query.exec();
}
