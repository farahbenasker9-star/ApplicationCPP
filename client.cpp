#include "client.h"
#include "ui_mainwindow.h"
#include "connection.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QRegularExpression>
#include <QDebug>
#include <QFileDialog>
#include <QPrinter>
#include <QPdfWriter>
#include <QPainter>
#include <QTextDocument>
#include <QDateTime>
#include <QBuffer>
#include <QtMath>

#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

Client::Client(Ui::MainWindow *ui, QObject *parent) : QObject(parent), ui(ui) {
    // 1. Modèle en lecture pour limiter les colonnes (évite les doublons visibles)
    model = new QSqlQueryModel(this);
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &Client::onSmsReplyFinished);

    // 2. Lier le modèle au QTableView
    ui->tab_clients->setModel(model);
    ui->tab_clients->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tab_clients->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tab_clients->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    rendementModel = new QSqlQueryModel(this);
    ui->tab_clients_4->setModel(rendementModel);

    // Redimensionnement automatique pour voir toutes les colonnes
    ui->tab_clients_4->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    // Mais laisser la dernière colonne prendre le reste si besoin
    ui->tab_clients_4->horizontalHeader()->setStretchLastSection(true);

    // Rendre la police plus petite pour gagner de la place
    QFont tableFont = ui->tab_clients_4->font();
    tableFont.setPointSize(8);
    ui->tab_clients_4->setFont(tableFont);

    rafraichirAffichage();
    rafraichirStats();
    rafraichirRendement();

    // Configuration du rafraîchissement automatique (toutes les 5 secondes)
    refreshTimer = new QTimer(this);
    connect(refreshTimer, &QTimer::timeout, this, &Client::rafraichirRendement);
    refreshTimer->start(5000); // 5000 ms = 5 secondes

    // Connexions
    connect(ui->btn_ajouter_client, &QPushButton::clicked, this, &Client::onBtnAjouterClicked);
    connect(ui->btn_modifier_client, &QPushButton::clicked, this, &Client::onBtnModifierClicked);
    connect(ui->btn_supprimer_client, &QPushButton::clicked, this, &Client::onBtnSupprimerClicked);
    connect(ui->tab_clients, &QTableView::clicked, this, &Client::onTableClicked);
    connect(ui->le_recherche_client, &QLineEdit::textChanged, this, &Client::onRechercheTextChanged);
    connect(ui->btn_tri_client, &QPushButton::clicked, this, &Client::onBtnTriClicked);
    connect(ui->btn_pdf_client, &QPushButton::clicked, this, &Client::onBtnPdfClicked);

}


void Client::rafraichirAffichage() {
    QString sql = "SELECT ID_CLIENT, NOM_CLIENT, VILLE, CODE_POSTAL, TO_CHAR(NUM_TEL), PDG, ADRESSE FROM CLIENT";
    if (!currentFilter.isEmpty()) {
        sql += " WHERE " + currentFilter;
    }
    if (!currentOrder.isEmpty()) {
        sql += " ORDER BY " + currentOrder;
    }
    model->setQuery(sql, QSqlDatabase::database());

    model->setHeaderData(0, Qt::Horizontal, "ID Client");
    model->setHeaderData(1, Qt::Horizontal, "Nom Client");
    model->setHeaderData(2, Qt::Horizontal, "Ville");
    model->setHeaderData(3, Qt::Horizontal, "Code Postal");
    model->setHeaderData(4, Qt::Horizontal, "Num Tel");
    model->setHeaderData(5, Qt::Horizontal, "Responsable");
    model->setHeaderData(6, Qt::Horizontal, "Adresse");

    if (model->lastError().isValid()) {
        qDebug() << "Erreur requête Client:" << model->lastError().text();
    }
}

