#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QModelIndex>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QChart>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QTimer>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

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
    void onBtnTriClicked();
    void onBtnPdfClicked();

private slots:
    void onSmsReplyFinished(QNetworkReply *reply);

private:
    Ui::MainWindow *ui;
    QSqlQueryModel *model;
    QString currentFilter;
    QString currentOrder;
    int selectedClientId = -1; // Id currently selected in the table

    QChartView *chartViewStats = nullptr;
    QTimer *refreshTimer = nullptr;
    QSqlQueryModel *rendementModel = nullptr;

    QNetworkAccessManager *networkManager;
    void sendTwilioSms(const QString &to, const QString &message);
    void checkAndResetFidelite(int clientId, int currentPoints, const QString &phone);

    void rafraichirAffichage();
    void rafraichirStats();
    void rafraichirRendement();
    bool verifierSaisie(); // Fonction de contrôle de saisie centralisée
    bool reaffecterIdClientDansRelations(int oldId, int newId);

};

#endif
