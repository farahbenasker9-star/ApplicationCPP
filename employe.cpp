#include "employe.h"
#include "ui_mainwindow.h"   // Full definition needed in the .cpp
#include "connection.h"      // For database connection

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>
#include <QDate>
#include <QRegularExpression>
#include <QIntValidator>
#include <QDoubleValidator>
#include <QPrinter>
#include <QTextDocument>
#include <QFileDialog>
#include <QDateTime>
#include <QPageSize>

// ==================== CONSTRUCTOR ====================

Employe::Employe(Ui::MainWindow *ui, QObject *parent)
    : QObject(parent), ui(ui), employeModel(nullptr)
{
    // Ajout du contrôle de saisie pour l'âge (ex: entre 18 et 100 ans)
    ui->le_age->setValidator(new QIntValidator(18, 100, this));
    
    // Ajout du contrôle de saisie pour le salaire (uniquement des nombres avec décimales)
    ui->le_salaire->setValidator(new QDoubleValidator(0.0, 999999.99, 2, this));

    // Load the table data
    afficherEmployes();

    // Manually connect UI signals to this object's slots
    // (replaces Qt's auto-connect which only works for slots on the form's own class)
    connect(ui->tab_employes,   &QTableView::clicked,
            this, &Employe::onTabEmployesClicked);

    connect(ui->btn_ajouter,    &QPushButton::clicked,
            this, &Employe::onBtnAjouterClicked);

    connect(ui->btn_modifier,   &QPushButton::clicked,
            this, &Employe::onBtnModifierClicked);

    connect(ui->btn_supprimer,  &QPushButton::clicked,
            this, &Employe::onBtnSupprimerClicked);

    connect(ui->le_recherche,   &QLineEdit::textChanged,
            this, &Employe::onLeRechercheTextChanged);

    connect(ui->btn_tri,        &QPushButton::clicked,
            this, &Employe::onBtnTriClicked);
    connect(ui->btn_pdf,        &QPushButton::clicked,
            this, &Employe::onBtnPdfClicked);

    // Actualiser les statistiques et le tableau
    connect(ui->btn_refresh,    &QPushButton::clicked,
            this, [this]() {
                employeModel->select(); // Actualise le tableau
                updateStats();          // Actualise les graphiques
            });

    // Initialiser les widgets statistiques
    setupStatsUI();
    updateStats();
}

// ==================== DISPLAY / LOAD ====================

void Employe::afficherEmployes()
{
    QSqlDatabase db = QSqlDatabase::database();

    // Insert dummy data if the table is empty (for testing)
    QSqlQuery query(db);
    query.exec("SELECT COUNT(*) FROM EMPLOYE");
    if (query.next() && query.value(0).toInt() == 0) {
        qDebug() << "Table EMPLOYE is empty. Inserting dummy data...";

        query.exec("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE, AGE, GENRE) "
                   "VALUES (11111111, 'Ben Ali', 'Ahmed', 'Trieur', 900.50, TO_DATE('2023-01-15', 'YYYY-MM-DD'), 'B-001', 30, 'Homme')");

        query.exec("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE, AGE, GENRE) "
                   "VALUES (22222222, 'Trabelsi', 'Sami', 'Chauffeur', 1000.00, TO_DATE('2023-06-20', 'YYYY-MM-DD'), 'B-002', 28, 'Homme')");

        query.exec("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE, AGE, GENRE) "
                   "VALUES (33333333, 'Mansour', 'Sarra', 'Trieur', 900.75, TO_DATE('2024-02-10', 'YYYY-MM-DD'), 'B-003', 25, 'Femme')");

        db.commit();
    }

    employeModel = new QSqlTableModel(this, db);
    employeModel->setTable("EMPLOYE");
    employeModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    employeModel->select();

    // Human-readable column headers
    employeModel->setHeaderData(0, Qt::Horizontal, QObject::tr("CIN"));
    employeModel->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    employeModel->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    employeModel->setHeaderData(3, Qt::Horizontal, QObject::tr("Poste"));
    employeModel->setHeaderData(4, Qt::Horizontal, QObject::tr("Salaire (TND)"));
    employeModel->setHeaderData(5, Qt::Horizontal, QObject::tr("Date d'embauche"));
    employeModel->setHeaderData(6, Qt::Horizontal, QObject::tr("Badge"));
    employeModel->setHeaderData(7, Qt::Horizontal, QObject::tr("Âge"));
    employeModel->setHeaderData(8, Qt::Horizontal, QObject::tr("Genre"));
    employeModel->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Badge"));

    ui->tab_employes->setModel(employeModel);
    ui->tab_employes->setAlternatingRowColors(false);
    ui->tab_employes->resizeColumnsToContents();
}