bool Client::verifierSaisie() {
    QString id = ui->le_id_client->text();
    QString nom = ui->le_nom_client->text();
    QString tel = ui->le_tel_client->text();
    QString adr = ui->le_adresse_client->text();
    QString responsable = ui->le_responsable_client->text();
    QString cp = ui->dsb_codepostal_client->text();

    if(id.isEmpty() || nom.isEmpty() || tel.isEmpty() || adr.isEmpty() || responsable.isEmpty() || cp.isEmpty()) {
        QMessageBox::warning(nullptr, "Champs vides", "Veuillez remplir tous les champs du formulaire.");
        return false;
    }

    QRegularExpression rxNom("^[A-Za-zÀ-ÿ\\s]+$");
    if(!rxNom.match(nom).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur Nom", "Le nom ne doit contenir que des lettres.");
        return false;
    }

    QRegularExpression rxTel("^[0-9]{8}$");
    if(!rxTel.match(tel).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur Téléphone", "Le numéro de téléphone doit contenir 8 chiffres.");
        return false;
    }

    if(id.toInt() <= 0) {
        QMessageBox::warning(nullptr, "Erreur ID", "L'ID doit être un nombre positif.");
        return false;
    }

    QRegularExpression rxCP("^[0-9]+$");
    if(!rxCP.match(cp).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur Code Postal", "Le code postal doit être numérique.");
        return false;
    }

    return true;
}

void Client::onBtnAjouterClicked() {
    if(!verifierSaisie()) return;

    // 1. ARRÊTER LE TIMER IMMÉDIATEMENT
    if (refreshTimer) refreshTimer->stop();

    QSqlQuery query;
    query.prepare("INSERT INTO CLIENT (ID_CLIENT, NOM_CLIENT, VILLE, CODE_POSTAL, NUM_TEL, PDG, ADRESSE, POINT_DE_FIDELITE) "
                  "VALUES (:id, :nom, :ville, :cp, :tel, :pdg, :adr, 0)");

    query.bindValue(":id", ui->le_id_client->text().toInt());
    query.bindValue(":nom", ui->le_nom_client->text());
    query.bindValue(":ville", ui->cb_ville_client->currentText());
    query.bindValue(":cp", ui->dsb_codepostal_client->text());
    query.bindValue(":tel", ui->le_tel_client->text());
    query.bindValue(":pdg", ui->le_responsable_client->text());
    query.bindValue(":adr", ui->le_adresse_client->text());

    if(query.exec()) {
        // 2. BLOQUER LES SIGNAUX POUR ÉVITER LES CRASHES LORS DU NETTOYAGE
        ui->le_id_client->blockSignals(true);
        ui->le_nom_client->blockSignals(true);
        ui->le_tel_client->blockSignals(true);
        ui->le_adresse_client->blockSignals(true);
        ui->le_responsable_client->blockSignals(true);
        ui->dsb_codepostal_client->blockSignals(true);

        // 3. VIDER LES CHAMPS
        ui->le_id_client->clear();
        ui->le_nom_client->clear();
        ui->le_tel_client->clear();
        ui->le_adresse_client->clear();
        ui->le_responsable_client->clear();
        ui->dsb_codepostal_client->clear();

        // 4. DÉBLOQUER LES SIGNAUX
        ui->le_id_client->blockSignals(false);
        ui->le_nom_client->blockSignals(false);
        ui->le_tel_client->blockSignals(false);
        ui->le_adresse_client->blockSignals(false);
        ui->le_responsable_client->blockSignals(false);
        ui->dsb_codepostal_client->blockSignals(false);

        // 5. RAFRAICHIR LES DONNÉES ET LES STATISTIQUES
        rafraichirAffichage();
        rafraichirStats();
        rafraichirRendement();
        
        // 6. MESSAGE DE RÉUSSITE SANS PARENT PRÉCIS (Plus stable)
        QMessageBox::information(nullptr, "Succès", "Client ajouté avec succès !");
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Erreur lors de l'ajout :\n" + query.lastError().text());
    }

    // 7. REDÉMARRER LE TIMER
    if (refreshTimer) refreshTimer->start(5000);
}

