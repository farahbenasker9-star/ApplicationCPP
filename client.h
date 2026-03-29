#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QModelIndex>

namespace Ui { class MainWindow; }

class Client : public QObject {
    Q_OBJECT
public:
    explicit Client(Ui::MainWindow *ui, QObject *parent = nullptr);

private slots:
    void onBtnAjouterClicked();
    void onBtnModifierClicked();
    void onBtnSupprimerClicked();
    void onTableClicked(const QModelIndex &index);
    void onRechercheTextChanged(const QString &text);

private:
    Ui::MainWindow *ui;
    QSqlQueryModel *model;
    QString currentFilter;
    int selectedClientId = -1; // Id currently selected in the table

    void rafraichirAffichage();
    bool verifierSaisie(); // Fonction de contrôle de saisie centralisée
    bool reaffecterIdClientDansRelations(int oldId, int newId);
};

#endif
