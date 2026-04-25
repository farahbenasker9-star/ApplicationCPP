#include "mapwidget.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QDebug>
#include <QFile>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>

MapWidget::MapWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(20, 20, 20, 20);
    layout->setSpacing(15);

    // Titre
    QLabel *titleLabel = new QLabel("Carte Geolocalisation - Tunisie");
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: #1b5e20;");
    layout->addWidget(titleLabel);

    // Description
    QLabel *descLabel = new QLabel(
        "Cliquez sur le bouton pour ouvrir la carte interactive dans votre navigateur.");
    descLabel->setStyleSheet("color: #666; font-size: 12px;");
    layout->addWidget(descLabel);

    // Bouton pour ouvrir la carte
    openMapButton = new QPushButton("Ouvrir la Carte Interactive");
    openMapButton->setMaximumWidth(250);
    openMapButton->setStyleSheet(
        "QPushButton { "
        "  background-color: #4CAF50; "
        "  color: white; "
        "  padding: 12px; "
        "  border: none; "
        "  border-radius: 6px; "
        "  font-size: 14px; "
        "  font-weight: bold; "
        "} "
        "QPushButton:hover { background-color: #45a049; } "
        "QPushButton:pressed { background-color: #3d8b40; }");

    connect(openMapButton, &QPushButton::clicked, this, &MapWidget::onOpenMapClicked);
    layout->addWidget(openMapButton);

    // Ajouter un spacer pour repousser vers le haut
    layout->addStretch();

    // Infos supplementaires
    QLabel *infoLabel = new QLabel(
        "La carte affiche:\n"
        "- Poubelles fonctionnelles (vert)\n"
        "- Poubelles en panne (rouge)\n"
        "- Poubelles avec etat inconnu (gris)");
    infoLabel->setStyleSheet("color: #999; font-size: 11px; margin-top: 20px;");
    layout->addWidget(infoLabel);

    setLayout(layout);
}

void MapWidget::setPoubelles(const QVector<GeoPoubelle> &list)
{
    data = list;
    qDebug() << "MapWidget recu" << list.size() << "poubelles";
}

void MapWidget::onOpenMapClicked()
{
    qDebug() << "Ouverture de la carte interactive...";
    generateAndOpenMap();
}