void Client::onBtnModifierClicked() {
    if(!verifierSaisie()) return;

    if (selectedClientId == -1) {
        QMessageBox::warning(ui->tab_clients->window(), "Sélection requise", "Veuillez sélectionner un client dans le tableau avant de modifier.");
        return;
    }

    if (refreshTimer) refreshTimer->stop();

    int newId = ui->le_id_client->text().toInt();
    QSqlDatabase db = QSqlDatabase::database();

    if (!db.transaction()) {
        QMessageBox::critical(ui->tab_clients->window(), "Erreur", "Impossible de démarrer la transaction :\n" + db.lastError().text());
        if (refreshTimer) refreshTimer->start(5000);
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE CLIENT SET ID_CLIENT=:newId, NOM_CLIENT=:nom, VILLE=:ville, CODE_POSTAL=:cp, NUM_TEL=:tel, PDG=:pdg, ADRESSE=:adr WHERE ID_CLIENT=:oldId");

    query.bindValue(":newId", newId);
    query.bindValue(":oldId", selectedClientId);
    query.bindValue(":nom", ui->le_nom_client->text());
    query.bindValue(":ville", ui->cb_ville_client->currentText());
    query.bindValue(":cp", ui->dsb_codepostal_client->text());
    query.bindValue(":tel", ui->le_tel_client->text());
    query.bindValue(":pdg", ui->le_responsable_client->text());
    query.bindValue(":adr", ui->le_adresse_client->text());

    if(!query.exec()) {
        db.rollback();
        QMessageBox::critical(ui->tab_clients->window(), "Erreur", "Erreur lors de la modification :\n" + query.lastError().text());
        if (refreshTimer) refreshTimer->start(5000);
        return;
    }

    if (!reaffecterIdClientDansRelations(selectedClientId, newId)) {
        db.rollback();
        if (refreshTimer) refreshTimer->start(5000);
        return;
    }

    if (!db.commit()) {
        QMessageBox::critical(ui->tab_clients->window(), "Erreur", "Impossible de valider la transaction :\n" + db.lastError().text());
        if (refreshTimer) refreshTimer->start(5000);
        return;
    }

    selectedClientId = newId;
    
    this->blockSignals(true);
    rafraichirAffichage();
    rafraichirStats();
    rafraichirRendement();
    this->blockSignals(false);

    QMessageBox::information(ui->tab_clients->window(), "Succès", "Client modifié avec succès.");
    
    if (refreshTimer) refreshTimer->start(5000);
}

void Client::onBtnSupprimerClicked() {
    int row = ui->tab_clients->currentIndex().row();
    if(row < 0) {
        QMessageBox::warning(ui->tab_clients->window(), "Sélection", "Veuillez sélectionner un client dans le tableau.");
        return;
    }

    if (refreshTimer) refreshTimer->stop();

    int id = model->data(model->index(row, 0)).toInt();

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CONTRAT WHERE ID_CLIENT = :id");
    checkQuery.bindValue(":id", id);
    checkQuery.exec();
    checkQuery.next();
    int count = checkQuery.value(0).toInt();

    QString msg = "Voulez-vous supprimer ce client ?";
    if (count > 0) {
        msg = QString("Ce client est lié à %1 contrat(s).\n"
                      "Si vous le supprimez, ces contrats deviendront 'non-assignés'.\n\n"
                      "Confirmer la suppression ?").arg(count);
    }

    if (QMessageBox::question(ui->tab_clients->window(), "Suppression", msg, QMessageBox::Yes|QMessageBox::No) != QMessageBox::Yes) {
        if (refreshTimer) refreshTimer->start(5000);
        return;
    }

    if (count > 0) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE CONTRAT SET ID_CLIENT = NULL WHERE ID_CLIENT = :id");
        updateQuery.bindValue(":id", id);
        if (!updateQuery.exec()) {
            QMessageBox::critical(ui->tab_clients->window(), "Erreur", "Impossible de détacher les contrats : " + updateQuery.lastError().text());
            if (refreshTimer) refreshTimer->start(5000);
            return;
        }
    }

    QSqlQuery query;
    query.prepare("DELETE FROM CLIENT WHERE ID_CLIENT = :id");
    query.bindValue(":id", id);
    if(query.exec()) {
        this->blockSignals(true);
        rafraichirAffichage();
        rafraichirStats();
        rafraichirRendement();
        this->blockSignals(false);
        QMessageBox::information(ui->tab_clients->window(), "Succès", "Client supprimé avec succès.");
    } else {
         QMessageBox::critical(ui->tab_clients->window(), "Erreur", "Erreur lors de la suppression :\n" + query.lastError().text());
    }

    if (refreshTimer) refreshTimer->start(5000);
}

