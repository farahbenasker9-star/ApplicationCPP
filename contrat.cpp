#include "contrat.h"

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

QSqlQueryModel * Contrat::afficher()
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
