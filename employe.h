#ifndef EMPLOYE_H
#define EMPLOYE_H

#include <QString>
#include <QDate>
#include <QSqlTableModel>
#include <QtCharts/QChart>

class Employe
{
public:
    Employe();
    Employe(QString cin, QString nom, QString prenom, QString poste, double salaire, QDate dateEmb, QString idBadge, int age, QString genre);

    // CRUD
    bool ajouter(QString cin, QString nom, QString prenom, QString poste, double salaire, QDate dateEmb, QString idBadge, int age, QString genre);
    bool modifier(QString cin, QString nom, QString prenom, QString poste, double salaire, QDate dateEmb, QString idBadge, int age, QString genre);
    bool supprimer(QString cin, QString &erreurDetachement);

    // Display / Search / Sort
    QSqlTableModel* afficher();
    QSqlTableModel* rechercher(QString texte);
    QSqlTableModel* trier(QString critere);

    // Export
    bool exporterPDF(QString filePath, QSqlTableModel *model);

    // Validation
    static bool validerFormulaire(const QString &cin, const QString &nom,
                                  const QString &prenom, const QString &idBadge,
                                  double salaire, QString &errorMessage);

    // Statistiques
    QChart* createGenreChart();
    QChart* createPosteChart();
    QChart* createSalaireChart();

private:
    QString m_cin;
    QString m_nom;
    QString m_prenom;
    QString m_poste;
    double m_salaire;
    QDate m_dateEmb;
    QString m_idBadge;
    int m_age;
    QString m_genre;
};

#endif // EMPLOYE_H