void Client::onTableClicked(const QModelIndex &index) {
    int r = index.row();
    selectedClientId = model->data(model->index(r, 0)).toInt();
    ui->le_id_client->setText(model->data(model->index(r, 0)).toString());
    ui->le_nom_client->setText(model->data(model->index(r, 1)).toString());
    ui->cb_ville_client->setCurrentText(model->data(model->index(r, 2)).toString());
    ui->dsb_codepostal_client->setText(model->data(model->index(r, 3)).toString());
    ui->le_tel_client->setText(model->data(model->index(r, 4)).toString());
    ui->le_responsable_client->setText(model->data(model->index(r, 5)).toString());
    ui->le_adresse_client->setText(model->data(model->index(r, 6)).toString());
}

void Client::onRechercheTextChanged(const QString &text) {
    if (text.isEmpty()) {
        currentFilter.clear();
    } else {
        // Recherche sur ID_CLIENT (nombre), NOM_CLIENT (string) et PDG (Responsable - string)
        currentFilter = QString("(TO_CHAR(ID_CLIENT) LIKE '%%1%' OR "
                                "UPPER(NOM_CLIENT) LIKE UPPER('%%1%') OR "
                                "UPPER(PDG) LIKE UPPER('%%1%'))").arg(text);
    }
    rafraichirAffichage();
}

