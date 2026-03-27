#include "contrat.h"

#include <QPrinter>
#include <QTextDocument>
#include <QDateTime>

// ─── Constructeur par défaut ────────────────────────────────────────────────
Contrat::Contrat()
    : id_contrat(0), id_client(0), cin(0), objectif_achat_annuel(0.0), taux_remise_accorde(0.0)
{}

// ─── Constructeur complet (ID_CONTRAT est auto-incrémenté) ───────────────────
Contrat::Contrat(int id_client, int cin,
                 const QString &type_exclusivite, const QString &produits_concernes,
                 const QDate &date_debut, const QDate &date_fin,
                 double objectif_achat_annuel, double taux_remise_accorde,
                 const QString &statut_contrat, const QString &clause_penale)
    : id_contrat(0), id_client(id_client), cin(cin),
      type_exclusivite(type_exclusivite), produits_concernes(produits_concernes),
      date_debut(date_debut), date_fin(date_fin),
      objectif_achat_annuel(objectif_achat_annuel), taux_remise_accorde(taux_remise_accorde),
      statut_contrat(statut_contrat), clause_penale(clause_penale)
{}

// ─── Méthode AJOUTER ────────────────────────────────────────────────────────
bool Contrat::ajouter()
{
    QSqlQuery query;

    // INSERT dans la table CONTRAT (Oracle - ECOCYCLEUSER)
    query.prepare(
        "INSERT INTO CONTRAT "
        "(ID_CLIENT, CIN, TYPE_EXCLUSIVITE, PRODUITS_CONCERNES, "
        " DATE_DEBUT, DATE_FIN, OBJECTIF_ACHAT_ANNUEL, TAUX_REMISE_ACCORDE, STATUT_CONTRAT, CLAUSE_PENALE) "
        "VALUES "
        "(:id_client, :cin, :type_ex, :prod_con, "
        " TO_DATE(:date_deb, 'DD/MM/YYYY'), TO_DATE(:date_fin, 'DD/MM/YYYY'), "
        " :ob_ach_an, :tau_rem_acc, :status, :clause)"
    );

    query.bindValue(":id_client",   id_client);
    query.bindValue(":cin",         cin);
    query.bindValue(":type_ex",     type_exclusivite);
    query.bindValue(":prod_con",    produits_concernes);
    query.bindValue(":date_deb",    date_debut.toString("dd/MM/yyyy"));
    query.bindValue(":date_fin",    date_fin.toString("dd/MM/yyyy"));
    query.bindValue(":ob_ach_an",   QString::number(objectif_achat_annuel, 'f', 2));
    query.bindValue(":tau_rem_acc", QString::number(taux_remise_accorde, 'f', 2));
    query.bindValue(":status",      statut_contrat);
    query.bindValue(":clause",      clause_penale);

    if (!query.exec()) {
        qDebug() << "Erreur ajouter contrat:" << query.lastError().text();
        return false;
    }

    qDebug() << "Contrat ajouté avec succès.";
    return true;
}

QSqlQueryModel * Contrat::afficher() const
{
    QSqlQueryModel * model = new QSqlQueryModel();

    model->setQuery("SELECT ID_CONTRAT, ID_CLIENT, CIN, TYPE_EXCLUSIVITE, PRODUITS_CONCERNES, "
                    "DATE_DEBUT, DATE_FIN, "
                    "OBJECTIF_ACHAT_ANNUEL, "
                    "TAUX_REMISE_ACCORDE, "
                    "STATUT_CONTRAT, CLAUSE_PENALE "
                    "FROM CONTRAT");

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("CIN Employé"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type Exclusivité"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Produits"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date Fin"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Obj. Achat"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Taux Rem."));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Clause Pénale"));

    return model;
}

bool Contrat::exporterPdf(const QString &filePath, QSqlQueryModel *model) const
{
    if (filePath.isEmpty()) {
        return false;
    }

    QSqlQueryModel *sourceModel = model;
    bool ownModel = false;

    if (!sourceModel) {
        sourceModel = afficher();
        ownModel = true;
    }

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
            <h1>Liste des Contrats - EcoCycle</h1>
            <p style="text-align: right; color: #666;">Généré le : %1</p>
            <table>
                <tr>
                    <th>ID</th><th>ID Client</th><th>CIN</th><th>Type</th><th>Produits</th><th>Date Début</th><th>Date Fin</th><th>Objectif</th><th>Taux</th><th>Statut</th><th>Clause</th>
                </tr>
    )";

    html = html.arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));

    for (int i = 0; i < sourceModel->rowCount(); ++i) {
        QString id_contrat  = sourceModel->data(sourceModel->index(i, 0)).toString();
        QString id_client  = sourceModel->data(sourceModel->index(i, 1)).toString();
        QString cin        = sourceModel->data(sourceModel->index(i, 2)).toString();
        QString type       = sourceModel->data(sourceModel->index(i, 3)).toString();
        QString produits   = sourceModel->data(sourceModel->index(i, 4)).toString();
        QString date_deb   = sourceModel->data(sourceModel->index(i, 5)).toDate().toString("dd/MM/yyyy");
        QString date_fin   = sourceModel->data(sourceModel->index(i, 6)).toDate().toString("dd/MM/yyyy");
        QString objectif   = sourceModel->data(sourceModel->index(i, 7)).toString();
        QString taux       = sourceModel->data(sourceModel->index(i, 8)).toString();
        QString statut     = sourceModel->data(sourceModel->index(i, 9)).toString();
        QString clause     = sourceModel->data(sourceModel->index(i, 10)).toString();

        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td><td>%7</td><td>%8</td><td>%9</td><td>%10</td><td>%11</td></tr>")
                .arg(id_contrat, id_client, cin, type, produits, date_deb, date_fin, objectif, taux, statut, clause);
    }

    html += "</table></body></html>";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);

    if (ownModel) {
        delete sourceModel;
    }

    return true;
}

