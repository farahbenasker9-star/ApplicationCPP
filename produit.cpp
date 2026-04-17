#include "produit.h"
#include <QFile>
#include <QTextStream>
#include <QDateTime>

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
QSqlQueryModel* Produit::rechercherSimple(QString valeur) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    // On cherche dans ID_PRODUIT (converti en texte), STATUT et TYPE_PRODUIT
    query.prepare("SELECT TO_CHAR(ID_PRODUIT) AS \"ID_PRODUIT\", "
                  "TO_CHAR(ID_CLIENT) AS \"ID_CLIENT\", "
                  "TYPE_PRODUIT, POIDS_KG, DATE_DE_CREATION, DATE_DE_VENTE, STATUT, PRIX_VENTE_ESTIME "
                  "FROM PRODUIT WHERE "
                  "TO_CHAR(ID_PRODUIT) LIKE :val OR "
                  "STATUT LIKE :val OR "
                  "TYPE_PRODUIT LIKE :val");

    query.bindValue(":val", "%" + valeur + "%");
    query.exec();

    model->setQuery(query);
    return model;
}
QSqlQueryModel* Produit::trier(QString critere) {
    QSqlQueryModel* model = new QSqlQueryModel();
    QString triSQL;

    // Mapping entre le texte de la ComboBox et les colonnes de la BDD
    if (critere == "Date") {
        triSQL = "DATE_DE_CREATION DESC"; // Les plus récents en premier
    } else if (critere == "Poids") {
        triSQL = "POIDS_KG ASC";          // Du plus léger au plus lourd
    } else if (critere == "Prix") {
        triSQL = "PRIX_VENTE_ESTIME DESC"; // Du plus cher au moins cher
    } else {
        triSQL = "ID_PRODUIT ASC";         // Tri par défaut par ID
    }

    // On utilise la même structure de requête que pour l'affichage
    model->setQuery("SELECT TO_CHAR(ID_PRODUIT), TO_CHAR(ID_CLIENT), TYPE_PRODUIT, POIDS_KG, "
                    "DATE_DE_CREATION, DATE_DE_VENTE, STATUT, PRIX_VENTE_ESTIME "
                    "FROM PRODUIT ORDER BY " + triSQL);

    return model;
}
QMap<QString, int> Produit::statistiqueParType() {
    QMap<QString, int> stats;
    QSqlQuery query("SELECT TYPE_PRODUIT, COUNT(*) FROM PRODUIT GROUP BY TYPE_PRODUIT");
    while (query.next()) {
        stats.insert(query.value(0).toString(), query.value(1).toInt());
    }
    return stats;
}

QMap<QString, double> Produit::statistiquePoidsParType() {
    QMap<QString, double> stats;
    QSqlQuery query("SELECT TYPE_PRODUIT, SUM(POIDS_KG) FROM PRODUIT GROUP BY TYPE_PRODUIT");
    while (query.next()) {
        stats.insert(query.value(0).toString(), query.value(1).toDouble());
    }
    return stats;
}

bool Produit::enregistrerAction(int id_p, QString type_action, QString type_produit, QString details) {
    QFile file("historique_ecocycle.csv");
    if (!file.open(QIODevice::Append | QIODevice::Text))
        return false;

    QTextStream out(&file);
    QString date = QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm:ss");

    // On enregistre maintenant 5 champs : Date ; Action ; ID ; Type ; Détails
    out << date << ";" << type_action << ";" << QString::number(id_p) << ";" << type_produit << ";" << details << "\n";

    file.close();
    return true;
}
