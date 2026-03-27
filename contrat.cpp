#include "contrat.h"
#include "ui_mainwindow.h"   // Full definition needed in the .cpp

#include <QPrinter>
#include <QTextDocument>
#include <QDateTime>
#include <QMessageBox>
#include <QFileDialog>
#include <QTableView>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QDoubleSpinBox>
#include <QDateEdit>
#include <QVBoxLayout>
#include <QSizePolicy>
#include <QFont>
#include <QColor>
#include <QMargins>
#include <QSqlDatabase>
#include <QPushButton>
#include <QPageSize>

#include <QtCharts/QChart>
#include <QtCharts/QPieSeries>
#include <QtCharts/QPieSlice>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

// ═══════════════════════════════════════════════════════════════════════════════
//  Classe Contrat  (DAO – données uniquement)
// ═══════════════════════════════════════════════════════════════════════════════

// ─── Constructeur par défaut ────────────────────────────────────────────────
Contrat::Contrat()
    : id_contrat(0), id_client(0), cin(0), objectif_achat_annuel(0.0), taux_remise_accorde(0.0)
{}

// ─── Constructeur complet (ID_CONTRAT est auto-incrémenté) ───────────────────
Contrat::Contrat(int id_client, int cin,
                 const QString &type_exclusivite, const QString &produits_concernes,
                 const QDate &date_debut, const QDate &date_fin,
                 double objectif_achat_annuel, double taux_remise_accorde,
                 const QString &statut_contrat, const QString &clause_penale)
    : id_contrat(0), id_client(id_client), cin(cin),
      type_exclusivite(type_exclusivite), produits_concernes(produits_concernes),
      date_debut(date_debut), date_fin(date_fin),
      objectif_achat_annuel(objectif_achat_annuel), taux_remise_accorde(taux_remise_accorde),
      statut_contrat(statut_contrat), clause_penale(clause_penale)
{}

// ─── Méthode AJOUTER ────────────────────────────────────────────────────────
bool Contrat::ajouter()
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO CONTRAT "
        "(ID_CLIENT, CIN, TYPE_EXCLUSIVITE, PRODUITS_CONCERNES, "
        " DATE_DEBUT, DATE_FIN, OBJECTIF_ACHAT_ANNUEL, TAUX_REMISE_ACCORDE, STATUT_CONTRAT, CLAUSE_PENALE) "
        "VALUES "
        "(:id_client, :cin, :type_ex, :prod_con, "
        " TO_DATE(:date_deb, 'DD/MM/YYYY'), TO_DATE(:date_fin, 'DD/MM/YYYY'), "
        " :ob_ach_an, :tau_rem_acc, :status, :clause)"
    );

    query.bindValue(":id_client",   id_client);
    query.bindValue(":cin",         cin);
    query.bindValue(":type_ex",     type_exclusivite);
    query.bindValue(":prod_con",    produits_concernes);
    query.bindValue(":date_deb",    date_debut.toString("dd/MM/yyyy"));
    query.bindValue(":date_fin",    date_fin.toString("dd/MM/yyyy"));
    query.bindValue(":ob_ach_an",   QString::number(objectif_achat_annuel, 'f', 2));
    query.bindValue(":tau_rem_acc", QString::number(taux_remise_accorde, 'f', 2));
    query.bindValue(":status",      statut_contrat);
    query.bindValue(":clause",      clause_penale);

    if (!query.exec()) {
        qDebug() << "Erreur ajouter contrat:" << query.lastError().text();
        return false;
    }

    qDebug() << "Contrat ajouté avec succès.";
    return true;
}