// ==================== SHARED VALIDATION ====================

bool Employe::validerFormulaire(const QString &cin, const QString &nom,
                                const QString &prenom, const QString &idBadge,
                                double salaire)
{
    QRegularExpression cinRegex("^[0-9]{8}$");
    if (!cinRegex.match(cin).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de saisie",
                             "Le CIN doit contenir exactement 8 chiffres.");
        return false;
    }

    QRegularExpression nameRegex("^[A-Za-zÀ-ÿ\\s]+$");
    if (nom.isEmpty() || nom.length() > 50 || !nameRegex.match(nom).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de saisie",
                             "Le nom est invalide (lettres uniquement, max 50 caractères).");
        return false;
    }
    if (prenom.isEmpty() || prenom.length() > 50 || !nameRegex.match(prenom).hasMatch()) {
        QMessageBox::warning(nullptr, "Erreur de saisie",
                             "Le prénom est invalide (lettres uniquement, max 50 caractères).");
        return false;
    }

    if (idBadge.isEmpty() || idBadge.length() > 20) {
        QMessageBox::warning(nullptr, "Erreur de saisie",
                             "L'ID Badge ne peut pas être vide et doit faire max 20 caractères.");
        return false;
    }

    if (salaire <= 0) {
        QMessageBox::warning(nullptr, "Erreur de saisie",
                             "Le salaire doit être supérieur à 0.");
        return false;
    }

    return true;
}

// ==================== TABLE CLICK (populate form) ====================

void Employe::onTabEmployesClicked(const QModelIndex &index)
{
    int row = index.row();

    ui->le_cin->setText(employeModel->data(employeModel->index(row, 0)).toString());
    ui->le_nom->setText(employeModel->data(employeModel->index(row, 1)).toString());
    ui->le_prenom->setText(employeModel->data(employeModel->index(row, 2)).toString());
    ui->cb_poste->setCurrentText(employeModel->data(employeModel->index(row, 3)).toString());
    ui->le_salaire->setText(employeModel->data(employeModel->index(row, 4)).toString());
    ui->de_embauche->setDate(employeModel->data(employeModel->index(row, 5)).toDate());
    ui->le_badge->setText(employeModel->data(employeModel->index(row, 6)).toString());
    ui->le_age->setText(employeModel->data(employeModel->index(row, 7)).toString());
    ui->cb_genre->setCurrentText(employeModel->data(employeModel->index(row, 8)).toString());
}

// ==================== ADD ====================

void Employe::onBtnAjouterClicked()
{
    QString cin      = ui->le_cin->text();
    QString nom      = ui->le_nom->text();
    QString prenom   = ui->le_prenom->text();
    QString poste    = ui->cb_poste->currentText();
    double  salaire  = ui->le_salaire->text().replace(",", ".").toDouble();
    QDate   dateEmb  = ui->de_embauche->date();
    QString idBadge  = ui->le_badge->text();
    int     age      = ui->le_age->text().toInt();
    QString genre    = ui->cb_genre->currentText();

    if (!validerFormulaire(cin, nom, prenom, idBadge, salaire))
        return;

    QSqlQuery query;
    query.prepare("INSERT INTO EMPLOYE (CIN, NOM, PRENOM, POSTE, SALAIRE, DATE_EMBAUCHE, ID_BADGE, AGE, GENRE) "
                  "VALUES (:cin, :nom, :prenom, :poste, :salaire, :date_embauche, :id_badge, :age, :genre)");
    query.bindValue(":cin",          cin);
    query.bindValue(":nom",          nom);
    query.bindValue(":prenom",       prenom);
    query.bindValue(":poste",        poste);
    query.bindValue(":salaire",      salaire);
    query.bindValue(":date_embauche", dateEmb);
    query.bindValue(":id_badge",     idBadge);
    query.bindValue(":age",          age);
    query.bindValue(":genre",        genre);

    if (query.exec()) {
        QMessageBox::information(nullptr, "Succès", "Employé ajouté avec succès.");
        employeModel->select();
    } else {
        QMessageBox::critical(nullptr, "Erreur",
                              "Erreur lors de l'ajout: " + query.lastError().text());
    }
}

