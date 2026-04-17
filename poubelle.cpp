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

bool Poubelle::modifier(int oldId)
{
    QSqlQuery query;
    query.prepare("UPDATE POUBELLE_INTELLIGENTE SET ID_POUBELLE = :new_id, TYPE_DECHET = :type, ADRESSE = :addr, CAPACITE_MAX = :cap, ETAT_DEPLOYEMENT = :etat, "
                  "NIVEAU_REMPLISSAGE = :niv, ETAT_BATTERIE = :batt, STATUT_CAPTEUR = :statut, DATE_INSTALLATION = :date_inst, DATE_DERNIERE_COLLECTE = :date_coll "
                  "WHERE ID_POUBELLE = :old_id");

    query.bindValue(":new_id", id_poubelle);
    query.bindValue(":old_id", oldId);
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
    if (query.numRowsAffected() == 0) {
        lastError = "Aucune poubelle trouvée avec l'ID sélectionné.";
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
    painter.drawText(0, height - 500, width, 500, Qt::AlignCenter, "Genere par EcoCycleApp - 2026");

    painter.end();
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

void Poubelle::exporterTableauPDF()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr, "Exporter le tableau en PDF",
                                                    "Liste_Poubelles.pdf", "*.pdf");
    if (fileName.isEmpty()) return;

    // Recuperer toutes les poubelles
    QSqlQuery query("SELECT ID_POUBELLE, TYPE_DECHET, ADRESSE, CAPACITE_MAX, "
                    "ETAT_DEPLOYEMENT, NIVEAU_REMPLISSAGE, ETAT_BATTERIE, STATUT_CAPTEUR "
                    "FROM POUBELLE_INTELLIGENTE ORDER BY ID_POUBELLE");

    QVector<QStringList> tableData;
    QStringList headers;
    headers << "ID" << "Type" << "Adresse" << "Capacite" << "Etat" 
            << "Remplissage %" << "Batterie %" << "Statut Capteur";
    tableData.append(headers);

    while (query.next()) {
        QStringList row;
        row << query.value(0).toString()
            << query.value(1).toString()
            << query.value(2).toString()
            << query.value(3).toString()
            << query.value(4).toString()
            << query.value(5).toString() + "%"
            << query.value(6).toString() + "%"
            << query.value(7).toString();
        tableData.append(row);
    }

    QPdfWriter pdfWriter(fileName);
    
    // Configuration en mode paysage
    QPageSize pageSize(QPageSize::A4);
    QPageLayout layout(pageSize, QPageLayout::Landscape, 
                      QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);
    pdfWriter.setPageLayout(layout);

    QPainter painter(&pdfWriter);

    int pageWidth = pdfWriter.width();
    int pageHeight = pdfWriter.height();
    int marginLeft = 200;
    int marginTop = 300;
    int currentY = marginTop;

    // En-tete
    painter.setPen(QColor(46, 125, 50));
    painter.setFont(QFont("Arial", 22, QFont::Bold));
    painter.drawText(marginLeft, currentY, "GESTION DES POUBELLES INTELLIGENTES");

    currentY += 600;
    painter.setPen(QColor(100, 100, 100));
    painter.drawLine(marginLeft, currentY, pageWidth - marginLeft, currentY);

    // Dimension du tableau
    currentY += 400;
    int colCount = headers.size();
    int rowHeight = 400;
    int colWidth = (pageWidth - 2 * marginLeft) / colCount;

    // En-tete du tableau
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.fillRect(marginLeft, currentY, pageWidth - 2 * marginLeft, rowHeight, QColor(46, 125, 50));

    for (int col = 0; col < colCount; col++) {
        int x = marginLeft + col * colWidth;
        painter.drawText(x + 50, currentY + 50, colWidth - 100, rowHeight - 100, 
                        Qt::AlignCenter | Qt::TextWordWrap, headers[col]);
    }

    currentY += rowHeight;

    // Donnees du tableau
    painter.setFont(QFont("Arial", 9));
    bool alternateColor = false;

    for (int row = 1; row < tableData.size(); row++) {
        // Verifier si on doit passer a une nouvelle page
        if (currentY + rowHeight > pageHeight - 200) {
            pdfWriter.newPage();
            currentY = 200;
        }

        // Fond alterne
        if (alternateColor) {
            painter.fillRect(marginLeft, currentY, pageWidth - 2 * marginLeft, rowHeight, QColor(240, 245, 240));
        }

        // Lignes
        painter.setPen(QColor(200, 200, 200));
        painter.drawLine(marginLeft, currentY + rowHeight, pageWidth - marginLeft, currentY + rowHeight);

        // Contenu
        painter.setPen(Qt::black);
        for (int col = 0; col < colCount && col < tableData[row].size(); col++) {
            int x = marginLeft + col * colWidth;
            painter.drawText(x + 50, currentY + 50, colWidth - 100, rowHeight - 100,
                            Qt::AlignCenter | Qt::TextWordWrap, tableData[row][col]);
        }

        currentY += rowHeight;
        alternateColor = !alternateColor;
    }

    // Pied de page
    painter.setPen(QColor(100, 100, 100));
    painter.setFont(QFont("Arial", 8, QFont::StyleItalic));
    painter.drawText(marginLeft, pageHeight - 200, pageWidth - 2 * marginLeft, 200,
                    Qt::AlignCenter, "Genere par EcoCycleApp - 2026");

    painter.end();
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

QChart* Poubelle::createRemplissageParAdresseChart()
{
    QChart *chart = new QChart();
    QBarSeries *series = new QBarSeries();
    QBarSet *set = new QBarSet("Taux de Remplissage (%)");

    set->setBrush(QColor("#27ae60"));

    QStringList categories;
    QSqlQuery query;

    const QString queryString = "SELECT ADRESSE, AVG(NIVEAU_REMPLISSAGE) "
                                "FROM POUBELLE_INTELLIGENTE "
                                "GROUP BY ADRESSE "
                                "ORDER BY AVG(NIVEAU_REMPLISSAGE) DESC";

    if (query.exec(queryString)) {
        while (query.next()) {
            QString addr = query.value(0).toString();
            if (addr.isEmpty()) addr = "Inconnue";

            double value = query.value(1).toDouble();
            *set << value;
            categories << addr;
        }
    }

    series->append(set);
    chart->addSeries(series);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0, 100);
    axisY->setTitleText("Taux de remplissage (%)");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->setTitle("Taux de Remplissage par Adresse");
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);

    return chart;
}