void Client::onBtnPdfClicked() {
    QString fileName = QFileDialog::getSaveFileName(ui->tab_clients->window(), "Exporter en PDF", "Liste_Clients.pdf", "*.pdf");
    if (fileName.isEmpty()) return;

    if (refreshTimer) refreshTimer->stop();

    // 1. Récupérer les données
    QSqlQuery query("SELECT ID_CLIENT, NOM_CLIENT, VILLE, CODE_POSTAL, TO_CHAR(NUM_TEL), PDG, ADRESSE FROM CLIENT ORDER BY ID_CLIENT");
    
    QStringList headers;
    headers << "ID" << "Nom" << "Ville" << "CP" << "Tel" << "PDG" << "Adresse";
    
    QVector<QStringList> tableData;
    tableData.append(headers);
    
    while (query.next()) {
        QStringList row;
        for (int i = 0; i < 7; ++i) row << query.value(i).toString();
        tableData.append(row);
    }

    // 2. Configuration PDF
    QPdfWriter pdfWriter(fileName);
    QPageLayout layout(QPageSize(QPageSize::A4), QPageLayout::Landscape, QMarginsF(15, 15, 15, 15), QPageLayout::Millimeter);
    pdfWriter.setPageLayout(layout);

    QPainter painter(&pdfWriter);
    int pageWidth = pdfWriter.width();
    int pageHeight = pdfWriter.height();
    int marginLeft = 200;
    int currentY = 300;


    // En-tête
    painter.setPen(QColor(46, 125, 50));
    painter.setFont(QFont("Arial", 22, QFont::Bold));
    painter.drawText(marginLeft, currentY + 500, "GESTION DES CLIENTS - ECOCYCLE");

    currentY += 1200;
    marginLeft = 200;
    painter.setPen(QColor(100, 100, 100));
    painter.drawLine(marginLeft, currentY, pageWidth - marginLeft, currentY);

    // Dimension du tableau
    currentY += 400;
    int colCount = headers.size();
    int rowHeight = 400;
    int colWidth = (pageWidth - 2 * marginLeft) / colCount;

    // En-tête du tableau
    painter.setPen(Qt::white);
    painter.setFont(QFont("Arial", 10, QFont::Bold));
    painter.fillRect(marginLeft, currentY, pageWidth - 2 * marginLeft, rowHeight, QColor(46, 125, 50));

    for (int col = 0; col < colCount; col++) {
        painter.drawText(marginLeft + col * colWidth + 50, currentY + 50, colWidth - 100, rowHeight - 100, 
                        Qt::AlignCenter | Qt::TextWordWrap, headers[col]);
    }

    currentY += rowHeight;

    // Données
    painter.setFont(QFont("Arial", 9));
    bool alternateColor = false;
    int pageNumber = 1;

    for (int row = 1; row < tableData.size(); row++) {
        if (currentY + rowHeight > pageHeight - 600) {
            // Footer avant nouvelle page
            painter.setPen(Qt::gray);
            painter.setFont(QFont("Arial", 8));
            painter.drawText(pageWidth - 1500, pageHeight - 300, QString("Page %1").arg(pageNumber));
            
            pdfWriter.newPage();
            currentY = 400;
            pageNumber++;
            
            // Header tableau
            painter.fillRect(marginLeft, currentY, pageWidth - 2 * marginLeft, rowHeight, QColor(46, 125, 50));
            painter.setPen(Qt::white);
            painter.setFont(QFont("Arial", 10, QFont::Bold));
            for (int col = 0; col < colCount; col++) {
                painter.drawText(marginLeft + col * colWidth + 50, currentY + 50, colWidth - 100, rowHeight - 100, 
                                Qt::AlignCenter | Qt::TextWordWrap, headers[col]);
            }
            currentY += rowHeight;
            painter.setFont(QFont("Arial", 9));
        }

        if (alternateColor) {
            painter.fillRect(marginLeft, currentY, pageWidth - 2 * marginLeft, rowHeight, QColor(240, 245, 240));
        }

        painter.setPen(QColor(200, 200, 200));
        painter.drawLine(marginLeft, currentY + rowHeight, pageWidth - marginLeft, currentY + rowHeight);

        painter.setPen(Qt::black);
        for (int col = 0; col < colCount; col++) {
            painter.drawText(marginLeft + col * colWidth + 50, currentY + 50, colWidth - 100, rowHeight - 100,
                            Qt::AlignCenter | Qt::TextWordWrap, tableData[row][col]);
        }

        currentY += rowHeight;
        alternateColor = !alternateColor;
    }

    // Dernier footer
    painter.setPen(Qt::gray);
    painter.setFont(QFont("Arial", 8));
    painter.drawText(pageWidth - 1500, pageHeight - 300, QString("Page %1").arg(pageNumber));
    
    painter.setPen(QColor(100, 100, 100));
    painter.drawText(marginLeft, pageHeight - 300, pageWidth - 2 * marginLeft, 200,
                    Qt::AlignCenter, "Document Officiel EcoCycleApp - 2026");

    painter.end();
    QMessageBox::information(ui->tab_clients->window(), "Succès", "Le fichier PDF Clients a été généré !");

    if (refreshTimer) refreshTimer->start(5000);
}

