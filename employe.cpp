#include "employe.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>
#include <QPrinter>
#include <QTextDocument>
#include <QDateTime>
#include <QPageSize>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QHorizontalBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

Employe::Employe()
{
    m_salaire = 0.0;
    m_age = 0;
}

Employe::Employe(QString cin, QString nom, QString prenom, QString poste, double salaire, QDate dateEmb, QString idBadge, int age, QString genre)
{
    m_cin = cin;
    m_nom = nom;
    m_prenom = prenom;
    m_poste = poste;
    m_salaire = salaire;
    m_dateEmb = dateEmb;
    m_idBadge = idBadge;
    m_age = age;
    m_genre = genre;
}

bool Employe::ajouter(QString cin, QString nom, QString prenom, QString poste, double salaire, QDate dateEmb, QString idBadge, int age, QString genre)
{
    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE, AGE, GENRE) "
                  "VALUES (:cin, :nom, :prenom, :poste, :salaire, :date_embauche, :id_badge, :age, :genre)");
    query.bindValue(":cin",          cin);
    query.bindValue(":nom",          nom);
    query.bindValue(":prenom",       prenom);
    query.bindValue(":poste",        poste);
    query.bindValue(":salaire",      salaire);
    query.bindValue(":date_embauche", dateEmb);
    query.bindValue(":id_badge",     idBadge);
    query.bindValue(":age",          age);
    query.bindValue(":genre",        genre);

    return query.exec();
}

bool Employe::modifier(QString cin, QString nom, QString prenom, QString poste, double salaire, QDate dateEmb, QString idBadge, int age, QString genre)
{
    QSqlQuery query;
    query.prepare("UPDATE EMPLOYE SET NOM = :nom, PRENOM = :prenom, POSTE = :poste, "
                  "SALAIRE = :salaire, DATE_EMBAUCHE = :date_embauche, ID_BADGE = :id_badge, "
                  "AGE = :age, GENRE = :genre "
                  "WHERE CIN = :cin");
    query.bindValue(":nom",          nom);
    query.bindValue(":prenom",       prenom);
    query.bindValue(":poste",        poste);
    query.bindValue(":salaire",      salaire);
    query.bindValue(":date_embauche", dateEmb);
    query.bindValue(":id_badge",     idBadge);
    query.bindValue(":age",          age);
    query.bindValue(":genre",        genre);
    query.bindValue(":cin",          cin);

    return query.exec();
}

bool Employe::supprimer(QString cin, QString &erreurDetachement)
{
    erreurDetachement.clear();
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CONTRAT WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);
    if(checkQuery.exec() && checkQuery.next()) {
        int count = checkQuery.value(0).toInt();
        if (count > 0) {
            QSqlQuery updateQuery;
            updateQuery.prepare("UPDATE CONTRAT SET CIN = NULL WHERE CIN = :cin");
            updateQuery.bindValue(":cin", cin);
            if (!updateQuery.exec()) {
                erreurDetachement = "Impossible de détacher les contrats : " + updateQuery.lastError().text();
                return false;
            }
        }
    }

    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYE WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    return query.exec();
}

QSqlTableModel* Employe::afficher()
{
    QSqlTableModel *employeModel = new QSqlTableModel();
    employeModel->setTable("EMPLOYE");
    employeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    employeModel->select();

    employeModel->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    employeModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    employeModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    employeModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Poste"));
    employeModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Salaire (TND)"));
    employeModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Date d'embauche"));
    employeModel->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Badge"));
    employeModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Âge"));
    employeModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Genre"));

    return employeModel;
}

QSqlTableModel* Employe::rechercher(QString texte)
{
    QSqlTableModel *employeModel = afficher();
    if (!texte.isEmpty()) {
        QString filter = QString(
            "(TO_CHAR(CIN) LIKE '%%1%' OR UPPER(NOM) LIKE UPPER('%%1%') OR UPPER(ID_BADGE) LIKE UPPER('%%1%'))"
        ).arg(texte);
        employeModel->setFilter(filter);
    }
    employeModel->select();
    return employeModel;
}

QSqlTableModel* Employe::trier(QString critere)
{
    QSqlTableModel *employeModel = afficher();
    if (critere == "CIN") {
        employeModel->setSort(0, Qt::AscendingOrder);
    } else if (critere == "Nom") {
        employeModel->setSort(1, Qt::AscendingOrder);
    } else if (critere == "ID Badge") {
        employeModel->setSort(6, Qt::AscendingOrder);
    }
    employeModel->select();
    return employeModel;
}

bool Employe::exporterPDF(QString filePath, QSqlTableModel *model)
{
    if (filePath.isEmpty() || !model) return false;

    QString html = R"(<html>
        <head>
            <style>
                body { font-family: 'Segoe UI', Arial, sans-serif; }
                h1 { color: #1B4332; text-align: center; }
                table { width: 100%; border-collapse: collapse; margin-top: 20px; }
                th { background-color: #1B4332; color: white; padding: 10px; text-align: left; }
                td { border-bottom: 1px solid #ddd; padding: 8px; color: #333; }
                tr:nth-child(even) { background-color: #f9fafb; }
            </style>
        </head>
        <body>
            <h1>Liste des Employés - EcoCycle</h1>
            <p style="text-align: right; color: #666;">Généré le : %1</p>
            <table>
                <tr>
                    <th>CIN</th><th>Nom</th><th>Prénom</th><th>Poste</th><th>Salaire</th><th>Embauche</th><th>Badge</th><th>Âge</th><th>Genre</th>
                </tr>
    )";

    html = html.arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));

    for (int i = 0; i < model->rowCount(); ++i) {
        QVariant cinData = model->data(model->index(i, 0));
        bool ok = false;
        double cinDouble = cinData.toDouble(&ok);
        QString cin = ok ? QString::number(static_cast<qulonglong>(cinDouble)).rightJustified(8, '0') : cinData.toString();
        
        QString nom = model->data(model->index(i, 1)).toString();
        QString prenom = model->data(model->index(i, 2)).toString();
        QString poste = model->data(model->index(i, 3)).toString();
        QString salaire = model->data(model->index(i, 4)).toString();
        QString dateEmb = model->data(model->index(i, 5)).toDate().toString("dd/MM/yyyy");
        QString badge = model->data(model->index(i, 6)).toString();
        QString age = model->data(model->index(i, 7)).toString();
        QString genre = model->data(model->index(i, 8)).toString();

        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td><td>%7</td><td>%8</td><td>%9</td></tr>")
                .arg(cin, nom, prenom, poste, salaire, dateEmb, badge, age, genre);
    }

    html += "</table></body></html>";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);
    return true;
}

