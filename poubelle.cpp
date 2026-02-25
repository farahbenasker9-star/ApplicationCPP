#include "poubelle.h"
#include "connection.h"
#include <QSqlError>
#include <QDebug>
#include <QVariant>

Poubelle::Poubelle()
{
    id_poubelle = 0;
    type_dechet = "";
    adresse = "";
    capacite_max = 0;
    etat_deployement = "EN_STOCK";
}

Poubelle::Poubelle(int id, QString type, QString addr, int cap, QString etat, 
                   int remplissage, int batterie, QString capteur,
                   QDate installation, QDate collecte)
{
    this->id_poubelle = id;
    this->type_dechet = type;
    this->adresse = addr;
    this->capacite_max = cap;
    this->etat_deployement = etat;
    this->niveau_remplissage = remplissage;
    this->etat_batterie = batterie;
    this->statut_capteur = capteur;
    this->date_installation = installation;
    this->date_derniere_collecte = collecte;
}

bool Poubelle::ajouter()
{
    QSqlQuery query;
    
    query.prepare("INSERT INTO POUBELLE_INTELLIGENTE (ID_POUBELLE, TYPE_DECHET, CAPACITE_MAX, NIVEAU_REMPLISSAGE, ETAT_BATTERIE, STATUT_CAPTEUR, DATE_INSTALLATION, DATE_DERNIERE_COLLECTE, ADRESSE, ETAT_DEPLOYEMENT) "
                  "VALUES (:id, :type, :cap, :niv, :batt, :statut, :date_inst, :date_coll, :addr, :etat)");
    
    query.bindValue(":id", id_poubelle);
    query.bindValue(":type", type_dechet);
    query.bindValue(":cap", capacite_max);
    query.bindValue(":niv", niveau_remplissage);
    query.bindValue(":batt", etat_batterie);
    
    if(statut_capteur.isEmpty() || statut_capteur == " ") query.bindValue(":statut", QVariant(QVariant::String));
    else query.bindValue(":statut", statut_capteur);
    
    // Mapping etat pour contrainte Oracle
    QString etat = etat_deployement.toUpper().trimmed();
    if (etat == "EN STOCK" || etat == "EN_STOCK" || etat == "STOCK") etat = "EN_STOCK";
    else if (etat == "INSTALLÉ" || etat == "INSTALLE") etat = "INSTALLE";
    else if (etat == "MAINTENANCE") etat = "MAINTENANCE";

    if (etat == "EN_STOCK") {
        query.bindValue(":date_inst", QVariant(QVariant::Date));
        query.bindValue(":date_coll", QVariant(QVariant::Date));
    } else {
        query.bindValue(":date_inst", date_installation);
        query.bindValue(":date_coll", date_derniere_collecte);
    }
    
    query.bindValue(":addr", adresse);
    query.bindValue(":etat", etat);

    if(!query.exec()) {
        lastError = query.lastError().text();
        qDebug() << "Erreur SQL (Ajout) :" << lastError;
        return false;
    }
    return true;
}

QSqlQueryModel * Poubelle::afficher()
{
    QSqlQueryModel * model = new QSqlQueryModel();
    QSqlDatabase db = QSqlDatabase::database();

    // Insert dummy data if the table is empty (for testing)
    QSqlQuery query(db);
    query.prepare("SELECT ID_POUBELLE, TYPE_DECHET, ADRESSE, CAPACITE_MAX, ETAT_DEPLOYEMENT, NIVEAU_REMPLISSAGE, ETAT_BATTERIE, STATUT_CAPTEUR, DATE_INSTALLATION, DATE_DERNIERE_COLLECTE FROM POUBELLE_INTELLIGENTE");
    query.exec();
    model->setQuery(query);
    
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Capacité (L)"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("État"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Remplissage (%)"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Batterie (%)"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Statut Capteur"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Installation"));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Dernière Collecte"));

    return model;
}