QJsonArray Poubelle::getPoubellesToJSON()
{
    QJsonArray jsonArray;
    QSqlQuery query;

    query.prepare("SELECT ID_POUBELLE, TYPE_DECHET, ADRESSE, CAPACITE_MAX, ETAT_DEPLOYEMENT, "
                  "NIVEAU_REMPLISSAGE, ETAT_BATTERIE, STATUT_CAPTEUR, DATE_INSTALLATION, DATE_DERNIERE_COLLECTE "
                  "FROM POUBELLE_INTELLIGENTE");

    if (query.exec()) {
        while (query.next()) {
            QJsonObject jsonObj;

            jsonObj["id"] = query.value("ID_POUBELLE").toInt();
            jsonObj["type"] = query.value("TYPE_DECHET").toString();
            jsonObj["adresse"] = query.value("ADRESSE").toString();
            jsonObj["capacite"] = query.value("CAPACITE_MAX").toInt();
            jsonObj["etat"] = query.value("ETAT_DEPLOYEMENT").toString();
            jsonObj["remplissage"] = query.value("NIVEAU_REMPLISSAGE").toInt();
            jsonObj["batterie"] = query.value("ETAT_BATTERIE").toInt();
            jsonObj["capteur"] = query.value("STATUT_CAPTEUR").toString();
            jsonObj["installation"] = query.value("DATE_INSTALLATION").toDate().toString("dd/MM/yyyy");
            jsonObj["collecte"] = query.value("DATE_DERNIERE_COLLECTE").toDate().toString("dd/MM/yyyy");

            jsonArray.append(jsonObj);
        }
    }

    return jsonArray;
}

