#include "equipement.h"

#include <QPrinter>
#include <QTextDocument>
#include <QDateTime>
#include <QPageSize>

// ═══════════════════════════════════════════════════════════════════════════════
//  Classe Equipement  (DAO – données uniquement)
// ═══════════════════════════════════════════════════════════════════════════════

// ─── Constructeur par défaut ────────────────────────────────────────────────
Equipement::Equipement()
    : equipment_id(0)
{}

// ─── Constructeur complet (EQUIPMENT_ID est auto-incrémenté) ─────────────────
Equipement::Equipement(const QString &cin,
                       const QString &equipment_type,
                       const QString &fabricant,
                       const QString &statut,
                       const QDateTime &date_der_maint,
                       const QDateTime &date_suiv_maint,
                       const QString &notes)
    : equipment_id(0),
      cin(cin),
      equipment_type(equipment_type),
      fabricant(fabricant),
      statut(statut),
      date_der_maint(date_der_maint),
      date_suiv_maint(date_suiv_maint),
      notes(notes)
{}

// ─── Méthode AJOUTER ─────────────────────────────────────────────────────────
bool Equipement::ajouter()
{
    last_error.clear();

    auto bindCommon = [&](QSqlQuery &q) {
        q.bindValue(":cin",    cin.isEmpty() ? QVariant(QVariant::String) : QVariant(cin));
        q.bindValue(":type",   equipment_type);
        q.bindValue(":fab",    fabricant);
        q.bindValue(":statut", statut);
        q.bindValue(":last",   date_der_maint.isValid() ? QVariant(date_der_maint) : QVariant(QVariant::DateTime));
        q.bindValue(":next",   date_suiv_maint.isValid() ? QVariant(date_suiv_maint) : QVariant(QVariant::DateTime));
        q.bindValue(":notes",  notes);
    };

    QSqlQuery query;
    query.prepare(
        "INSERT INTO EQUIPMENT "
        "  (EQUIPMENT_ID, CIN, EQUIPMENT_TYPE, FABRICANT, STATUT, "
        "   DATE_DER_MAINT, DATE_SUIV_MAINT, NOTES) "
        "VALUES "
        "  (SEQ_EQUIPMENT.NEXTVAL, :cin, :type, :fab, :statut, "
        "   :last, :next, :notes)"
    );
    bindCommon(query);

    if (!query.exec()) {
        last_error = query.lastError().text();

        // Fallback for schemas where SEQ_EQUIPMENT does not exist.
        if (last_error.contains("ORA-02289", Qt::CaseInsensitive) ||
            last_error.contains("SEQ_EQUIPMENT", Qt::CaseInsensitive)) {
            QSqlQuery qId;
            if (!qId.exec("SELECT NVL(MAX(EQUIPMENT_ID), 0) + 1 FROM EQUIPMENT") || !qId.next()) {
                last_error = qId.lastError().text();
                qDebug() << "Erreur ajouter equipement (fallback id):" << last_error;
                return false;
            }

            const int nextId = qId.value(0).toInt();
            QSqlQuery qInsert;
            qInsert.prepare(
                "INSERT INTO EQUIPMENT "
                "  (EQUIPMENT_ID, CIN, EQUIPMENT_TYPE, FABRICANT, STATUT, "
                "   DATE_DER_MAINT, DATE_SUIV_MAINT, NOTES) "
                "VALUES "
                "  (:id, :cin, :type, :fab, :statut, :last, :next, :notes)"
            );
            qInsert.bindValue(":id", nextId);
            bindCommon(qInsert);

            if (!qInsert.exec()) {
                last_error = qInsert.lastError().text();
                qDebug() << "Erreur ajouter equipement (fallback insert):" << last_error;
                return false;
            }
        } else {
            qDebug() << "Erreur ajouter equipement:" << last_error;
            return false;
        }
    }

    qDebug() << "Équipement ajouté avec succès.";
    return true;
}

// ─── Méthode AFFICHER ────────────────────────────────────────────────────────
QSqlQueryModel * Equipement::afficher() const
{
    QSqlQueryModel *model = new QSqlQueryModel();

    model->setQuery(
        "SELECT EQ.EQUIPMENT_ID, "
        "       EQ.CIN, "
        "       E.NOM || ' ' || E.PRENOM AS EMPLOYE, "
        "       EQ.EQUIPMENT_TYPE, "
        "       EQ.FABRICANT, "
        "       EQ.STATUT, "
        "       EQ.DATE_DER_MAINT, "
        "       EQ.DATE_SUIV_MAINT, "
        "       EQ.NOTES "
        "FROM   EQUIPMENT EQ "
        "LEFT JOIN EMPLOYE E ON EQ.CIN = E.CIN "
        "ORDER BY EQ.EQUIPMENT_ID"
    );

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Employé"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Fabricant"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Dern. Maint."));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Proch. Maint."));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Notes"));

    return model;
}

