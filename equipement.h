#ifndef EQUIPEMENT_H
#define EQUIPEMENT_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QDateTimeEdit>
#include <QPlainTextEdit>
#include <QTableView>
#include <QLineEdit>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QDateTime>
#include <QDebug>

// Forward declarations
namespace Ui { class MainWindow; }

class Equipement : public QObject
{
    Q_OBJECT

public:
    // ─── Constructor / Destructor ──────────────────────────────────────────────
    explicit Equipement(Ui::MainWindow *ui, QWidget *parent = nullptr);
    ~Equipement();

    // ─── Database CRUD ─────────────────────────────────────────────────────────
    bool            ajouter();
    bool            modifier();
    bool            supprimer();
    QSqlQueryModel* afficher();
    void            rafraichirTable();

    // ─── UI Helpers ────────────────────────────────────────────────────────────
    void chargerCIN();          // Populate Eq_CIN combo from EMPLOYE table
    void clearForm();           // Reset all form fields

public slots:
    // Triggered by UI buttons (connected in constructor)
    void onAjouter();
    void onModifier();
    void onSupprimer();
    void onRowSelected(const QModelIndex &index);
    void onSearch();
    void onSort();
    void onCINChanged(int index);     // Auto-set statut hint when CIN changes
    void onStatutChanged(int index);  // Re-validate on statut change

private:
    // ─── Validation ────────────────────────────────────────────────────────────
    bool validerFormulaire();                    // Returns true if all valid
    void setFieldError(QWidget *field,
                       QLabel  *errorLabel,
                       bool     hasError,
                       const QString &msg = "✕"); // Apply/remove red border + X

    void resetValidationStyles();                // Clear all error styles

    // ─── Members ───────────────────────────────────────────────────────────────
    Ui::MainWindow   *ui;
    QWidget          *parentWidget;
    QSqlQueryModel   *model;
    int               selectedEquipmentId;       // ID of the row currently selected
    bool    sortAscending;   // tracks current sort direction
    QString sortColumn;

    // Error indicator labels (created dynamically inside infoScrollContent)
    QLabel *errCIN;
    QLabel *errType;
    QLabel *errFab;
    QLabel *errStatut;
    QLabel *errNextMaint;

    // Helper to get the scroll content widget (parent of form widgets)
    QWidget* formContainer();
};

#endif // EQUIPEMENT_H
