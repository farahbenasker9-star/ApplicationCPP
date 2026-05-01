#ifndef CONTRAT_H
#define CONTRAT_H

#include <QString>
#include <QDate>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>

// ─── Classe modèle / DAO ─────────────────────────────────────────────────────
class Contrat
{
public:
    // Constructeur par défaut
    Contrat();

    // Constructeur avec tous les champs (ID_CONTRAT est auto-incrémenté)
    Contrat(int id_client, int cin,
            const QString &type_exclusivite, const QString &produits_concernes,
            const QDate &date_debut, const QDate &date_fin,
            double objectif_achat_annuel, double taux_remise_accorde,
            const QString &statut_contrat, const QString &clause_penale);

    // Accesseurs
    int getId_contrat() const                { return id_contrat; }
    int getId_client() const                 { return id_client; }
    int getCin() const                       { return cin; }
    QString getType_exclusivite() const      { return type_exclusivite; }
    QString getProduits_concernes() const    { return produits_concernes; }
    QDate getDate_debut() const              { return date_debut; }
    QDate getDate_fin() const               { return date_fin; }
    double getObjectif_achat_annuel() const  { return objectif_achat_annuel; }
    double getTaux_remise_accorde() const    { return taux_remise_accorde; }
    QString getStatut_contrat() const        { return statut_contrat; }
    QString getClause_penale() const         { return clause_penale; }

    // Mutateurs
    void setId_contrat(int v)                      { id_contrat = v; }
    void setId_client(int v)                       { id_client = v; }
    void setCin(int v)                             { cin = v; }
    void setType_exclusivite(const QString &v)     { type_exclusivite = v; }
    void setProduits_concernes(const QString &v)   { produits_concernes = v; }
    void setDate_debut(const QDate &v)             { date_debut = v; }
    void setDate_fin(const QDate &v)               { date_fin = v; }
    void setObjectif_achat_annuel(double v)        { objectif_achat_annuel = v; }
    void setTaux_remise_accorde(double v)          { taux_remise_accorde = v; }
    void setStatut_contrat(const QString &v)       { statut_contrat = v; }
    void setClause_penale(const QString &v)        { clause_penale = v; }

    // CRUD
    bool ajouter();
    QSqlQueryModel * afficher() const;
    QSqlQueryModel * rechercher(const QString &texte) const;
    bool supprimer(int id);
    bool modifier(int old_id);
    QString getLastError() const;

    // Export PDF
    bool exporterPdf(const QString &filePath, QSqlQueryModel *model = nullptr) const;

    // Méthodes de validation statiques
    static bool validerDateDebut(const QDate &date_debut);
    static bool validerDateFin(const QDate &date_fin, const QDate &date_debut);
    static bool validerDates(const QDate &date_debut, const QDate &date_fin);
    static bool validerID(int id);
    static bool validerFloats(double obj_ach_ann, double tau_rem_acc);
    static bool validerDescription(const QString &clause_penale);

private:
    int     id_contrat;
    int     id_client;
    int     cin;
    QString type_exclusivite;
    QString produits_concernes;
    QDate   date_debut;
    QDate   date_fin;
    double  objectif_achat_annuel;
    double  taux_remise_accorde;
    QString statut_contrat;
    QString clause_penale;
    QString last_error;
};

#endif // CONTRAT_H