bool Poubelle::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM POUBELLE_INTELLIGENTE WHERE ID_POUBELLE = :id");
    query.bindValue(":id", id);
    return query.exec();
}

bool Poubelle::modifier()
{
    QSqlQuery query;
    query.prepare("UPDATE POUBELLE_INTELLIGENTE SET TYPE_DECHET = :type, ADRESSE = :addr, CAPACITE_MAX = :cap, ETAT_DEPLOYEMENT = :etat, "
                  "NIVEAU_REMPLISSAGE = :niv, ETAT_BATTERIE = :batt, STATUT_CAPTEUR = :statut, DATE_INSTALLATION = :date_inst, DATE_DERNIERE_COLLECTE = :date_coll "
                  "WHERE ID_POUBELLE = :id");
    
    query.bindValue(":id", id_poubelle);
    query.bindValue(":type", type_dechet);
    query.bindValue(":addr", adresse);
    query.bindValue(":cap", capacite_max);
    
    QString etat = etat_deployement.toUpper().trimmed();
    if (etat == "EN STOCK" || etat == "EN_STOCK" || etat == "STOCK") etat = "EN_STOCK";
    else if (etat == "INSTALLÉ" || etat == "INSTALLE") etat = "INSTALLE";
    else if (etat == "MAINTENANCE") etat = "MAINTENANCE";

    query.bindValue(":etat", etat);
    query.bindValue(":niv", niveau_remplissage);
    query.bindValue(":batt", etat_batterie);
    
    if(statut_capteur.isEmpty() || statut_capteur == " ") query.bindValue(":statut", QVariant(QVariant::String));
    else query.bindValue(":statut", statut_capteur);

    if (etat == "EN_STOCK") {
        query.bindValue(":date_inst", QVariant(QVariant::Date));
        query.bindValue(":date_coll", QVariant(QVariant::Date));
    } else {
        query.bindValue(":date_inst", date_installation);
        query.bindValue(":date_coll", date_derniere_collecte);
    }
    
    if(!query.exec()) {
        lastError = query.lastError().text();
        return false;
    }
    return true;
}

QSqlQueryModel* Poubelle::trier(QString critere)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    QString queryStr = "SELECT * FROM POUBELLE_INTELLIGENTE ORDER BY " + critere;
    query.prepare(queryStr);
    query.exec();
    model->setQuery(query);
    return model;
}

QSqlQueryModel* Poubelle::rechercher(QString val)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    QString queryStr = "SELECT * FROM POUBELLE_INTELLIGENTE WHERE ID_POUBELLE LIKE :val OR ADRESSE LIKE :val";
    query.prepare(queryStr);
    query.bindValue(":val", val + "%");
    query.exec();
    model->setQuery(query);
    return model;
}

bool Poubelle::existe(int id)
{
    QSqlQuery query;
    query.prepare("SELECT ID_POUBELLE FROM POUBELLE_INTELLIGENTE WHERE ID_POUBELLE = :id");
    query.bindValue(":id", id);
    if (query.exec() && query.next())
        return true;
    return false;
}

Poubelle Poubelle::getPoubelle(int id)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM POUBELLE_INTELLIGENTE WHERE ID_POUBELLE = :id");
    query.bindValue(":id", id);
    
    if (query.exec() && query.next()) {
        return Poubelle(
            query.value("ID_POUBELLE").toInt(),
            query.value("TYPE_DECHET").toString(),
            query.value("ADRESSE").toString(),
            query.value("CAPACITE_MAX").toInt(),
            query.value("ETAT_DEPLOYEMENT").toString(),
            query.value("NIVEAU_REMPLISSAGE").toInt(),
            query.value("ETAT_BATTERIE").toInt(),
            query.value("STATUT_CAPTEUR").toString(),
            query.value("DATE_INSTALLATION").toDate(),
            query.value("DATE_DERNIERE_COLLECTE").toDate()
        );
    }
    return Poubelle();
}