// ==================== EDIT ====================

void Employe::onBtnModifierClicked()
{
    QString cin      = ui->le_cin->text();
    QString nom      = ui->le_nom->text();
    QString prenom   = ui->le_prenom->text();
    QString poste    = ui->cb_poste->currentText();
    double  salaire  = ui->le_salaire->text().replace(",", ".").toDouble();
    QDate   dateEmb  = ui->de_embauche->date();
    QString idBadge  = ui->le_badge->text();
    int     age      = ui->le_age->text().toInt();
    QString genre    = ui->cb_genre->currentText();

    if (!validerFormulaire(cin, nom, prenom, idBadge, salaire))
        return;

    QSqlQuery query;
    query.prepare("UPDATE EMPLOYE SET NOM = :nom, PRENOM = :prenom, POSTE = :poste, "
                  "SALAIRE = :salaire, DATE_EMBAUCHE = :date_embauche, ID_BADGE = :id_badge, "
                  "AGE = :age, GENRE = :genre "
                  "WHERE CIN = :cin");
    query.bindValue(":nom",          nom);
    query.bindValue(":prenom",       prenom);
    query.bindValue(":poste",        poste);
    query.bindValue(":salaire",      salaire);
    query.bindValue(":date_embauche", dateEmb);
    query.bindValue(":id_badge",     idBadge);
    query.bindValue(":age",          age);
    query.bindValue(":genre",        genre);
    query.bindValue(":cin",          cin);

    if (query.exec()) {
        QMessageBox::information(nullptr, "Succès", "Employé modifié avec succès.");
        employeModel->select();
    } else {
        QMessageBox::critical(nullptr, "Erreur",
                              "Erreur lors de la modification: " + query.lastError().text());
    }
}

// ==================== DELETE ====================

void Employe::onBtnSupprimerClicked()
{
    QString cin = ui->le_cin->text();

    if (cin.isEmpty()) {
        QMessageBox::warning(nullptr, "Avertissement",
                             "Veuillez sélectionner un employé à supprimer.");
        return;
    }

    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM CONTRAT WHERE CIN = :cin");
    checkQuery.bindValue(":cin", cin);
    checkQuery.exec();
    checkQuery.next();
    int count = checkQuery.value(0).toInt();

    QString msg = "Voulez-vous vraiment supprimer cet employé ?";
    if (count > 0) {
        msg = QString("Cet employé est lié à %1 contrat(s).\n"
                      "Si vous le supprimez, ces contrats deviendront 'non-assignés'.\n\n"
                      "Confirmer la suppression ?").arg(count);
    }

    if (QMessageBox::question(nullptr, "Confirmation de suppression", msg, 
                              QMessageBox::Yes|QMessageBox::No) != QMessageBox::Yes) {
        return;
    }

    if (count > 0) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE CONTRAT SET CIN = NULL WHERE CIN = :cin");
        updateQuery.bindValue(":cin", cin);
        if (!updateQuery.exec()) {
            QMessageBox::critical(nullptr, "Erreur", "Impossible de détacher les contrats : " + updateQuery.lastError().text());
            return;
        }
    }

    QSqlQuery query;
    query.prepare("DELETE FROM EMPLOYE WHERE CIN = :cin");
    query.bindValue(":cin", cin);

    if (query.exec()) {
        QMessageBox::information(nullptr, "Succès", "Employé supprimé avec succès.");
        // Nettoyage etc...

        // Clear the form
        ui->le_cin->clear();
        ui->le_nom->clear();
        ui->le_prenom->clear();
        ui->cb_poste->setCurrentIndex(0);
        ui->le_salaire->clear();
        ui->de_embauche->setDate(QDate::currentDate());
        ui->le_badge->clear();
        ui->le_age->clear();
        ui->cb_genre->setCurrentIndex(0);

        employeModel->select();
    } else {
        QMessageBox::critical(nullptr, "Erreur",
                              "Erreur lors de la suppression: " + query.lastError().text());
    }
}

