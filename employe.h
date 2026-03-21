#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QObject>
#include <QSqlTableModel>
#include <QModelIndex>
#include <QLabel>

#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QHorizontalBarSeries>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

// Forward-declare the generated UI class to avoid tight coupling in header
namespace Ui { class MainWindow; }

class Employe : public QObject
{
    Q_OBJECT

public:
    // Takes a pointer to the MainWindow's UI so it can interact with the widgets
    explicit Employe(Ui::MainWindow *ui, QObject *parent = nullptr);

    // Initialiser et mettre à jour les statistiques
    void setupStatsUI();
    void updateStats();

private slots:
    // Called when a row in the employee table is clicked (populates the form)
    void onTabEmployesClicked(const QModelIndex &index);

    // CRUD slots
    void onBtnAjouterClicked();
    void onBtnModifierClicked();
    void onBtnSupprimerClicked();

    // Search & Sort slots
    void onLeRechercheTextChanged(const QString &arg1);
    void onBtnTriClicked();

    // PDF Export
    void onBtnPdfClicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *employeModel;

    // Loads data from DB and binds it to the table view
    void afficherEmployes();

    // Shared validation logic used by Add and Edit
    bool validerFormulaire(const QString &cin, const QString &nom,
                           const QString &prenom, const QString &idBadge,
                           double salaire);

    // Composants de la page statistiques
    QChartView *chartViewGenre;
    QChartView *chartViewPoste;
    QChartView *chartViewSalaire;
};

#endif // EMPLOYE_H
