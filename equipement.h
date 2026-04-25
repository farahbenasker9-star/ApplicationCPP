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


// ═══════════════════════════════════════════════════════════════════════════════
//  EquipementDelegate  — color-coded row highlighting by next-maintenance date
//
//  Visual rules:
//    • DATE_SUIV_MAINT  >  today  →  steady soft-green  (#A8D5A2)
//    • DATE_SUIV_MAINT  <  today  →  steady soft-red     (#F4A0A0)
//    • DATE_SUIV_MAINT  == today  →  alternating yellow  (bright #FFEE00  ↔
//                                                          dark   #B89400), 500 ms
//
//  Integration (MainWindow constructor, AFTER the model is set on the view):
//  ─────────────────────────────────────────────────────────────────────────
//      auto *eqDelegate = new EquipementDelegate(ui->tab_employes_2, this);
//      ui->tab_employes_2->setItemDelegate(eqDelegate);
//  ─────────────────────────────────────────────────────────────────────────
//  The delegate is set once; it survives subsequent setModel() calls because
//  QAbstractItemView::setItemDelegate() is independent of the model.
// ═══════════════════════════════════════════════════════════════════════════════

#include <QStyledItemDelegate>
#include <QAbstractItemView>
#include <QTimer>
#include <QPainter>

class EquipementDelegate : public QStyledItemDelegate
{
    // NOTE: No Q_OBJECT macro — the delegate uses a lambda-connected timer
    // instead of a declared slot, which avoids moc/vtable linker errors when
    // the class is embedded in an existing DAO header.

public:
    // parentView – the QTableView this delegate is installed on.
    //              The flicker timer calls parentView->viewport()->update()
    //              to trigger continuous repaints only when needed.
    explicit EquipementDelegate(QAbstractItemView *parentView,
                                QObject           *parent = nullptr);

    void paint(QPainter                   *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex          &index) const override;

private:
    // Called every 500 ms: toggles the flicker state and repaints the viewport.
    // Connected via lambda in the constructor — no Q_OBJECT/slot required.
    void onFlickerTimeout();

private:
    // ── column index of DATE_SUIV_MAINT in the SELECT produced by afficher() ──
    // SELECT EQUIPMENT_ID(0) CIN(1) EMPLOYE(2) EQUIPMENT_TYPE(3) FABRICANT(4)
    //        STATUT(5) DATE_DER_MAINT(6) DATE_SUIV_MAINT(7) NOTES(8)
    static constexpr int COL_NEXT_MAINT = 7;

    // Converts a QVariant that may be a QDateTime, QDate, or formatted QString
    // into a valid QDate. Returns an invalid QDate if conversion fails.
    static QDate parseDateFromVariant(const QVariant &v);

    QAbstractItemView *m_view;
    QTimer            *m_flickerTimer;

    // Toggled by the timer; mutable so paint() (which is const) can read it.
    mutable bool m_flickerOn = false;

    // Tracks whether any "today" row is visible — used to start/stop the timer
    // efficiently so we don't burn CPU when there are no flickering rows.
    mutable bool m_hasTodayRow = false;
};

#endif // EQUIPEMENT_H
