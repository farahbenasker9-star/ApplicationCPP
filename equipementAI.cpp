#include "equipementAI.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QScrollArea>
#include <QFont>
#include <QSizePolicy>
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QApplication>

// ═══════════════════════════════════════════════════════════════════════════════
//  Constructor
// ═══════════════════════════════════════════════════════════════════════════════
EquipementAIDialog::EquipementAIDialog(const QString &apiKey, QWidget *parent)
    : QDialog(parent)
    , m_apiKey(apiKey)
    , m_animTimer(new QTimer(this))
    , m_network(new QNetworkAccessManager(this))
{
    setWindowTitle("🤖  Recommandation IA — Équipement");
    setMinimumSize(560, 680);
    resize(600, 720);
    setModal(true);

    // Animation timer: 500 ms per frame
    m_animTimer->setInterval(500);
    connect(m_animTimer, &QTimer::timeout,
            this,        &EquipementAIDialog::onAnimationTimer);

    // Network reply handler
    connect(m_network, &QNetworkAccessManager::finished,
            this,      &EquipementAIDialog::onNetworkReply);

    setupUI();
    loadEquipmentTypes();

    // Start in idle state (wave robot)
    setRobotImage("wave.png");
}

// ═══════════════════════════════════════════════════════════════════════════════
//  UI construction
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIDialog::setupUI()
{
    QVBoxLayout *root = new QVBoxLayout(this);
    root->setSpacing(12);
    root->setContentsMargins(18, 16, 18, 16);

    // ── Robot image ──────────────────────────────────────────────────────────
    m_robotLabel = new QLabel(this);
    m_robotLabel->setAlignment(Qt::AlignCenter);
    m_robotLabel->setFixedSize(ROBOT_W, ROBOT_H);
    m_robotLabel->setStyleSheet("border: none; background: transparent;");

    QHBoxLayout *robotRow = new QHBoxLayout();
    robotRow->addStretch();
    robotRow->addWidget(m_robotLabel);
    robotRow->addStretch();
    root->addLayout(robotRow);

    // ── Title label ──────────────────────────────────────────────────────────
    QLabel *title = new QLabel("Recommandation d'Équipement par IA", this);
    QFont tf = title->font();
    tf.setPointSize(13);
    tf.setBold(true);
    title->setFont(tf);
    title->setAlignment(Qt::AlignCenter);
    title->setStyleSheet("color: #1B4332;");
    root->addWidget(title);

    QLabel *subtitle = new QLabel(
        "Sélectionnez un type d'équipement. L'IA analysera le statut, le fabricant\n"
        "et les notes (NLP) pour recommander l'unité optimale.", this);
    subtitle->setAlignment(Qt::AlignCenter);
    subtitle->setStyleSheet("color: #555; font-size: 11px;");
    subtitle->setWordWrap(true);
    root->addWidget(subtitle);

    // ── Equipment type selector ───────────────────────────────────────────────
    QGroupBox *selBox = new QGroupBox("Type d'équipement", this);
    selBox->setStyleSheet(
        "QGroupBox { font-weight: bold; color: #1B4332; border: 1px solid #BDBDBD;"
        " border-radius: 6px; margin-top: 8px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; }");

    QHBoxLayout *selLayout = new QHBoxLayout(selBox);
    selLayout->setSpacing(10);

    QLabel *selLabel = new QLabel("Type :", selBox);
    m_typeCombo = new QComboBox(selBox);
    m_typeCombo->setMinimumWidth(200);
    m_typeCombo->setStyleSheet(
        "QComboBox { border: 1px solid #BDBDBD; border-radius: 4px; padding: 4px 8px;"
        " background: white; color: #222; }"
        "QComboBox QAbstractItemView { background: white; color: #222;"
        " selection-background-color: #4CAF50; selection-color: white; }");

    m_analyzeBtn = new QPushButton("⚡  Analyser", selBox);
    m_analyzeBtn->setEnabled(false);
    m_analyzeBtn->setStyleSheet(
        "QPushButton { background-color: #1B4332; color: white; border-radius: 5px;"
        " padding: 6px 18px; font-weight: bold; font-size: 12px; }"
        "QPushButton:hover   { background-color: #2E7D52; }"
        "QPushButton:pressed { background-color: #145228; }"
        "QPushButton:disabled{ background-color: #BDBDBD; color: #777; }");

    selLayout->addWidget(selLabel);
    selLayout->addWidget(m_typeCombo, 1);
    selLayout->addWidget(m_analyzeBtn);
    root->addWidget(selBox);

    // Status line
    m_statusLabel = new QLabel("", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet("color: #888; font-size: 11px;");
    root->addWidget(m_statusLabel);

    // ── Output text area ──────────────────────────────────────────────────────
    QGroupBox *outBox = new QGroupBox("Recommandation de l'IA", this);
    outBox->setStyleSheet(
        "QGroupBox { font-weight: bold; color: #1B4332; border: 1px solid #BDBDBD;"
        " border-radius: 6px; margin-top: 8px; padding-top: 6px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; }");
    QVBoxLayout *outLayout = new QVBoxLayout(outBox);

    m_outputEdit = new QPlainTextEdit(outBox);
    m_outputEdit->setReadOnly(true);
    m_outputEdit->setMinimumHeight(260);
    m_outputEdit->setPlaceholderText(
        "La recommandation de l'IA apparaîtra ici après l'analyse…");
    m_outputEdit->setStyleSheet(
        "QPlainTextEdit { background: #FAFAFA; color: #222; border: 1px solid #DDD;"
        " border-radius: 4px; padding: 8px; font-family: 'Segoe UI', Arial; font-size: 12px; }");

    outLayout->addWidget(m_outputEdit);
    root->addWidget(outBox, 1);

    // ── Close button ──────────────────────────────────────────────────────────
    QPushButton *closeBtn = new QPushButton("Fermer", this);
    closeBtn->setStyleSheet(
        "QPushButton { background: #E0E0E0; color: #333; border-radius: 5px;"
        " padding: 5px 20px; }"
        "QPushButton:hover { background: #BDBDBD; }");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);

    QHBoxLayout *closeRow = new QHBoxLayout();
    closeRow->addStretch();
    closeRow->addWidget(closeBtn);
    root->addLayout(closeRow);

    // ── Signal connections ────────────────────────────────────────────────────
    connect(m_typeCombo, qOverload<int>(&QComboBox::currentIndexChanged),
            this,        &EquipementAIDialog::onEquipmentTypeChanged);
    connect(m_analyzeBtn, &QPushButton::clicked,
            this,         &EquipementAIDialog::onAnalyzeClicked);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  DB: load equipment types
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIDialog::loadEquipmentTypes()
{
    m_typeCombo->blockSignals(true);
    m_typeCombo->clear();
    m_typeCombo->addItem("-- Sélectionner un type --", QString());

    QSqlQuery q;
    if (q.exec("SELECT DISTINCT EQUIPMENT_TYPE FROM EQUIPMENT "
               "WHERE EQUIPMENT_TYPE IS NOT NULL "
               "ORDER BY EQUIPMENT_TYPE")) {
        while (q.next()) {
            QString t = q.value(0).toString().trimmed();
            if (!t.isEmpty())
                m_typeCombo->addItem(t, t);
        }
    } else {
        qDebug() << "EquipementAIDialog::loadEquipmentTypes error:"
                 << q.lastError().text();
    }

    m_typeCombo->blockSignals(false);
    onEquipmentTypeChanged(m_typeCombo->currentIndex());
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Slot: equipment type changed
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIDialog::onEquipmentTypeChanged(int index)
{
    const bool hasSelection = (index > 0);
    m_analyzeBtn->setEnabled(hasSelection && m_state != State::Thinking);

    if (!hasSelection) {
        // Back to idle — show wave robot
        m_state = State::Idle;
        stopThinkingAnimation();
        setRobotImage("wave.png");
        m_outputEdit->clear();
        m_statusLabel->setText("");
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Slot: analyze button clicked
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIDialog::onAnalyzeClicked()
{
    const QString type = m_typeCombo->currentData().toString().trimmed();
    if (type.isEmpty()) return;

    // ── Transition to Thinking state ─────────────────────────────────────────
    m_state = State::Thinking;
    m_analyzeBtn->setEnabled(false);
    m_typeCombo->setEnabled(false);
    m_outputEdit->clear();
    m_statusLabel->setText("Interrogation de l'IA en cours…");
    startThinkingAnimation();

    // ── Build prompt and call API ─────────────────────────────────────────────
    const QString prompt = buildPrompt(type);
    callOpenAI(prompt);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  DB: fetch equipment data for a given type
// ═══════════════════════════════════════════════════════════════════════════════
QString EquipementAIDialog::fetchEquipmentData(const QString &equipmentType)
{
    QSqlQuery q;
    q.prepare(
        "SELECT EQ.EQUIPMENT_ID, EQ.CIN, "
        "       E.NOM || ' ' || E.PRENOM  AS EMPLOYE, "
        "       EQ.FABRICANT, EQ.STATUT, "
        "       EQ.DATE_DER_MAINT, EQ.DATE_SUIV_MAINT, "
        "       EQ.NOTES "
        "FROM   EQUIPMENT EQ "
        "LEFT JOIN EMPLOYE E ON EQ.CIN = E.CIN "
        "WHERE  UPPER(EQ.EQUIPMENT_TYPE) = UPPER(:type) "
        "ORDER BY EQ.EQUIPMENT_ID"
    );
    q.bindValue(":type", equipmentType);

    if (!q.exec()) {
        qDebug() << "EquipementAIDialog::fetchEquipmentData error:"
                 << q.lastError().text();
        return QString("Erreur lors de la récupération des données: %1")
                   .arg(q.lastError().text());
    }

    if (!q.next()) {
        return QString("Aucun équipement de type \"%1\" trouvé dans la base de données.")
                   .arg(equipmentType);
    }

    // Build a readable Markdown table
    QString table;
    table += "| ID | Assigné à | Fabricant | Statut | Dern. Maint. | Proch. Maint. | Notes |\n";
    table += "|----|-----------|-----------|---------|--------------|--------------:|-------|\n";

    do {
        const QString id       = q.value(0).toString();
        const QString employe  = q.value(2).toString().trimmed();
        const QString fab      = q.value(3).toString().trimmed();
        const QString statut   = q.value(4).toString().trimmed();
        const QString lastM    = q.value(5).toDateTime().toString("dd/MM/yyyy");
        const QString nextM    = q.value(6).toDateTime().toString("dd/MM/yyyy");
        // Sanitize notes: collapse newlines, cap at 200 chars to keep prompt compact
        QString notes = q.value(7).toString().simplified();
        if (notes.length() > 200) notes = notes.left(197) + "…";

        table += QString("| %1 | %2 | %3 | %4 | %5 | %6 | %7 |\n")
                     .arg(id, employe.isEmpty() ? "—" : employe,
                          fab, statut, lastM, nextM,
                          notes.isEmpty() ? "—" : notes);
    } while (q.next());

    return table;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Build the OpenAI prompt
// ═══════════════════════════════════════════════════════════════════════════════
QString EquipementAIDialog::buildPrompt(const QString &equipmentType)
{
    const QString data = fetchEquipmentData(equipmentType);

    return QString(
        "Vous êtes un expert en gestion de parc d'équipements industriels "
        "spécialisé dans la maintenance préventive et corrective.\n\n"

        "Type d'équipement demandé : **%1**\n\n"

        "Voici l'inventaire complet de ce type d'équipement extrait de la base "
        "de données Oracle de l'entreprise :\n\n"
        "%2\n\n"

        "Votre mission (répondez en français) :\n"
        "1. **Analyse NLP des notes** : Lisez attentivement chaque note de "
        "maintenance. Identifiez les problèmes récurrents, pannes critiques, "
        "mots-clés négatifs (\"défaillance\", \"bruit\", \"fuite\", \"surchauffe\"…) "
        "et indicateurs positifs (\"révisé\", \"conforme\", \"performant\"…).\n"
        "2. **Évaluation par unité** : Pour chaque équipement, évaluez sur la "
        "base du statut opérationnel, de la réputation du fabricant, de "
        "l'historique de maintenance et de l'analyse des notes.\n"
        "3. **Recommandation principale** : Indiquez clairement l'ID de "
        "l'équipement optimal et justifiez votre choix en 3-5 phrases.\n"
        "4. **Alertes urgentes** : Signalez tout équipement nécessitant une "
        "intervention immédiate (maintenance dépassée, statut critique, notes "
        "alarmantes).\n"
        "5. **Résumé** : Terminez par une ligne de synthèse courte.\n\n"
        "Soyez précis, structuré et professionnel."
    ).arg(equipmentType, data);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  HTTP: call OpenAI Chat Completions
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIDialog::callOpenAI(const QString &prompt)
{
    QNetworkRequest request(QUrl("https://api.openai.com/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization",
                         QString("Bearer %1").arg(m_apiKey).toUtf8());

    // Build the JSON body
    QJsonObject systemMsg;
    systemMsg["role"]    = "system";
    systemMsg["content"] = "You are an expert industrial equipment management analyst "
                           "with deep knowledge of maintenance planning and NLP-based "
                           "text analysis. Always respond in French.";

    QJsonObject userMsg;
    userMsg["role"]    = "user";
    userMsg["content"] = prompt;

    QJsonObject body;
    body["model"]       = "gpt-4o";          // Change to "gpt-4o-mini" to reduce cost
    body["max_tokens"]  = 1024;
    body["temperature"] = 0.4;               // Lower = more deterministic/factual
    body["messages"]    = QJsonArray{ systemMsg, userMsg };

    m_network->post(request, QJsonDocument(body).toJson(QJsonDocument::Compact));
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Slot: network reply received
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIDialog::onNetworkReply(QNetworkReply *reply)
{
    reply->deleteLater();

    stopThinkingAnimation();
    m_state = State::Done;
    m_typeCombo->setEnabled(true);
    m_analyzeBtn->setEnabled(true);

    // ── Network-level error ───────────────────────────────────────────────────
    if (reply->error() != QNetworkReply::NoError) {
        const QString errMsg = QString(
            "⚠️  Erreur réseau : %1\n\n"
            "Vérifiez votre connexion internet et la validité de la clé API.")
            .arg(reply->errorString());
        m_outputEdit->setPlainText(errMsg);
        m_statusLabel->setText("Échec de la connexion à l'API.");
        setRobotImage("wave.png");   // go back to neutral
        return;
    }

    // ── Parse JSON response ───────────────────────────────────────────────────
    const QByteArray raw = reply->readAll();
    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(raw, &parseErr);

    if (parseErr.error != QJsonParseError::NoError) {
        m_outputEdit->setPlainText(
            QString("⚠️  Réponse JSON invalide : %1\n\nRéponse brute :\n%2")
                .arg(parseErr.errorString(), QString::fromUtf8(raw)));
        m_statusLabel->setText("Erreur de parsing.");
        setRobotImage("wave.png");
        return;
    }

    // Check for an API-level error (e.g. invalid key, quota exceeded)
    if (doc.object().contains("error")) {
        const QString apiErr =
            doc.object()["error"].toObject()["message"].toString();
        m_outputEdit->setPlainText(
            QString("⚠️  Erreur API OpenAI :\n%1").arg(apiErr));
        m_statusLabel->setText("Erreur retournée par l'API.");
        setRobotImage("wave.png");
        return;
    }

    // Extract the assistant message content
    const QJsonArray choices = doc.object()["choices"].toArray();
    if (choices.isEmpty()) {
        m_outputEdit->setPlainText("⚠️  Aucune réponse reçue de l'API (choices vide).");
        m_statusLabel->setText("Réponse vide.");
        setRobotImage("wave.png");
        return;
    }

    const QString content =
        choices[0].toObject()["message"].toObject()["content"].toString().trimmed();

    if (content.isEmpty()) {
        m_outputEdit->setPlainText("⚠️  L'IA a retourné une réponse vide.");
        m_statusLabel->setText("Réponse vide.");
        setRobotImage("wave.png");
        return;
    }

    // ── Success ───────────────────────────────────────────────────────────────
    m_outputEdit->setPlainText(content);
    m_statusLabel->setText("✅  Analyse terminée.");
    setRobotImage("idea.png");   // robot shows the "idea" image
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Animation helpers
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIDialog::setRobotImage(const QString &resourceName)
{
    // Try Qt resource path first (":/<name>"), then bare file path
    const QString resourcePath = ":/" + resourceName;
    QPixmap pix(resourcePath);

    if (pix.isNull()) {
        // Fallback: look next to the executable
        pix.load(resourceName);
    }

    if (!pix.isNull()) {
        m_robotLabel->setPixmap(
            pix.scaled(ROBOT_W, ROBOT_H, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        // If no image found, show a placeholder emoji so the UI still looks ok
        m_robotLabel->setPixmap(QPixmap());
        m_robotLabel->setText(resourceName == "wave.png"  ? "🤖" :
                              resourceName == "idea.png"  ? "💡" : "🔄");
        m_robotLabel->setStyleSheet("font-size: 56px; border: none; background: transparent;");
    }
}

void EquipementAIDialog::startThinkingAnimation()
{
    m_animPhase = false;
    setRobotImage("think1.png");
    m_animTimer->start();
}

void EquipementAIDialog::stopThinkingAnimation()
{
    m_animTimer->stop();
}

// ─── Animation frame tick ─────────────────────────────────────────────────────
void EquipementAIDialog::onAnimationTimer()
{
    m_animPhase = !m_animPhase;
    setRobotImage(m_animPhase ? "think2.png" : "think1.png");
}