bool Employe::validerFormulaire(const QString &cin, const QString &nom,
                                const QString &prenom, const QString &idBadge,
                                double salaire, QString &errorMessage)
{
    QRegularExpression cinRegex("^[0-9]{8}$");
    if (!cinRegex.match(cin).hasMatch()) {
        errorMessage = "Le CIN doit contenir exactement 8 chiffres.";
        return false;
    }

    QRegularExpression nameRegex("^[A-Za-zÀ-ÿ\\s]+$");
    if (nom.isEmpty() || nom.length() > 50 || !nameRegex.match(nom).hasMatch()) {
        errorMessage = "Le nom est invalide (lettres uniquement, max 50 caractères).";
        return false;
    }
    if (prenom.isEmpty() || prenom.length() > 50 || !nameRegex.match(prenom).hasMatch()) {
        errorMessage = "Le prénom est invalide (lettres uniquement, max 50 caractères).";
        return false;
    }

    if (idBadge.isEmpty() || idBadge.length() > 20) {
        errorMessage = "L'ID Badge ne peut pas être vide et doit faire max 20 caractères.";
        return false;
    }

    if (salaire <= 0) {
        errorMessage = "Le salaire doit être supérieur à 0.";
        return false;
    }

    return true;
}

QChart* Employe::createGenreChart()
{
    QChart *chartGenre = new QChart();
    QPieSeries *seriesGenre = new QPieSeries();
    QSqlQuery query;

    if (query.exec("SELECT GENRE, COUNT(*) FROM EMPLOYE GROUP BY GENRE")) {
        while (query.next()) {
            QString genre = query.value(0).toString();
            if (genre.isEmpty()) genre = "Non défini";
            int count = query.value(1).toInt();

            QPieSlice *slice = seriesGenre->append(genre + " (" + QString::number(count) + ")", count);
            slice->setLabelVisible(true);

            if (genre.toUpper() == "HOMME" || genre.toUpper() == "M") {
                slice->setBrush(QColor("#3498db"));
            } else if (genre.toUpper() == "FEMME" || genre.toUpper() == "F") {
                slice->setBrush(QColor("#e74c3c"));
            } else {
                slice->setBrush(QColor("#95a5a6"));
            }
        }
    }
    chartGenre->addSeries(seriesGenre);
    chartGenre->setTitle("Répartition par Genre");
    chartGenre->setAnimationOptions(QChart::SeriesAnimations);
    return chartGenre;
}

QChart* Employe::createPosteChart()
{
    QChart *chartPoste = new QChart();
    QPieSeries *seriesPoste = new QPieSeries();
    QSqlQuery query;

    if (query.exec("SELECT POSTE, COUNT(*) FROM EMPLOYE GROUP BY POSTE")) {
        while (query.next()) {
            QString poste = query.value(0).toString();
            if (poste.isEmpty()) continue;
            QPieSlice *slice = seriesPoste->append(poste, query.value(1).toInt());
            slice->setLabelVisible(true);
        }
    }
    chartPoste->addSeries(seriesPoste);
    chartPoste->setTitle("Répartition par Poste");
    chartPoste->setAnimationOptions(QChart::SeriesAnimations);
    return chartPoste;
}

QChart* Employe::createSalaireChart()
{
    QChart *chartSalaire = new QChart();
    QHorizontalBarSeries *seriesSalaire = new QHorizontalBarSeries();
    QBarSet *setSalaire = new QBarSet("Salaire Moyen (TND)");
    setSalaire->setBrush(QColor("#1B4332"));

    QStringList categories;
    QSqlQuery query;

    if (query.exec("SELECT POSTE, AVG(SALAIRE) FROM EMPLOYE GROUP BY POSTE ORDER BY AVG(SALAIRE) ASC")) {
        while (query.next()) {
            QString poste = query.value(0).toString();
            if (poste.isEmpty()) continue;
            categories << poste;
            *setSalaire << query.value(1).toDouble();
        }
    }

    seriesSalaire->append(setSalaire);
    chartSalaire->addSeries(seriesSalaire);
    chartSalaire->setTitle("Salaire Moyen par Poste");
    chartSalaire->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisY = new QBarCategoryAxis();
    axisY->append(categories);
    chartSalaire->addAxis(axisY, Qt::AlignLeft);
    seriesSalaire->attachAxis(axisY);

    QValueAxis *axisX = new QValueAxis();
    axisX->setLabelFormat("%.0f TND");
    chartSalaire->addAxis(axisX, Qt::AlignBottom);
    seriesSalaire->attachAxis(axisX);

    return chartSalaire;
}