void MapWidget::generateAndOpenMap()
{
    // Calculer les statistiques
    int total = 0, functional = 0, broken = 0;
    for (const auto &pb : data) {
        if (pb.etat.toUpper() == "EN_STOCK") continue;
        if (pb.etat.isEmpty()) continue;

        total++;

        bool isFunctional = (pb.statut_capteur.toUpper().contains("FONCTION") ||
                             pb.statut_capteur.toUpper() == "FONCTIONNEL" ||
                             pb.statut_capteur.toUpper() == "OK" ||
                             pb.statut_capteur.toUpper() == "ACTIF");
        bool isBroken = (pb.statut_capteur.toUpper().contains("PANNE") ||
                         pb.statut_capteur.toUpper() == "EN_PANNE");

        if (isFunctional) functional++;
        if (isBroken) broken++;
    }

    // GÃ©nÃ©rer le contenu HTML avec tous les marqueurs
    QString markers = "";
    for (const auto &pb : data) {
        if (pb.etat.toUpper() == "EN_STOCK") continue;
        if (pb.etat.isEmpty()) continue;

        QString color = "#999999";
        bool isFunctional = (pb.statut_capteur.toUpper().contains("FONCTION") ||
                             pb.statut_capteur.toUpper() == "FONCTIONNEL" ||
                             pb.statut_capteur.toUpper() == "OK" ||
                             pb.statut_capteur.toUpper() == "ACTIF");
        bool isBroken = (pb.statut_capteur.toUpper().contains("PANNE") ||
                         pb.statut_capteur.toUpper() == "EN_PANNE");

        if (pb.etat.toUpper() == "INSTALLE" || pb.etat.toUpper() == "INSTALLED") {
            if (isFunctional) {
                color = "#4caf50"; // Vert
            } else if (isBroken) {
                color = "#f44336"; // Rouge
            } else {
                color = "#4caf50"; // Vert par defaut
            }
        }

        QString status = isFunctional ? "Fonctionnelle" :
                             (isBroken ? "Panne" : "Etat inconnu");

        QString popupColor = (color == "#4caf50") ? "#4caf50" : "#f44336";

        markers += QString(
                       "const marker%1 = L.circleMarker([%2, %3], {\n"
                       "    radius: 8,\n"
                       "    fillColor: '%4',\n"
                       "    color: 'white',\n"
                       "    weight: 2,\n"
                       "    opacity: 0.8,\n"
                       "    fillOpacity: 0.8\n"
                       "}).bindPopup(`\n"
                       "    <div style='font-family: Arial; font-size: 12px;'>\n"
                       "        <strong>Poubelle #%5</strong><br>\n"
                       "        Type: %6<br>\n"
                       "        Adresse: %7<br>\n"
                       "        Ã‰tat: %8<br>\n"
                       "        Batterie: %9%%<br>\n"
                       "        <span style='display: inline-block; margin-top: 5px; padding: 3px 8px; border-radius: 3px; color: white; background: %10'>%11</span>\n"
                       "    </div>\n`);\n"
                       "markerClusterGroup.addLayer(marker%1);\n"
                       ).arg(pb.id).arg(pb.lat).arg(pb.lng).arg(color).arg(pb.id).arg(pb.type).arg(pb.adresse).arg(pb.etat).arg(pb.batterie).arg(popupColor).arg(status);
    }

    // HTML Leaflet complet
    QString htmlContent = QString(R"(
<!DOCTYPE html>
<html>
<head>
    <meta charset="utf-8"  />
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>EcoCycle - Carte Tunisie</title>
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.min.css" />
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/leaflet.markercluster/1.4.1/MarkerCluster.css" />
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/leaflet.markercluster/1.4.1/MarkerCluster.Default.css" />
    <script src="https://cdnjs.cloudflare.com/ajax/libs/leaflet/1.9.4/leaflet.min.js"></script>
    <script src="https://cdnjs.cloudflare.com/ajax/libs/leaflet.markercluster/1.4.1/leaflet.markercluster.js"></script>
    <style>
        * { margin: 0; padding: 0; box-sizing: border-box; }
        html, body { height: 100%; width: 100%; font-family: Arial, sans-serif; }
        #map { position: absolute; top: 0; left: 0; width: 100%; height: 100%; }
        .title {
            position: absolute;
            top: 10px;
            left: 50px;
            background: white;
            padding: 10px 15px;
            border-radius: 8px;
            box-shadow: 0 2px 8px rgba(0,0,0,0.15);
            font-weight: bold;
            color: #1b5e20;
            z-index: 1000;
            font-size: 14px;
        }
        .legend {
            background: white;
            padding: 12px 15px;
            border-radius: 8px;
            box-shadow: 0 2px 8px rgba(0,0,0,0.15);
            font-size: 12px;
            position: absolute;
            bottom: 80px;
            left: 10px;
            z-index: 1000;
        }
        .legend-item {
            display: flex;
            align-items: center;
            margin: 6px 0;
        }
        .legend-color {
            width: 12px;
            height: 12px;
            border-radius: 50%;
            margin-right: 8px;
            border: 1px solid white;
            box-shadow: 0 0 1px rgba(0,0,0,0.3);
        }
        .stats {
            background: rgba(27, 94, 32, 0.95);
            color: white;
            padding: 10px 15px;
            border-radius: 6px;
            font-size: 11px;
            z-index: 1000;
            font-weight: bold;
            position: absolute;
            bottom: 10px;
            left: 10px;
        }
    </style>
</head>
<body>
    <div id="map"></div>
    <div class="title">EcoCycle - Carte Tunisie</div>
    <div class="legend">
        <div style="font-weight: bold; margin-bottom: 8px; color: #1b5e20;">Etat des Poubelles</div>
        <div class="legend-item">
            <div class="legend-color" style="background: #4caf50;"></div>
            <span>Fonctionnelle</span>
        </div>
        <div class="legend-item">
            <div class="legend-color" style="background: #f44336;"></div>
            <span>En Panne</span>
        </div>
        <div class="legend-item">
            <div class="legend-color" style="background: #999999;"></div>
            <span>Ã‰tat Inconnu</span>
        </div>
    </div>
    <div class="stats">
        Total: %1 | Fonctionnelles: %2 | Pannes: %3
    </div>

    <script>
        // Initialiser la carte centrÃ©e sur la Tunisie
        const map = L.map('map').setView([34.5, 9.5], 7);

        // Ajouter les tuiles OpenStreetMap
        L.tileLayer('https://{s}.tile.openstreetmap.org/{z}/{x}/{y}.png', {
            maxZoom: 19,
            attribution: 'Â© OpenStreetMap contributors'
        }).addTo(map);

        // Initialiser le cluster group
        const markerClusterGroup = L.markerClusterGroup({
            maxClusterRadius: 50,
            disableClusteringAtZoom: 16
        });

        // Ajouter tous les marqueurs des poubelles
        %4

        // Ajouter le cluster group Ã  la carte
        map.addLayer(markerClusterGroup);
    </script>
</body>
</html>
    )").arg(total).arg(functional).arg(broken).arg(markers);

    // Sauvegarder le fichier HTML
    QString tempPath = QStandardPaths::writableLocation(QStandardPaths::TempLocation);
    QString filePath = tempPath + "/ecocycle_map.html";

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << htmlContent;
        file.close();

        // Ouvrir dans le navigateur par defaut
        QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
        qDebug() << "Carte sauvegardee et ouverte:" << filePath;
    } else {
        qWarning() << "Erreur: impossible de creer le fichier" << filePath;
    }
}