QSqlQueryModel * Contrat::rechercher(const QString &texte) const
{
    QSqlQueryModel * model = new QSqlQueryModel();
    QString key = texte.trimmed();
    if (key.isEmpty()) {
        return afficher();
    }

    QString sql = QString(
        "SELECT ID_CONTRAT, ID_CLIENT, CIN, TYPE_EXCLUSIVITE, PRODUITS_CONCERNES, "
        "DATE_DEBUT, DATE_FIN, OBJECTIF_ACHAT_ANNUEL, TAUX_REMISE_ACCORDE, STATUT_CONTRAT, CLAUSE_PENALE "
        "FROM CONTRAT WHERE "
        "CAST(ID_CONTRAT AS VARCHAR2(50)) LIKE '%%1%' OR "
        "UPPER(TYPE_EXCLUSIVITE) LIKE UPPER('%%1%') OR "
        "UPPER(STATUT_CONTRAT) LIKE UPPER('%%1%')"
    ).arg(key);

    model->setQuery(sql);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("CIN Employé"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type Exclusivité"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Produits"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Date Fin"));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Obj. Achat"));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Taux Rem."));
    model->setHeaderData(9, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Clause Pénale"));

    return model;
}

bool Contrat::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CONTRAT WHERE ID_CONTRAT = :id");
    query.bindValue(":id", id);
    
    if (!query.exec()) {
        qDebug() << "Erreur suppression contrat:" << query.lastError().text();
        return false;
    }
    return true;
}

bool Contrat::modifier(int old_id)
{
    QSqlQuery query;
    query.prepare("UPDATE CONTRAT SET "
                  "ID_CONTRAT = :new_id, "
                  "ID_CLIENT = :id_client, "
                  "CIN = :cin, "
                  "TYPE_EXCLUSIVITE = :type_ex, "
                  "PRODUITS_CONCERNES = :prod_con, "
                  "DATE_DEBUT = TO_DATE(:date_deb, 'DD/MM/YYYY'), "
                  "DATE_FIN = TO_DATE(:date_fin, 'DD/MM/YYYY'), "
                  "OBJECTIF_ACHAT_ANNUEL = :ob_ach_an, "
                  "TAUX_REMISE_ACCORDE = :tau_rem_acc, "
                  "STATUT_CONTRAT = :status, "
                  "CLAUSE_PENALE = :clause "
                  "WHERE ID_CONTRAT = :old_id");

    query.bindValue(":new_id",      id_contrat);

    query.bindValue(":old_id",      old_id);
    query.bindValue(":id_client",   id_client);
    query.bindValue(":cin",         cin);
    query.bindValue(":type_ex",     type_exclusivite);
    query.bindValue(":prod_con",    produits_concernes);
    query.bindValue(":date_deb",    date_debut.toString("dd/MM/yyyy"));
    query.bindValue(":date_fin",    date_fin.toString("dd/MM/yyyy"));
    query.bindValue(":ob_ach_an",   QString::number(objectif_achat_annuel, 'f', 2));
    query.bindValue(":tau_rem_acc", QString::number(taux_remise_accorde, 'f', 2));
    query.bindValue(":status",      statut_contrat);
    query.bindValue(":clause",      clause_penale);

    if (!query.exec()) {
        qDebug() << "Erreur modification contrat:" << query.lastError().text();
        return false;
    }
    return true;
}

// ─── Méthodes de validation statiques ────────────────────────────────────────

bool Contrat::validerDateDebut(const QDate &date_debut) {
    QDate today = QDate::currentDate();
    return date_debut == today;  // date de début doit être aujourd'hui uniquement
}

bool Contrat::validerDateFin(const QDate &date_fin, const QDate &date_debut) {
    return date_fin >= date_debut; // date de fin après ou égale à début
}

bool Contrat::validerDates(const QDate &date_debut, const QDate &date_fin) {
    return validerDateDebut(date_debut) && validerDateFin(date_fin, date_debut);
}

bool Contrat::validerID(int id) {
    return id > 0;
}

bool Contrat::validerFloats(double obj_ach_ann, double tau_rem_acc) {
    return (obj_ach_ann > 0) && (tau_rem_acc >= 0 && tau_rem_acc <= 100);
}

bool Contrat::validerDescription(const QString &clause_penale) {
    return clause_penale.trimmed().length() >= 5;
}