// ─── Méthode AFFICHER ────────────────────────────────────────────────────────
QSqlQueryModel * Contrat::afficher() const
{
    QSqlQueryModel * model = new QSqlQueryModel();

    model->setQuery("SELECT ID_CONTRAT, ID_CLIENT, CIN, TYPE_EXCLUSIVITE, PRODUITS_CONCERNES, "
                    "DATE_DEBUT, DATE_FIN, "
                    "OBJECTIF_ACHAT_ANNUEL, "
                    "TAUX_REMISE_ACCORDE, "
                    "STATUT_CONTRAT, CLAUSE_PENALE "
                    "FROM CONTRAT");

    model->setHeaderData(0,  Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1,  Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(2,  Qt::Horizontal, QObject::tr("CIN Employé"));
    model->setHeaderData(3,  Qt::Horizontal, QObject::tr("Type Exclusivité"));
    model->setHeaderData(4,  Qt::Horizontal, QObject::tr("Produits"));
    model->setHeaderData(5,  Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(6,  Qt::Horizontal, QObject::tr("Date Fin"));
    model->setHeaderData(7,  Qt::Horizontal, QObject::tr("Obj. Achat"));
    model->setHeaderData(8,  Qt::Horizontal, QObject::tr("Taux Rem."));
    model->setHeaderData(9,  Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Clause Pénale"));

    return model;
}

// ─── Export PDF ──────────────────────────────────────────────────────────────
bool Contrat::exporterPdf(const QString &filePath, QSqlQueryModel *model) const
{
    if (filePath.isEmpty()) {
        return false;
    }

    QSqlQueryModel *sourceModel = model;
    bool ownModel = false;

    if (!sourceModel) {
        sourceModel = afficher();
        ownModel = true;
    }

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
            <h1>Liste des Contrats - EcoCycle</h1>
            <p style="text-align: right; color: #666;">Généré le : %1</p>
            <table>
                <tr>
                    <th>ID</th><th>ID Client</th><th>CIN</th><th>Type</th><th>Produits</th><th>Date Début</th><th>Date Fin</th><th>Objectif</th><th>Taux</th><th>Statut</th><th>Clause</th>
                </tr>
    )";

    html = html.arg(QDateTime::currentDateTime().toString("dd/MM/yyyy HH:mm"));

    for (int i = 0; i < sourceModel->rowCount(); ++i) {
        QString id_contrat_s = sourceModel->data(sourceModel->index(i, 0)).toString();
        QString id_client_s  = sourceModel->data(sourceModel->index(i, 1)).toString();
        QString cin_s        = sourceModel->data(sourceModel->index(i, 2)).toString();
        QString type_s       = sourceModel->data(sourceModel->index(i, 3)).toString();
        QString produits_s   = sourceModel->data(sourceModel->index(i, 4)).toString();
        QString date_deb_s   = sourceModel->data(sourceModel->index(i, 5)).toDate().toString("dd/MM/yyyy");
        QString date_fin_s   = sourceModel->data(sourceModel->index(i, 6)).toDate().toString("dd/MM/yyyy");
        QString objectif_s   = sourceModel->data(sourceModel->index(i, 7)).toString();
        QString taux_s       = sourceModel->data(sourceModel->index(i, 8)).toString();
        QString statut_s     = sourceModel->data(sourceModel->index(i, 9)).toString();
        QString clause_s     = sourceModel->data(sourceModel->index(i, 10)).toString();

        html += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5</td><td>%6</td><td>%7</td><td>%8</td><td>%9</td><td>%10</td><td>%11</td></tr>")
                .arg(id_contrat_s, id_client_s, cin_s, type_s, produits_s, date_deb_s, date_fin_s, objectif_s, taux_s, statut_s, clause_s);
    }

    html += "</table></body></html>";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);
    printer.setPageSize(QPageSize(QPageSize::A4));

    QTextDocument doc;
    doc.setHtml(html);
    doc.print(&printer);

    if (ownModel) {
        delete sourceModel;
    }

    return true;
}

// ─── Méthode RECHERCHER ──────────────────────────────────────────────────────
QSqlQueryModel * Contrat::rechercher(const QString &texte) const
{
    QSqlQueryModel * model = new QSqlQueryModel();
    QString key = texte.trimmed();
    if (key.isEmpty()) {
        return afficher();
    }

    QString sql = QString(
        "SELECT ID_CONTRAT, ID_CLIENT, CIN, TYPE_EXCLUSIVITE, PRODUITS_CONCERNES, "
        "DATE_DEBUT, DATE_FIN, OBJECTIF_ACHAT_ANNUEL, TAUX_REMISE_ACCORDE, STATUT_CONTRAT, CLAUSE_PENALE "
        "FROM CONTRAT WHERE "
        "CAST(ID_CONTRAT AS VARCHAR2(50)) LIKE '%%1%' OR "
        "UPPER(TYPE_EXCLUSIVITE) LIKE UPPER('%%1%') OR "
        "UPPER(STATUT_CONTRAT) LIKE UPPER('%%1%')"
    ).arg(key);

    model->setQuery(sql);
    model->setHeaderData(0,  Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1,  Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(2,  Qt::Horizontal, QObject::tr("CIN Employé"));
    model->setHeaderData(3,  Qt::Horizontal, QObject::tr("Type Exclusivité"));
    model->setHeaderData(4,  Qt::Horizontal, QObject::tr("Produits"));
    model->setHeaderData(5,  Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(6,  Qt::Horizontal, QObject::tr("Date Fin"));
    model->setHeaderData(7,  Qt::Horizontal, QObject::tr("Obj. Achat"));
    model->setHeaderData(8,  Qt::Horizontal, QObject::tr("Taux Rem."));
    model->setHeaderData(9,  Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Clause Pénale"));

    return model;
}

// ─── Méthode SUPPRIMER ───────────────────────────────────────────────────────
bool Contrat::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM CONTRAT WHERE ID_CONTRAT = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        qDebug() << "Erreur suppression contrat:" << query.lastError().text();
        return false;
    }
    return true;
}

// ─── Méthode MODIFIER ────────────────────────────────────────────────────────
bool Contrat::modifier(int old_id)
{
    QSqlQuery query;
    query.prepare("UPDATE CONTRAT SET "
                  "ID_CONTRAT = :new_id, "
                  "ID_CLIENT = :id_client, "
                  "CIN = :cin, "
                  "TYPE_EXCLUSIVITE = :type_ex, "
                  "PRODUITS_CONCERNES = :prod_con, "
                  "DATE_DEBUT = TO_DATE(:date_deb, 'DD/MM/YYYY'), "
                  "DATE_FIN = TO_DATE(:date_fin, 'DD/MM/YYYY'), "
                  "OBJECTIF_ACHAT_ANNUEL = :ob_ach_an, "
                  "TAUX_REMISE_ACCORDE = :tau_rem_acc, "
                  "STATUT_CONTRAT = :status, "
                  "CLAUSE_PENALE = :clause "
                  "WHERE ID_CONTRAT = :old_id");

    query.bindValue(":new_id",      id_contrat);
    query.bindValue(":old_id",      old_id);
    query.bindValue(":id_client",   id_client);
    query.bindValue(":cin",         cin);
    query.bindValue(":type_ex",     type_exclusivite);
    query.bindValue(":prod_con",    produits_concernes);
    query.bindValue(":date_deb",    date_debut.toString("dd/MM/yyyy"));
    query.bindValue(":date_fin",    date_fin.toString("dd/MM/yyyy"));
    query.bindValue(":ob_ach_an",   QString::number(objectif_achat_annuel, 'f', 2));
    query.bindValue(":tau_rem_acc", QString::number(taux_remise_accorde, 'f', 2));
    query.bindValue(":status",      statut_contrat);
    query.bindValue(":clause",      clause_penale);

    if (!query.exec()) {
        qDebug() << "Erreur modification contrat:" << query.lastError().text();
        return false;
    }
    return true;
}

// ─── Méthodes de validation statiques ────────────────────────────────────────

bool Contrat::validerDateDebut(const QDate &date_debut) {
    QDate today = QDate::currentDate();
    return date_debut == today;
}

bool Contrat::validerDateFin(const QDate &date_fin, const QDate &date_debut) {
    return date_fin >= date_debut;
}

bool Contrat::validerDates(const QDate &date_debut, const QDate &date_fin) {
    return validerDateDebut(date_debut) && validerDateFin(date_fin, date_debut);
}

bool Contrat::validerID(int id) {
    return id > 0;
}

bool Contrat::validerFloats(double obj_ach_ann, double tau_rem_acc) {
    return (obj_ach_ann > 0) && (tau_rem_acc >= 0 && tau_rem_acc <= 100);
}

bool Contrat::validerDescription(const QString &clause_penale) {
    return clause_penale.trimmed().length() >= 5;
}


// ═══════════════════════════════════════════════════════════════════════════════
//  Classe ContratManager  (Contrôleur UI – même pattern qu'Employe)
// ═══════════════════════════════════════════════════════════════════════════════

// ─── Constructeur ────────────────────────────────────────────────────────────
ContratManager::ContratManager(Ui::MainWindow *ui, QObject *parent)
    : QObject(parent), ui(ui)
{
    // Affichage initial du tableau
    ui->tab_contrat_2->setModel(Contrat().afficher());
    populateComboBoxes();

    // ── Connexions CRUD ─────────────────────────────────────────────────────
    connect(ui->tab_contrat_2, &QTableView::clicked,
            this, &ContratManager::onTabClicked);
    connect(ui->tab_contrat_2, &QTableView::doubleClicked,
            this, &ContratManager::onTabDoubleClicked);

    connect(ui->ajouter_contrat,   &QPushButton::clicked, this, &ContratManager::onAjouterClicked);
    connect(ui->supprimer_contrat, &QPushButton::clicked, this, &ContratManager::onSupprimerClicked);
    connect(ui->modifier_contrat,  &QPushButton::clicked, this, &ContratManager::onModifierClicked);
    connect(ui->exporter_contrat,  &QPushButton::clicked, this, &ContratManager::onExporterClicked);
    connect(ui->tri2_contrat,      &QPushButton::clicked, this, &ContratManager::onTriClicked);

    // ── Editabilité & limites ────────────────────────────────────────────────
    ui->ID_Contrat->setReadOnly(false);
    ui->ID_Contrat->setEnabled(true);
    ui->obj_ach_ann->setMaximum(9999999.99);
    ui->tau_rem_acc->setMaximum(9999999.99);

    // ── Connexions validation en temps réel ──────────────────────────────────
    connect(ui->ID_Contrat,    &QLineEdit::textChanged,
            this, &ContratManager::validateID);
    connect(ui->date_debut,    &QDateEdit::dateChanged,
            this, &ContratManager::validateDates);
    connect(ui->date_fin,      &QDateEdit::dateChanged,
            this, &ContratManager::validateDates);
    connect(ui->obj_ach_ann,   QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ContratManager::validateFloats);
    connect(ui->tau_rem_acc,   QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &ContratManager::validateFloats);
    connect(ui->clause_penale, &QTextEdit::textChanged,
            this, &ContratManager::validateDescription);
    connect(ui->combo_ID_Client, &QComboBox::currentIndexChanged,
            this, &ContratManager::validateID);

    // Dates par défaut
    ui->date_debut->setDate(QDate::currentDate());
    ui->date_fin->setDate(QDate::currentDate());

    // ── Barre de recherche ───────────────────────────────────────────────────
    ui->recherche0_contrat->setReadOnly(false);
    ui->recherche0_contrat->setEnabled(true);
    ui->recherche0_contrat->setFocusPolicy(Qt::StrongFocus);
    ui->recherche0_contrat->setPlaceholderText("🔍 Rechercher par ID contrat, Type, Statut...");
    ui->recherche0_contrat->setClearButtonEnabled(true);
    ui->recherche0_contrat->setStyleSheet(
        "QLineEdit { background: #e8eceb; color: #3a3a3a; border: 1px solid #bfbfbf; border-radius: 8px; padding: 8px; } "
        "QLineEdit:focus { background: #e8eceb; color: #222; border: 1px solid #6aa84f; }");

    connect(ui->recherche0_contrat, &QLineEdit::textChanged,
            this, &ContratManager::onRechercheTextChanged);

    // ContratManager gère son propre event filter pour le focus de la recherche
    ui->recherche0_contrat->installEventFilter(this);

    // ── Statistiques ─────────────────────────────────────────────────────────
    setupStatsUI();
    refreshStats();
}

// ─── Initialisation de la zone graphiques ────────────────────────────────────
void ContratManager::setupStatsUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(ui->gb_stats_contrat);
    mainLayout->setSpacing(6);
    mainLayout->setContentsMargins(6, 22, 6, 6);

    // Histogramme
    chartViewContratType = new QChartView();
    chartViewContratType->setRenderHint(QPainter::Antialiasing);
    chartViewContratType->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartViewContratType->setMinimumHeight(160);
    chartViewContratType->setMaximumHeight(295);
    chartViewContratType->setMinimumWidth(0);

    // Camembert
    chartViewContratTypePie = new QChartView();
    chartViewContratTypePie->setRenderHint(QPainter::Antialiasing);
    chartViewContratTypePie->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    chartViewContratTypePie->setMinimumHeight(160);
    chartViewContratTypePie->setMaximumHeight(295);
    chartViewContratTypePie->setMinimumWidth(0);

    mainLayout->addWidget(chartViewContratType);
    mainLayout->addWidget(chartViewContratTypePie);

    ui->gb_stats_contrat->setLayout(mainLayout);
}

// ─── Mise à jour des graphiques statistiques ─────────────────────────────────
void ContratManager::refreshStats()
{
    QSqlQuery q;

    // ── 1. Histogramme : 3 barres côte à côte (Totale / Sur gamme / Partielle) ──
    int cntTotale = 0, cntSurGamme = 0, cntPartielle = 0;
    if (q.exec("SELECT TYPE_EXCLUSIVITE, COUNT(*) FROM CONTRAT GROUP BY TYPE_EXCLUSIVITE")) {
        while (q.next()) {
            QString t = q.value(0).toString().trimmed();
            int n     = q.value(1).toInt();
            if      (t == "Totale")    cntTotale    = n;
            else if (t == "Sur gamme") cntSurGamme  = n;
            else if (t == "Partielle") cntPartielle = n;
        }
    }

    QChart *chartType = new QChart();
    chartType->setTitle("Contrats par Type");
    chartType->setTitleFont(QFont("Arial", 9, QFont::Bold));
    chartType->setAnimationOptions(QChart::SeriesAnimations);
    chartType->setMargins(QMargins(0, 4, 8, 0));
    chartType->setBackgroundRoundness(0);
    chartType->legend()->setVisible(true);
    chartType->legend()->setAlignment(Qt::AlignBottom);
    chartType->legend()->setFont(QFont("Arial", 7));

    QBarSet *sTotale    = new QBarSet("Totale");
    QBarSet *sSurGamme  = new QBarSet("Sur gamme");
    QBarSet *sPartielle = new QBarSet("Partielle");

    sTotale->setColor(QColor("#1b5e20"));
    sTotale->setBorderColor(QColor("#0d3d12"));
    sTotale->setLabelColor(Qt::white);
    *sTotale << cntTotale;

    sSurGamme->setColor(QColor("#e53935"));
    sSurGamme->setBorderColor(QColor("#b71c1c"));
    sSurGamme->setLabelColor(Qt::white);
    *sSurGamme << cntSurGamme;

    sPartielle->setColor(QColor("#1565c0"));
    sPartielle->setBorderColor(QColor("#0d47a1"));
    sPartielle->setLabelColor(Qt::white);
    *sPartielle << cntPartielle;

    QBarSeries *barSeries = new QBarSeries();
    barSeries->setBarWidth(0.5);
    barSeries->append(sTotale);
    barSeries->append(sSurGamme);
    barSeries->append(sPartielle);
    barSeries->setLabelsVisible(true);
    barSeries->setLabelsPosition(QAbstractBarSeries::LabelsOutsideEnd);
    barSeries->setLabelsFormat("@value");

    chartType->addSeries(barSeries);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(QStringList{"Types d'exclusivite"});
    axisX->setLabelsFont(QFont("Arial", 8));
    axisX->setGridLineVisible(false);
    chartType->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    int maxVal = qMax(qMax(cntTotale, cntSurGamme), cntPartielle);
    QValueAxis *axisY = new QValueAxis();
    axisY->setMin(0);
    axisY->setMax(maxVal < 1 ? 5 : maxVal + 2);
    axisY->setLabelFormat("%i");
    axisY->setLabelsFont(QFont("Arial", 7));
    axisY->setTickCount(qMin(maxVal + 3, 7));
    chartType->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    chartViewContratType->setChart(chartType);

    // ── 2. Camembert : Répartition par statut ────────────────────────────────
    QChart *chartPie = new QChart();
    chartPie->setTitle("Statuts des contrats");
    chartPie->setTitleFont(QFont("Arial", 8, QFont::Bold));
    chartPie->setMargins(QMargins(0, 4, 0, 0));
    chartPie->setBackgroundRoundness(0);
    chartPie->setAnimationOptions(QChart::SeriesAnimations);

    QPieSeries *pieSeries = new QPieSeries();
    pieSeries->setPieStartAngle(30);
    pieSeries->setHoleSize(0.3);
    pieSeries->setPieSize(0.62);

    QList<QColor> pieColors = {
        QColor("#1b5e20"), QColor("#42a5f5"),
        QColor("#ef9a9a"), QColor("#ffe082")
    };
    int ci = 0;
    if (q.exec("SELECT STATUT_CONTRAT, COUNT(*) FROM CONTRAT GROUP BY STATUT_CONTRAT")) {
        while (q.next()) {
            QString statut = q.value(0).toString();
            if (statut.isEmpty()) statut = "Indéfini";
            int count = q.value(1).toInt();
            QPieSlice *slice = pieSeries->append(statut + " (" + QString::number(count) + ")", count);
            slice->setLabelVisible(true);
            slice->setLabelFont(QFont("Arial", 7));
            slice->setColor(pieColors[ci % pieColors.size()]);
            ci++;
        }
    }

    chartPie->addSeries(pieSeries);
    chartPie->legend()->setAlignment(Qt::AlignRight);
    chartPie->legend()->setFont(QFont("Arial", 7));
    chartViewContratTypePie->setChart(chartPie);
}

// ─── Remplir les comboboxes Client / Employé ─────────────────────────────────
void ContratManager::populateComboBoxes()
{
    // 1. Dropdown Client
    ui->combo_ID_Client->clear();
    ui->combo_ID_Client->setEditable(true);
    ui->combo_ID_Client->addItem("Sélectionner...", 0);

    QSqlQuery qClient("SELECT ID_CLIENT FROM CLIENT ORDER BY ID_CLIENT ASC");
    while (qClient.next()) {
        ui->combo_ID_Client->addItem(qClient.value(0).toString());
    }

    // 2. Dropdown Employé (CIN)
    ui->combo_CIN_Employe->clear();
    ui->combo_CIN_Employe->setEditable(true);
    ui->combo_CIN_Employe->addItem("Sélectionner...", 0);

    QSqlQuery qEmp("SELECT CIN FROM EMPLOYE ORDER BY CIN ASC");
    while (qEmp.next()) {
        ui->combo_CIN_Employe->addItem(qEmp.value(0).toString());
    }
}

// ─── En-têtes du modèle ──────────────────────────────────────────────────────
void ContratManager::setupModelHeaders(QSqlQueryModel *model)
{
    model->setHeaderData(0,  Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1,  Qt::Horizontal, QObject::tr("ID Client"));
    model->setHeaderData(2,  Qt::Horizontal, QObject::tr("CIN Employé"));
    model->setHeaderData(3,  Qt::Horizontal, QObject::tr("Type Exclusivité"));
    model->setHeaderData(4,  Qt::Horizontal, QObject::tr("Produits"));
    model->setHeaderData(5,  Qt::Horizontal, QObject::tr("Date Début"));
    model->setHeaderData(6,  Qt::Horizontal, QObject::tr("Date Fin"));
    model->setHeaderData(7,  Qt::Horizontal, QObject::tr("Obj. Achat"));
    model->setHeaderData(8,  Qt::Horizontal, QObject::tr("Taux Rem."));
    model->setHeaderData(9,  Qt::Horizontal, QObject::tr("Statut"));
    model->setHeaderData(10, Qt::Horizontal, QObject::tr("Clause Pénale"));
}

// ─── Clic simple : affiche l'ID dans le formulaire ───────────────────────────
void ContratManager::onTabClicked(const QModelIndex &index)
{
    currentSelectedId = ui->tab_contrat_2->model()
                            ->data(ui->tab_contrat_2->model()->index(index.row(), 0)).toInt();
    ui->ID_Contrat->setText(QString::number(currentSelectedId));
}

// ─── Double-clic : remplit tout le formulaire ─────────────────────────────────
void ContratManager::onTabDoubleClicked(const QModelIndex &index)
{
    int row = index.row();
    auto model = ui->tab_contrat_2->model();

    currentSelectedId = model->data(model->index(row, 0)).toInt();
    ui->ID_Contrat->setText(QString::number(currentSelectedId));

    QString id_client = model->data(model->index(row, 1)).toString();
    int clientIdx = ui->combo_ID_Client->findText(id_client);
    if (clientIdx != -1) ui->combo_ID_Client->setCurrentIndex(clientIdx);

    QString cin_val = model->data(model->index(row, 2)).toString();
    int cinIdx = ui->combo_CIN_Employe->findText(cin_val);
    if (cinIdx != -1) ui->combo_CIN_Employe->setCurrentIndex(cinIdx);

    ui->type_ex->setCurrentText(model->data(model->index(row, 3)).toString());
    ui->prod_con->setCurrentText(model->data(model->index(row, 4)).toString());
    ui->date_debut->setDate(model->data(model->index(row, 5)).toDate());
    ui->date_fin->setDate(model->data(model->index(row, 6)).toDate());

    QString objStr = model->data(model->index(row, 7)).toString().replace(",", ".");
    QString tauStr = model->data(model->index(row, 8)).toString().replace(",", ".");
    ui->obj_ach_ann->setValue(objStr.toDouble());
    ui->tau_rem_acc->setValue(tauStr.toDouble());

    ui->status_contrat->setCurrentText(model->data(model->index(row, 9)).toString());
    ui->clause_penale->setPlainText(model->data(model->index(row, 10)).toString());

    validateID();
    validateDates();
    validateFloats();
    validateDescription();
}

// ─── Ajouter un contrat ───────────────────────────────────────────────────────
void ContratManager::onAjouterClicked()
{
    int     id_client       = ui->combo_ID_Client->currentText().toInt();
    int     cin             = ui->combo_CIN_Employe->currentText().toInt();
    QString type_ex         = ui->type_ex->currentText();
    QString prod_con        = ui->prod_con->currentText();
    QDate   date_debut      = ui->date_debut->date();
    QDate   date_fin        = ui->date_fin->date();
    double  obj_ach_ann     = ui->obj_ach_ann->value();
    double  tau_rem_acc     = ui->tau_rem_acc->value();
    QString statut_contrat  = ui->status_contrat->currentText();
    QString clause_penale   = ui->clause_penale->toPlainText().trimmed();

    if (!checkContratDates()) {
        QMessageBox::warning(nullptr, "Erreur",
            "La date de début doit être au plus aujourd'hui et la date de fin doit être après ou égale à la date de début.");
        return;
    }
    if (id_client == 0) {
        QMessageBox::warning(nullptr, "Erreur", "Veuillez sélectionner ou saisir un Client ID.");
        return;
    }
    if (cin == 0) {
        QMessageBox::warning(nullptr, "Erreur", "Veuillez sélectionner un CIN Employé.");
        return;
    }

    Contrat c(id_client, cin, type_ex, prod_con, date_debut, date_fin,
              obj_ach_ann, tau_rem_acc, statut_contrat, clause_penale);

    if (c.ajouter()) {
        QMessageBox::information(nullptr, "Succès", "Le contrat a été ajouté avec succès !");
        ui->ID_Contrat->clear();
        ui->combo_ID_Client->setCurrentIndex(0);
        ui->combo_CIN_Employe->setCurrentIndex(0);
        ui->tab_contrat_2->setModel(Contrat().afficher());
        resetValidationStyles();
        refreshStats();
    } else {
        QSqlError err = QSqlDatabase::database().lastError();
        QMessageBox::critical(nullptr, "Erreur lors de l'ajout",
                              "L'opération a échoué.\n\nDétails techniques :\n" + err.text());
    }
}

// ─── Modifier un contrat ──────────────────────────────────────────────────────
void ContratManager::onModifierClicked()
{
    if (currentSelectedId == -1) {
        QMessageBox::warning(nullptr, "Sélection requise",
            "Veuillez sélectionner (double-clic) un contrat pour le modifier.");
        return;
    }

    int     id_client       = ui->combo_ID_Client->currentText().toInt();
    int     cin             = ui->combo_CIN_Employe->currentText().toInt();
    QString type_ex         = ui->type_ex->currentText();
    QString prod_con        = ui->prod_con->currentText();
    QDate   date_debut      = ui->date_debut->date();
    QDate   date_fin        = ui->date_fin->date();
    double  obj_ach_ann     = ui->obj_ach_ann->value();
    double  tau_rem_acc     = ui->tau_rem_acc->value();
    QString statut_contrat  = ui->status_contrat->currentText();
    QString clause_penale   = ui->clause_penale->toPlainText().trimmed();

    bool ok;
    int new_id = ui->ID_Contrat->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(nullptr, "ID invalide", "Veuillez saisir un ID numérique valide.");
        return;
    }

    if (!checkContratDates()) {
        QMessageBox::warning(nullptr, "Erreur",
            "La date de début doit être au plus aujourd'hui et la date de fin doit être après ou égale à la date de début.");
        return;
    }
    if (id_client == 0) {
        QMessageBox::warning(nullptr, "Erreur", "Veuillez sélectionner un Client.");
        return;
    }
    if (cin == 0) {
        QMessageBox::warning(nullptr, "Erreur", "Veuillez sélectionner un Employé.");
        return;
    }

    Contrat c(id_client, cin, type_ex, prod_con, date_debut, date_fin,
              obj_ach_ann, tau_rem_acc, statut_contrat, clause_penale);
    c.setId_contrat(new_id);

    if (c.modifier(currentSelectedId)) {
        QMessageBox::information(nullptr, "Succès", "Le contrat a été mis à jour !");
        currentSelectedId = new_id;
        ui->tab_contrat_2->setModel(Contrat().afficher());
        refreshStats();
    } else {
        QSqlError err = QSqlDatabase::database().lastError();
        QMessageBox::critical(nullptr, "Erreur lors de la modification",
                              "L'opération a échoué.\n\nDétails techniques :\n" + err.text());
    }
}