void Client::rafraichirStats() {
    QSqlQuery query("SELECT VILLE, COUNT(*) FROM CLIENT GROUP BY VILLE");

    QPieSeries *series = new QPieSeries();
    while (query.next()) {
        QString ville = query.value(0).toString();
        int count = query.value(1).toInt();
        series->append(ville + " (" + QString::number(count) + ")", count);
    }

    if (series->slices().isEmpty()) {
        // Optionnel : afficher un message si pas de données
        return;
    }

    // Personnalisation : rendre une part "explosée" si besoin ou ajouter des labels
    for (QPieSlice *slice : series->slices()) {
        slice->setLabelVisible(true);
        slice->setExploded(false);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition Clients / Ville"); // Titre plus court
    chart->legend()->setAlignment(Qt::AlignBottom); // Légende en bas pour gagner de la place en largeur
    chart->setMargins(QMargins(5, 5, 5, 5)); // Réduire les marges du graphique
    chart->setAnimationOptions(QChart::AllAnimations);

    if (chartViewStats == nullptr) {
        chartViewStats = new QChartView(chart);
        chartViewStats->setRenderHint(QPainter::Antialiasing);
        
        // Ajuster la taille minimale pour s'assurer que tout soit visible
        chartViewStats->setMinimumHeight(300); 

        // Intégration dans gb_stats_client
        if (ui->gb_stats_client->layout() == nullptr) {
            QVBoxLayout *layout = new QVBoxLayout(ui->gb_stats_client);
            layout->setContentsMargins(5, 5, 5, 5); // Réduire les marges
            ui->gb_stats_client->setLayout(layout);
        } else {
            // Supprimer tout contenu précédent pour éviter les doublons visuels
            QLayoutItem *child;
            while ((child = ui->gb_stats_client->layout()->takeAt(0)) != nullptr) {
                if (child->widget()) delete child->widget();
                delete child;
            }
        }
        ui->gb_stats_client->layout()->addWidget(chartViewStats);
    } else {
        // Mettre à jour le graphique existant sans recréer le widget
        QChart *oldChart = chartViewStats->chart();
        chartViewStats->setChart(chart);
        if (oldChart) delete oldChart;
    }
}

void Client::rafraichirRendement() {
    // Calculer le rendement dynamiquement :
    // - 2 points par produit avec statut 'Vendu' (insensible à la casse)
    // - 1 point par contrat existant
    QString sql = 
        "SELECT C.NOM_CLIENT, C.ID_CLIENT, C.NUM_TEL, "
        "((SELECT COUNT(*) FROM PRODUIT P WHERE P.ID_CLIENT = C.ID_CLIENT AND UPPER(P.STATUT) = 'VENDU') * 2 + "
        " (SELECT COUNT(*) FROM CONTRAT CT WHERE CT.ID_CLIENT = C.ID_CLIENT)) as SCORE "
        "FROM CLIENT C "
        "ORDER BY SCORE DESC";

    rendementModel->setQuery(sql, QSqlDatabase::database());
    rendementModel->setHeaderData(0, Qt::Horizontal, "Nom Client");
    rendementModel->setHeaderData(1, Qt::Horizontal, "ID Client");
    rendementModel->setHeaderData(3, Qt::Horizontal, "Points Fidélité");
    
    // Cacher le numéro de téléphone du tableau de rendement car il sert à la vérification
    ui->tab_clients_4->setColumnHidden(2, true);

    // Vérification des points pour réinitialisation
    for (int i = 0; i < rendementModel->rowCount(); ++i) {
        int clientId = rendementModel->data(rendementModel->index(i, 1)).toInt();
        QString phone = rendementModel->data(rendementModel->index(i, 2)).toString();
        int score = rendementModel->data(rendementModel->index(i, 3)).toInt();
        
        if (score >= 10) {
            checkAndResetFidelite(clientId, score, phone);
        }
    }

    if (rendementModel->lastError().isValid()) {
        qDebug() << "Erreur rendement Client:" << rendementModel->lastError().text();
    }
}

void Client::checkAndResetFidelite(int clientId, int currentPoints, const QString &phone) {
    // Récupérer le nom du client pour le message personnalisé
    QString nomClient = "Client";
    QSqlQuery nameQuery;
    nameQuery.prepare("SELECT NOM_CLIENT FROM CLIENT WHERE ID_CLIENT = :id");
    nameQuery.bindValue(":id", clientId);
    if (nameQuery.exec() && nameQuery.next()) {
        nomClient = nameQuery.value(0).toString();
    }

    // Message personnalisé demandé
    QString msg = QString("%1 felicitation vous avez ganier 15% de reduction pour la prochaine reduction").arg(nomClient);
    sendTwilioSms(phone, msg);
    
    // Réinitialisation des points à 0 dans la base de données
    // Comme les points sont calculés via les tables PRODUIT et CONTRAT, 
    // on marque les enregistrements actuels comme "traités/récompensés" ou on nettoie l'historique de fidélité.
    // Selon votre demande "remet les points à 0", nous allons mettre à jour une colonne POINT_DE_FIDELITE si elle existe 
    // ou archiver les liens actuels. 
    
    QSqlQuery resetQuery;
    // Approche sécurisée : On décrémente ou on met un flag. 
    // Si vous avez une colonne POINT_DE_FIDELITE dans la table CLIENT :
    resetQuery.prepare("UPDATE CLIENT SET POINT_DE_FIDELITE = 0 WHERE ID_CLIENT = :id");
    resetQuery.bindValue(":id", clientId);
    if (!resetQuery.exec()) {
        qDebug() << "Erreur réinitialisation points:" << resetQuery.lastError().text();
    }

    // Pour que le calcul dynamique (COUNT) ne redonne pas 10 immédiatement, 
    // il faudrait normalement marquer les produits comme 'RECOMPENSE'.
    // Voici une requête pour marquer les produits vendus comme déjà comptabilisés :
    QSqlQuery prodQuery;
    prodQuery.prepare("UPDATE PRODUIT SET STATUT = 'VENDU_RECOMPENSE' WHERE ID_CLIENT = :id AND UPPER(STATUT) = 'VENDU'");
    prodQuery.bindValue(":id", clientId);
    prodQuery.exec();
}

void Client::sendTwilioSms(const QString &to, const QString &message) {
    QString accountSid = "AC8e97d35bfe809bf22bded53c97eabf3c";
    QString authToken = "72523db94a101c7cbbac982872d790fc";
    QString fromNumber = "+16624934698";

    // Formatter le numéro proprement
    QString formattedTo = to.trimmed();
    // Enlever les espaces ou tirets éventuels
    formattedTo.remove(" ");
    formattedTo.remove("-");

    // Twilio exige le format international E.164 (+216XXXXXXXX pour la Tunisie)
    if(!formattedTo.startsWith("+")) {
        if(formattedTo.length() == 8) {
            formattedTo = "+216" + formattedTo;
        } else {
            qDebug() << "Numéro mal formé pour Twilio:" << formattedTo;
            return;
        }
    }

    qDebug() << "Tentative d'envoi SMS à:" << formattedTo;

    QUrl url("https://api.twilio.com/2010-04-01/Accounts/" + accountSid + "/Messages.json");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QString auth = accountSid + ":" + authToken;
    request.setRawHeader("Authorization", "Basic " + auth.toUtf8().toBase64());

    QUrlQuery postData;
    postData.addQueryItem("From", fromNumber);
    postData.addQueryItem("To", formattedTo);
    postData.addQueryItem("Body", message);

    networkManager->post(request, postData.toString(QUrl::FullyEncoded).toUtf8());
}

void Client::onSmsReplyFinished(QNetworkReply *reply) {
    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "SMS Twilio envoyé avec succès: " << reply->readAll();
    } else {
        qDebug() << "Erreur SMS Twilio: " << reply->errorString() << reply->readAll();
    }
    reply->deleteLater();
}

