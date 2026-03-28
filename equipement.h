#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include <QString>
#include <QDate>
#include <QDateTime>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>

// ─── Classe modèle / DAO ─────────────────────────────────────────────────────
class Equipement
{
public:
    // ─── Constructeur par défaut ──────────────────────────────────────────────
    Equipement();

    // ─── Constructeur complet (EQUIPMENT_ID est auto-incrémenté) ─────────────
    Equipement(const QString &cin,
               const QString &equipment_type,
               const QString &fabricant,
               const QString &statut,
               const QDateTime &date_der_maint,
               const QDateTime &date_suiv_maint,
               const QString &notes);

    // ─── Accesseurs ──────────────────────────────────────────────────────────
    int         getEquipmentId()    const { return equipment_id; }
    QString     getCin()            const { return cin; }
    QString     getEquipmentType()  const { return equipment_type; }
    QString     getFabricant()      const { return fabricant; }
    QString     getStatut()         const { return statut; }
    QDateTime   getDateDerMaint()   const { return date_der_maint; }
    QDateTime   getDateSuivMaint()  const { return date_suiv_maint; }
    QString     getNotes()          const { return notes; }

    // ─── Mutateurs ───────────────────────────────────────────────────────────
    void setEquipmentId(int v)               { equipment_id = v; }
    void setCin(const QString &v)            { cin = v; }
    void setEquipmentType(const QString &v)  { equipment_type = v; }
    void setFabricant(const QString &v)      { fabricant = v; }
    void setStatut(const QString &v)         { statut = v; }
    void setDateDerMaint(const QDateTime &v) { date_der_maint = v; }
    void setDateSuivMaint(const QDateTime &v){ date_suiv_maint = v; }
    void setNotes(const QString &v)          { notes = v; }

    // ─── CRUD ─────────────────────────────────────────────────────────────────
    bool             ajouter();
    bool             modifier(int old_id);
    bool             supprimer(int id);
    QSqlQueryModel * afficher()                         const;
    QSqlQueryModel * rechercher(const QString &texte)   const;

    // ─── Export PDF ───────────────────────────────────────────────────────────
    bool exporterPdf(const QString &filePath, QSqlQueryModel *model = nullptr) const;
    QString getLastError() const;

    // ─── Méthodes de validation statiques ────────────────────────────────────
    static bool validerType(const QString &equipment_type);
    static bool validerFabricant(const QString &fabricant);
    static bool validerStatut(const QString &statut);
    static bool validerDateSuivMaint(const QDateTime &date_suiv_maint);
    static bool validerCoherenceCinStatut(const QString &cin, const QString &statut);

private:
    int       equipment_id;
    QString   cin;
    QString   equipment_type;
    QString   fabricant;
    QString   statut;
    QDateTime date_der_maint;
    QDateTime date_suiv_maint;
    QString   notes;
    QString   last_error;
};

#endif // EQUIPEMENT_H