// ─── Supprimer un contrat ─────────────────────────────────────────────────────
void ContratManager::onSupprimerClicked()
{
    bool ok;
    int id = ui->ID_Contrat->text().toInt(&ok);

    if (!ok) {
        QMessageBox::warning(nullptr, "Sélection requise",
            "Veuillez sélectionner un contrat dans le tableau.");
        return;
    }

    if (QMessageBox::question(nullptr, "Confirmation",
            "Voulez-vous vraiment supprimer ce contrat ?",
            QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if (Contrat().supprimer(id)) {
            QMessageBox::information(nullptr, "Succès", "Contrat supprimé avec succès.");
            ui->tab_contrat_2->setModel(Contrat().afficher());
            ui->ID_Contrat->clear();
            currentSelectedId = -1;
            refreshStats();
        } else {
            QMessageBox::critical(nullptr, "Erreur", "La suppression a échoué.");
        }
    }
}

// ─── Exporter PDF ─────────────────────────────────────────────────────────────
void ContratManager::onExporterClicked()
{
    QString filePath = QFileDialog::getSaveFileName(nullptr,
        "Exporter la liste des contrats", "Liste_Contrats.pdf", "PDF (*.pdf)");
    if (filePath.isEmpty()) return;

    Contrat c;
    QSqlQueryModel *contractModel = c.afficher();
    if (!contractModel) {
        QMessageBox::critical(nullptr, "Erreur",
            "Impossible de charger la liste des contrats pour l'export PDF.");
        return;
    }

    bool ok2 = c.exporterPdf(filePath, contractModel);
    delete contractModel;

    if (ok2) {
        QMessageBox::information(nullptr, "Succès",
            "La liste des contrats a été exportée en PDF avec succès.");
    } else {
        QMessageBox::critical(nullptr, "Erreur", "Échec de l'export PDF pour les contrats.");
    }
}

// ─── Recherche ────────────────────────────────────────────────────────────────
void ContratManager::onRechercheTextChanged(const QString &arg1)
{
    Contrat c;
    QSqlQueryModel *model = c.rechercher(arg1);
    setupModelHeaders(model);
    ui->tab_contrat_2->setModel(model);
}

// ─── Tri ──────────────────────────────────────────────────────────────────────
void ContratManager::onTriClicked()
{
    QString critere = ui->tri_contrat->currentText();
    QString orderBy;
    if      (critere == "ID contrat")        orderBy = "ID_CONTRAT";
    else if (critere == "Type exclusivité")  orderBy = "TYPE_EXCLUSIVITE";
    else if (critere == "Statut contrat")    orderBy = "STATUT_CONTRAT";
    else                                     orderBy = "ID_CONTRAT";

    QSqlQueryModel *model = new QSqlQueryModel();
    QString sql = QString(
        "SELECT ID_CONTRAT, ID_CLIENT, CIN, TYPE_EXCLUSIVITE, PRODUITS_CONCERNES, "
        "DATE_DEBUT, DATE_FIN, OBJECTIF_ACHAT_ANNUEL, TAUX_REMISE_ACCORDE, STATUT_CONTRAT, CLAUSE_PENALE "
        "FROM CONTRAT ORDER BY %1"
    ).arg(orderBy);

    model->setQuery(sql);
    setupModelHeaders(model);
    ui->tab_contrat_2->setModel(model);
}

// ─── Event filter (focus barre de recherche) ──────────────────────────────────
bool ContratManager::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->recherche0_contrat) {
        if (event->type() == QEvent::FocusIn) {
            if (ui->recherche0_contrat->text().isEmpty()) {
                ui->recherche0_contrat->clear();
            }
            ui->recherche0_contrat->setStyleSheet(
                "QLineEdit { background: white; color: black; border: 1px solid #4CAF50; border-radius: 8px; padding: 5px; } "
                "QLineEdit:focus { background: white; color: black; }");
        } else if (event->type() == QEvent::FocusOut) {
            if (ui->recherche0_contrat->text().isEmpty()) {
                ui->recherche0_contrat->setStyleSheet(
                    "QLineEdit { background: #f5f5f5; color: #555; border: 1px solid #c8c8c8; border-radius: 8px; padding: 5px; } "
                    "QLineEdit:focus { background: white; color: black; border: 1px solid #4CAF50; }");
            }
        }
    }
    return QObject::eventFilter(watched, event);
}

