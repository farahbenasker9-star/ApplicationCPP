#include "poubelle.h"
#include "connection.h"
#include <QSqlError>
#include <QDebug>
#include <QVariant>
#include <QPdfWriter>
#include <QPainter>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>

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
    
    if(statut_capteur.isEmpty() || statut_capteur == " ") query.bindValue(":statut", QVariant(QMetaType::fromType<QString>()));
    else query.bindValue(":statut", statut_capteur);
    
    // Mapping etat pour contrainte Oracle
    QString etat = etat_deployement.toUpper().trimmed();
    if (etat == "EN STOCK" || etat == "EN_STOCK" || etat == "STOCK") etat = "EN_STOCK";
    else if (etat == "INSTALLÉ" || etat == "INSTALLE") etat = "INSTALLE";
    else if (etat == "MAINTENANCE") etat = "MAINTENANCE";

    if (etat == "EN_STOCK") {
        query.bindValue(":date_inst", QVariant(QMetaType::fromType<QDate>()));
        query.bindValue(":date_coll", QVariant(QMetaType::fromType<QDate>()));
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
    QSqlQuery query;
    query.prepare("SELECT ID_POUBELLE, TYPE_DECHET, ADRESSE, CAPACITE_MAX, ETAT_DEPLOYEMENT, NIVEAU_REMPLISSAGE, ETAT_BATTERIE, STATUT_CAPTEUR, DATE_INSTALLATION, DATE_DERNIERE_COLLECTE FROM POUBELLE_INTELLIGENTE");
    query.exec();
    model->setQuery(std::move(query));
    
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
    
    if(statut_capteur.isEmpty() || statut_capteur == " ") query.bindValue(":statut", QVariant(QMetaType::fromType<QString>()));
    else query.bindValue(":statut", statut_capteur);

    if (etat == "EN_STOCK") {
        query.bindValue(":date_inst", QVariant(QMetaType::fromType<QDate>()));
        query.bindValue(":date_coll", QVariant(QMetaType::fromType<QDate>()));
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
    QSqlQueryModel * model = new QSqlQueryModel();
    QSqlQuery query;
    QString queryStr = "SELECT ID_POUBELLE, TYPE_DECHET, ADRESSE, CAPACITE_MAX, ETAT_DEPLOYEMENT, NIVEAU_REMPLISSAGE, ETAT_BATTERIE, STATUT_CAPTEUR, DATE_INSTALLATION, DATE_DERNIERE_COLLECTE FROM POUBELLE_INTELLIGENTE ORDER BY " + critere;
    query.prepare(queryStr);
    query.exec();
    model->setQuery(std::move(query));
    
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

QSqlQueryModel* Poubelle::rechercher(QString critere, QString val)
{
    QSqlQueryModel * model = new QSqlQueryModel();
    QSqlQuery query;
    
    QString column = "ID_POUBELLE";
    if (critere == "Type de déchets" || critere == "Type de déchet" || critere == "Type") column = "TYPE_DECHET";
    else if (critere == "Adresse") column = "ADRESSE";
    
    QString queryStr = "SELECT ID_POUBELLE, TYPE_DECHET, ADRESSE, CAPACITE_MAX, ETAT_DEPLOYEMENT, NIVEAU_REMPLISSAGE, ETAT_BATTERIE, STATUT_CAPTEUR, DATE_INSTALLATION, DATE_DERNIERE_COLLECTE FROM POUBELLE_INTELLIGENTE WHERE UPPER(" + column + ") LIKE UPPER(:val)";
    query.prepare(queryStr);
    query.bindValue(":val", "%" + val + "%");
    query.exec();
    model->setQuery(std::move(query));

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

void Poubelle::exporterPDF(int id)
{
    Poubelle p = getPoubelle(id);
    if (p.getId() == 0) return;

    QString fileName = QFileDialog::getSaveFileName(nullptr, "Exporter en PDF", 
                        QString("Fiche_Poubelle_%1.pdf").arg(id), "*.pdf");
    
    if (fileName.isEmpty()) return;

    QPdfWriter pdfWriter(fileName);
    pdfWriter.setPageSize(QPageSize(QPageSize::A4));
    pdfWriter.setPageMargins(QMarginsF(30, 30, 30, 30));

    QPainter painter(&pdfWriter);
    int width = pdfWriter.width();
    int height = pdfWriter.height();

    // -- DESIGN --
    painter.setPen(QColor(46, 125, 50)); // Vert foncé
    painter.setFont(QFont("Arial", 25, QFont::Bold));
    painter.drawText(0, 500, width, 500, Qt::AlignCenter, "FICHE TECHNIQUE POUBELLE");
    
    painter.setPen(Qt::black);
    painter.drawLine(100, 1100, width-100, 1100);

    // Infos
    QFont labelFont("Arial", 12, QFont::Bold);
    QFont valFont("Arial", 12);
    int y = 1800;
    int step = 600;

    auto drawRow = [&](QString label, QString value) {
        painter.setFont(labelFont);
        painter.setPen(QColor(70, 70, 70));
        painter.drawText(200, y, label);
        painter.setFont(valFont);
        painter.setPen(Qt::black);
        painter.drawText(2500, y, value);
        y += step;
    };

    drawRow("Identifiant :", QString::number(p.getId()));
    drawRow("Type de Déchet :", p.getType());
    drawRow("Adresse :", p.getAdresse());
    drawRow("Capacité Max :", QString::number(p.getCapacite()) + " Litres");
    drawRow("État Déploiement :", p.getEtat());
    drawRow("Niveau Remplissage :", QString::number(p.getNiveauRemplissage()) + " %");
    drawRow("État Batterie :", QString::number(p.getEtatBatterie()) + " %");
    drawRow("Statut Capteur :", p.getStatutCapteur());
    drawRow("Date Installation :", p.getDateInstallation().toString("dd/MM/yyyy"));
    drawRow("Prochaine Collecte :", p.getDateCollecte().toString("dd/MM/yyyy"));

    painter.setFont(QFont("Arial", 10, QFont::StyleItalic));
    painter.drawText(0, height - 500, width, 500, Qt::AlignCenter, "Généré par EcoCycleApp - 2026");

    painter.end();
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