void Client::onBtnTriClicked() {
    QString critere = ui->comboBox_client->currentText().trimmed();
    
    if (critere == "Nom Client") {
        currentOrder = "NOM_CLIENT ASC";
    } else if (critere == "Responsable") {
        currentOrder = "PDG ASC";
    } else if (critere == "ID Client") {
        currentOrder = "ID_CLIENT ASC";
    } else {
        // Fallback sur l'index de la comboBox au cas où les textes diffèrent légèrement
        int index = ui->comboBox_client->currentIndex();
        if (index == 0) currentOrder = "ID_CLIENT ASC";
        else if (index == 1) currentOrder = "NOM_CLIENT ASC";
        else if (index == 2) currentOrder = "PDG ASC";
    }
    
    rafraichirAffichage();
}

bool Client::reaffecterIdClientDansRelations(int oldId, int newId) {
    QSqlQuery q(QSqlDatabase::database());

    // Contrats
    q.prepare("UPDATE CONTRAT SET ID_CLIENT = :newId WHERE ID_CLIENT = :oldId");
    q.bindValue(":newId", newId);
    q.bindValue(":oldId", oldId);
    if (!q.exec()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Échec de la mise à jour des contrats :\n" + q.lastError().text());
        return false;
    }

    // Produits
    q.prepare("UPDATE PRODUIT SET ID_CLIENT = :newId WHERE ID_CLIENT = :oldId");
    q.bindValue(":newId", newId);
    q.bindValue(":oldId", oldId);
    if (!q.exec()) {
        QMessageBox::critical(nullptr, "Erreur de base de données", "Échec de la mise à jour des produits :\n" + q.lastError().text());
        return false;
    }

    return true;
}