// ─── Style validation ─────────────────────────────────────────────────────────
void ContratManager::setWidgetStyle(QWidget *widget, bool isValid)
{
    QString style = "background-color: white; color: black; border-radius: 5px; padding: 5px; font-weight: bold;";
    style += isValid ? "border: 2px solid green;" : "border: 2px solid red;";
    widget->setStyleSheet(style);
}

void ContratManager::resetValidationStyles()
{
    ui->ID_Contrat->setStyleSheet("");
    ui->combo_ID_Client->setStyleSheet("");
    ui->date_debut->setStyleSheet("");
    ui->date_fin->setStyleSheet("");
    ui->obj_ach_ann->setStyleSheet("");
    ui->tau_rem_acc->setStyleSheet("");
    ui->clause_penale->setStyleSheet("");
}

bool ContratManager::checkContratDates()
{
    QDate debut = ui->date_debut->date();
    QDate fin   = ui->date_fin->date();

    bool debutOk = Contrat::validerDateDebut(debut);
    bool finOk   = Contrat::validerDateFin(fin, debut);

    setWidgetStyle(ui->date_debut, debutOk);
    setWidgetStyle(ui->date_fin,   finOk);

    return debutOk && finOk;
}

void ContratManager::validateID()
{
    QString text = ui->ID_Contrat->text().trimmed();
    if (text.isEmpty()) {
        ui->ID_Contrat->setStyleSheet("");
    } else {
        bool idOk;
        int id = text.toInt(&idOk);
        setWidgetStyle(ui->ID_Contrat, idOk && Contrat::validerID(id));
    }

    if (ui->combo_ID_Client->currentIndex() <= 0) {
        ui->combo_ID_Client->setStyleSheet("");
    } else {
        setWidgetStyle(ui->combo_ID_Client, true);
    }
}

void ContratManager::validateDates()
{
    checkContratDates();
}

void ContratManager::validateFloats()
{
    double obj = ui->obj_ach_ann->value();
    double tau = ui->tau_rem_acc->value();

    if (obj == 0.0) {
        ui->obj_ach_ann->setStyleSheet("");
    } else {
        setWidgetStyle(ui->obj_ach_ann, Contrat::validerFloats(obj, tau));
    }

    if (tau == 0.0) {
        ui->tau_rem_acc->setStyleSheet("");
    } else {
        setWidgetStyle(ui->tau_rem_acc, Contrat::validerFloats(obj, tau));
    }
}

void ContratManager::validateDescription()
{
    QString text = ui->clause_penale->toPlainText().trimmed();
    if (text.isEmpty()) {
        ui->clause_penale->setStyleSheet("");
    } else {
        setWidgetStyle(ui->clause_penale, Contrat::validerDescription(text));
    }
}