// ==================== SEARCH ====================

void Employe::onLeRechercheTextChanged(const QString &arg1)
{
    if (arg1.isEmpty()) {
        employeModel->setFilter("");
    } else {
        QString filter = QString(
            "(TO_CHAR(CIN) LIKE '%%1%' OR UPPER(NOM) LIKE UPPER('%%1%') OR UPPER(ID_BADGE) LIKE UPPER('%%1%'))"
        ).arg(arg1);
        employeModel->setFilter(filter);
    }
    employeModel->select();
}

// ==================== SORT ====================

void Employe::onBtnTriClicked()
{
    QString currentSort = ui->comboBox->currentText();

    if (currentSort == "CIN") {
        employeModel->setSort(0, Qt::AscendingOrder);
    } else if (currentSort == "Nom") {
        employeModel->setSort(1, Qt::AscendingOrder);
    } else if (currentSort == "ID Badge") {
        employeModel->setSort(6, Qt::AscendingOrder);
    }

    employeModel->select();
}

// ==================== PDF EXPORT ====================

void Employe::onBtnPdfClicked()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr, "Exporter la liste", "Liste_Employes.pdf", "PDF (*.pdf)");
    if (filePath.isEmpty()) return;

    QString html = R"(<html>
        <head>
            <style>
                body { font-family: 'Segoe UI', Arial, sans-serif; }
                h1 { color: #1B4332; text-align: center; }
                table { width: 100%; border-collapse: collapse; margin-top: 20px; }
                th { background-color: #1B4332; color: white; padding: 10px; text-align: left; }
                td { border-bottom: 1px solid #ddd; padding: 8px; color: #333; }
                tr:nth-child(even) { background-color: #f9fafb; }
            </style>
        </head>
        <body>
            <h1>Liste des Employés - EcoCycle</h1>
            <p style="text-align: right; color: #666;">Généré le : %1</p>
            <table>
                <tr>
                    <th>CIN</th><th>Nom</th><th>Prénom</th><th>Poste</th><th>Salaire</th><th>Embauche</th><th>Badge</th><th>Âge</th><th>Genre</th>
                </tr>
    )";

    html = html.arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));

    for (int i = 0; i < employeModel->rowCount(); ++i) {
        QString cin = employeModel->data(employeModel->index(i, 0)).toString();
        QString nom = employeModel->data(employeModel->index(i, 1)).toString();
        QString prenom = employeModel->data(employeModel->index(i, 2)).toString();
        QString poste = employeModel->data(employeModel->index(i, 3)).toString();
        QString salaire = employeModel->data(employeModel->index(i, 4)).toString();
        QString dateEmb = employeModel->data(employeModel->index(i, 5)).toDate().toString("dd/MM/yyyy");
        QString badge = employeModel->data(employeModel->index(i, 6)).toString();
        QString age = employeModel->data(employeModel->index(i, 7)).toString();
        QString genre = employeModel->data(employeModel->index(i, 8)).toString();

        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td><td>%7</td><td>%8</td><td>%9</td></tr>")
                .arg(cin, nom, prenom, poste, salaire, dateEmb, badge, age, genre);
    }

    html += "</table></body></html>";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);

    QMessageBox::information(nullptr, "Succès", "La liste des employés a été exportée en PDF avec succès.");
}


// ==================== STATISTIQUES ====================