// ─── Méthode RECHERCHER ──────────────────────────────────────────────────────
QSqlQueryModel * Equipement::rechercher(const QString &texte) const
{
    QSqlQueryModel *model = new QSqlQueryModel();
    QString key = texte.trimmed();
    if (key.isEmpty()) {
        return afficher();
    }

    QString sql = QString(
        "SELECT EQ.EQUIPMENT_ID, EQ.CIN, "
        "       E.NOM || ' ' || E.PRENOM AS EMPLOYE, "
        "       EQ.EQUIPMENT_TYPE, EQ.FABRICANT, EQ.STATUT, "
        "       EQ.DATE_DER_MAINT, EQ.DATE_SUIV_MAINT, EQ.NOTES "
        "FROM   EQUIPMENT EQ "
        "LEFT JOIN EMPLOYE E ON EQ.CIN = E.CIN "
        "WHERE  UPPER(EQ.FABRICANT)      LIKE UPPER('%%1%') OR "
        "       UPPER(EQ.EQUIPMENT_TYPE) LIKE UPPER('%%1%') OR "
        "       UPPER(EQ.STATUT)         LIKE UPPER('%%1%') OR "
        "       UPPER(E.NOM)             LIKE UPPER('%%1%') OR "
        "       UPPER(E.PRENOM)          LIKE UPPER('%%1%')"
    ).arg(key);

    model->setQuery(sql);
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("CIN"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Employé"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Type"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Fabricant"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("Dern. Maint."));
    model->setHeaderData(7, Qt::Horizontal, QObject::tr("Proch. Maint."));
    model->setHeaderData(8, Qt::Horizontal, QObject::tr("Notes"));

    return model;
}

// ─── Méthode MODIFIER ────────────────────────────────────────────────────────
bool Equipement::modifier(int old_id)
{
    QSqlQuery query;
    last_error.clear();

    query.prepare(
        "UPDATE EQUIPMENT SET "
        "  CIN             = :cin, "
        "  EQUIPMENT_TYPE  = :type, "
        "  FABRICANT       = :fab, "
        "  STATUT          = :statut, "
        "  DATE_DER_MAINT  = :last, "
        "  DATE_SUIV_MAINT = :next, "
        "  NOTES           = :notes "
        "WHERE EQUIPMENT_ID = :old_id"
    );

    query.bindValue(":cin",    cin.isEmpty() ? QVariant(QVariant::String) : QVariant(cin));
    query.bindValue(":type",   equipment_type);
    query.bindValue(":fab",    fabricant);
    query.bindValue(":statut", statut);
    query.bindValue(":last",   date_der_maint.isValid() ? QVariant(date_der_maint) : QVariant(QVariant::DateTime));
    query.bindValue(":next",   date_suiv_maint.isValid() ? QVariant(date_suiv_maint) : QVariant(QVariant::DateTime));
    query.bindValue(":notes",  notes);
    query.bindValue(":old_id", old_id);

    if (!query.exec()) {
        last_error = query.lastError().text();
        qDebug() << "Erreur modification equipement:" << last_error;
        return false;
    }
    return true;
}

QString Equipement::getLastError() const
{
    return last_error;
}

// ─── Méthode SUPPRIMER ───────────────────────────────────────────────────────
bool Equipement::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM EQUIPMENT WHERE EQUIPMENT_ID = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur suppression equipement:" << query.lastError().text();
        return false;
    }
    return true;
}

// ─── Export PDF ──────────────────────────────────────────────────────────────
bool Equipement::exporterPdf(const QString &filePath, QSqlQueryModel *model) const
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
            <h1>Liste des Équipements - EcoCycle</h1>
            <p style="text-align: right; color: #666;">Généré le : %1</p>
            <table>
                <tr>
                    <th>ID</th><th>CIN</th><th>Employé</th><th>Type</th>
                    <th>Fabricant</th><th>Statut</th>
                    <th>Dern. Maint.</th><th>Proch. Maint.</th><th>Notes</th>
                </tr>
    )";

    html = html.arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));

    for (int i = 0; i < sourceModel->rowCount(); ++i) {
        QString id_s       = sourceModel->data(sourceModel->index(i, 0)).toString();
        QString cin_s      = sourceModel->data(sourceModel->index(i, 1)).toString();
        QString employe_s  = sourceModel->data(sourceModel->index(i, 2)).toString();
        QString type_s     = sourceModel->data(sourceModel->index(i, 3)).toString();
        QString fab_s      = sourceModel->data(sourceModel->index(i, 4)).toString();
        QString statut_s   = sourceModel->data(sourceModel->index(i, 5)).toString();
        QString last_s     = sourceModel->data(sourceModel->index(i, 6)).toDateTime().toString("dd/MM/yyyy");
        QString next_s     = sourceModel->data(sourceModel->index(i, 7)).toDateTime().toString("dd/MM/yyyy");
        QString notes_s    = sourceModel->data(sourceModel->index(i, 8)).toString();

        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td>"
                        "<td>%5</td><td>%6</td><td>%7</td><td>%8</td><td>%9</td></tr>")
                .arg(id_s, cin_s, employe_s, type_s, fab_s, statut_s, last_s, next_s, notes_s);
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

// ─── Méthodes de validation statiques ────────────────────────────────────────

bool Equipement::validerType(const QString &equipment_type) {
    return !equipment_type.trimmed().isEmpty() && equipment_type != "-";
}

bool Equipement::validerFabricant(const QString &fabricant) {
    return !fabricant.trimmed().isEmpty() && fabricant != "-";
}

bool Equipement::validerStatut(const QString &statut) {
    return !statut.trimmed().isEmpty() && statut != "-";
}

bool Equipement::validerDateSuivMaint(const QDateTime &date_suiv_maint) {
    return date_suiv_maint.date() >= QDate::currentDate();
}

bool Equipement::validerCoherenceCinStatut(const QString &cin, const QString &statut) {
    bool cinSelected = !cin.trimmed().isEmpty();
    if (!cinSelected && statut != "Disponible")  return false;
    if (cinSelected  && statut == "Disponible")  return false;
    return true;
}
