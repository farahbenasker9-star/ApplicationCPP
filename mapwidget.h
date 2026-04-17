#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPushButton>
#include <QString>

struct GeoPoubelle
{
    int id;
    QString type;
    QString adresse;
    int capacite;
    QString etat;
    int remplissage;
    int batterie;
    QString statut_capteur;
    double lat;
    double lng;
};

class MapWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MapWidget(QWidget *parent = nullptr);
    void setPoubelles(const QVector<GeoPoubelle> &list);

private slots:
    void onOpenMapClicked();

private:
    QPushButton *openMapButton = nullptr;
    QVector<GeoPoubelle> data;

    void generateAndOpenMap();
};

#endif // MAPWIDGET_H