void Employe::setupStatsUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->gb_stats);
    mainLayout->setSpacing(20);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    // --- 1. ZONE CAMEMBERTS (H/F et Postes) ---
    QHBoxLayout *pieLayout = new QHBoxLayout();

    chartViewGenre = new QChartView();
    chartViewGenre->setRenderHint(QPainter::Antialiasing);
    chartViewGenre->setMinimumHeight(300);

    chartViewPoste = new QChartView();
    chartViewPoste->setRenderHint(QPainter::Antialiasing);
    chartViewPoste->setMinimumHeight(300);

    pieLayout->addWidget(chartViewGenre);
    pieLayout->addWidget(chartViewPoste);
    mainLayout->addLayout(pieLayout);

    // --- 2. ZONE BARRES HORIZONTALES (Salaire moyen) ---
    chartViewSalaire = new QChartView();
    chartViewSalaire->setRenderHint(QPainter::Antialiasing);
    chartViewSalaire->setMinimumHeight(350);
    mainLayout->addWidget(chartViewSalaire);

    ui->gb_stats->setLayout(mainLayout);
}

void Employe::updateStats()
{
    QSqlQuery query;

    // 1. Camembert : Répartition H/F
    QChart *chartGenre = new QChart();
    QPieSeries *seriesGenre = new QPieSeries();

    if (query.exec("SELECT GENRE, COUNT(*) FROM EMPLOYE GROUP BY GENRE")) {
        while (query.next()) {
            QString genre = query.value(0).toString();
            if (genre.isEmpty()) genre = "Non défini";
            int count = query.value(1).toInt();

            QPieSlice *slice = seriesGenre->append(genre + " (" + QString::number(count) + ")", count);
            slice->setLabelVisible(true);

            if (genre.toUpper() == "HOMME" || genre.toUpper() == "M") {
                slice->setBrush(QColor("#3498db"));
            } else if (genre.toUpper() == "FEMME" || genre.toUpper() == "F") {
                slice->setBrush(QColor("#e74c3c"));
            } else {
                slice->setBrush(QColor("#95a5a6"));
            }
        }
    }
    chartGenre->addSeries(seriesGenre);
    chartGenre->setTitle("Répartition par Genre");
    chartGenre->setAnimationOptions(QChart::SeriesAnimations);
    chartViewGenre->setChart(chartGenre);

    // 3. Camembert : Répartition par Poste
    QChart *chartPoste = new QChart();
    QPieSeries *seriesPoste = new QPieSeries();

    if (query.exec("SELECT POSTE, COUNT(*) FROM EMPLOYE GROUP BY POSTE")) {
        while (query.next()) {
            QString poste = query.value(0).toString();
            if (poste.isEmpty()) continue;
            QPieSlice *slice = seriesPoste->append(poste, query.value(1).toInt());
            slice->setLabelVisible(true);
        }
    }
    chartPoste->addSeries(seriesPoste);
    chartPoste->setTitle("Répartition par Poste");
    chartPoste->setAnimationOptions(QChart::SeriesAnimations);
    chartViewPoste->setChart(chartPoste);

    // 4. Barres Horizontales : Salaire moyen par poste
    QChart *chartSalaire = new QChart();
    QHorizontalBarSeries *seriesSalaire = new QHorizontalBarSeries();
    QBarSet *setSalaire = new QBarSet("Salaire Moyen (TND)");
    setSalaire->setBrush(QColor("#1B4332"));

    QStringList categories;

    if (query.exec("SELECT POSTE, AVG(SALAIRE) FROM EMPLOYE GROUP BY POSTE ORDER BY AVG(SALAIRE) ASC")) {
        while (query.next()) {
            QString poste = query.value(0).toString();
            if (poste.isEmpty()) continue;
            categories << poste;
            *setSalaire << query.value(1).toDouble();
        }
    }

    seriesSalaire->append(setSalaire);
    chartSalaire->addSeries(seriesSalaire);
    chartSalaire->setTitle("Salaire Moyen par Poste");
    chartSalaire->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisY = new QBarCategoryAxis();
    axisY->append(categories);
    chartSalaire->addAxis(axisY, Qt::AlignLeft);
    seriesSalaire->attachAxis(axisY);

    QValueAxis *axisX = new QValueAxis();
    axisX->setLabelFormat("%.0f TND");
    chartSalaire->addAxis(axisX, Qt::AlignBottom);
    seriesSalaire->attachAxis(axisX);

    chartViewSalaire->setChart(chartSalaire);
}
