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
#include <QFile>

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

static QString employe_imageToBase64(const QString &resourcePath)
{
    QFile f(resourcePath);
    if (!f.open(QIODevice::ReadOnly)) return QString();
    QByteArray data = f.readAll();
    QString ext = resourcePath.split('.').last().toLower();
    return QString("data:image/%1;base64,%2").arg(ext, QString(data.toBase64()));
}

bool Employe::genererContratPDF(const QString &filePath, const QString &cin, 
                                const QString &typeContrat, 
                                int periodeEssai, const QString &lieuTravail)
{
    if (filePath.isEmpty()) return false;

    QSqlQuery query;
    query.prepare("SELECT NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE FROM EMPLOYE WHERE CIN = :cin");
    query.bindValue(":cin", cin);
    if (!query.exec() || !query.next()) {
        qDebug() << "Erreur: Impossible de trouver l'employé avec le CIN" << cin;
        return false;
    }

    QString nom = query.value(0).toString();
    QString prenom = query.value(1).toString();
    QString poste = query.value(2).toString();
    double salaire = query.value(3).toDouble();
    QDate dateEmb = query.value(4).toDate();

    QString logoPath = employe_imageToBase64(":/new/prefix1/images/lougo.png");
    QString html =
        "<html>\n"
        "<head>\n"
        "    <style>\n"
        "        body { font-family: 'Times New Roman', serif; font-size: 12pt; color: #000; margin: 40px; }\n"
        "        .logo-container { text-align: center; margin-bottom: 20px; }\n"
        "        h1 { text-align: center; color: #1B4332; font-size: 18pt; text-transform: uppercase; text-decoration: underline; margin-bottom: 30px;}\n"
        "        h2 { color: #333; font-size: 14pt; margin-top: 20px; }\n"
        "        p { text-align: justify; line-height: 1.5; }\n"
        "        .bold { font-weight: bold; }\n"
        "        .signature-box { width: 100%; margin-top: 50px; }\n"
        "        .signature-left { float: left; width: 45%; text-align: center; }\n"
        "        .signature-right { float: right; width: 45%; text-align: center; }\n"
        "        .footer { text-align: center; font-size: 9pt; color: #777; margin-top: 50px; border-top: 1px solid #ccc; padding-top: 10px; }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <div class=\"logo-container\">\n"
        "        <img src=\"%11\" width=\"200\" />\n"
        "    </div>\n"
        "    <h1>Contrat de Travail (%1)</h1>\n"
        "    \n"
        "    <p>Entre les soussign&eacute;s :</p>\n"
        "    <p><span class=\"bold\">La soci&eacute;t&eacute; EcoCycle</span>, sise &agrave; Tunis, repr&eacute;sent&eacute;e par son Directeur G&eacute;n&eacute;ral,<br>\n"
        "    Ci-apr&egrave;s d&eacute;nomm&eacute;e <span class=\"bold\">\"L'Employeur\"</span>,</p>\n"
        "    \n"
        "    <p>ET</p>\n"
        "    \n"
        "    <p>M./Mme <span class=\"bold\">%2 %3</span>, titulaire du CIN n&deg; <span class=\"bold\">%4</span>,<br>\n"
        "    Ci-apr&egrave;s d&eacute;nomm&eacute;(e) <span class=\"bold\">\"L'Employ&eacute;(e)\"</span>.</p>\n"
        "    \n"
        "    <p>Il a &eacute;t&eacute; convenu et arr&ecirc;t&eacute; ce qui suit :</p>\n"
        "\n"
        "    <h2>Article 1 : Objet et Poste</h2>\n"
        "    <p>L'Employeur engage l'Employ&eacute;(e) dans le cadre d'un contrat de type <span class=\"bold\">%1</span>. L'Employ&eacute;(e) exercera les fonctions de <span class=\"bold\">%5</span>.</p>\n"
        "\n"
        "    <h2>Article 2 : Date d'effet et P&eacute;riode d'essai</h2>\n"
        "    <p>Le pr&eacute;sent contrat prend effet &agrave; compter du <span class=\"bold\">%6</span>. Il est assorti d'une p&eacute;riode d'essai de <span class=\"bold\">%7 mois</span>, durant laquelle chacune des parties pourra rompre le contrat sans pr&eacute;avis ni indemnit&eacute;.</p>\n"
        "\n"
        "    <h2>Article 3 : R&eacute;mun&eacute;ration</h2>\n"
        "    <p>En contrepartie de ses services, l'Employ&eacute;(e) percevra un salaire mensuel brut de <span class=\"bold\">%8 TND</span>. Ce salaire sera vers&eacute; &agrave; la fin de chaque mois par virement bancaire.</p>\n"
        "\n"
        "    <h2>Article 4 : Lieu de travail</h2>\n"
        "    <p>Le lieu de travail est fix&eacute; &agrave; : <span class=\"bold\">%9</span>. L'Employeur se r&eacute;serve le droit de modifier ce lieu en fonction des n&eacute;cessit&eacute;s du service.</p>\n"
        "\n"
        "    <h2>Article 5 : Confidentialit&eacute;</h2>\n"
        "    <p>L'Employ&eacute;(e) s'engage &agrave; conserver une discr&eacute;tion absolue sur l'ensemble des informations et documents relatifs &agrave; EcoCycle auxquels il/elle aura acc&egrave;s dans l'exercice de ses fonctions.</p>\n"
        "\n"
        "    <p style=\"margin-top: 40px; text-align: right;\">Fait &agrave; Tunis, le %10, en deux exemplaires originaux.</p>\n"
        "\n"
        "    <div class=\"signature-box\">\n"
        "        <div class=\"signature-left\">\n"
        "            <p class=\"bold\">L'Employ&eacute;(e)</p>\n"
        "            <p style=\"font-size: 10pt; font-style: italic;\">(Lu et approuv&eacute;)</p>\n"
        "        </div>\n"
        "        <div class=\"signature-right\">\n"
        "            <p class=\"bold\">Pour EcoCycle</p>\n"
        "            <p style=\"font-size: 10pt; font-style: italic;\">(Le Directeur / La Direction RH)</p>\n"
        "        </div>\n"
        "    </div>\n"
        "\n"
        "    <div style=\"clear: both;\"></div>\n"
        "\n"
        "    <div class=\"footer\">\n"
        "        EcoCycle - Entreprise Innovante de Recyclage - SIRET : 123456789 - Contact : rh@ecocycle.tn\n"
        "    </div>\n"
        "</body>\n"
        "</html>";

    // Remplacement des %1 à %11
    // Astuce Qt : '.arg()' ne peut prendre que 9 paramètres maximum à la fois.
    // On doit donc enchaîner les appels '.arg()' pour les paramètres 10 et 11 !
    html = html.arg(typeContrat, nom.toUpper(), prenom, cin, poste, 
                    dateEmb.toString("dd/MM/yyyy"), QString::number(periodeEssai), 
                    QString::number(salaire, 'f', 2), lieuTravail)
               .arg(QDate::currentDate().toString("dd/MM/yyyy"), logoPath);

    // Impression en PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));
    
    // Ajout de marges pour un rendu "Document Officiel"
    printer.setPageMargins(QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);

    return true;
}