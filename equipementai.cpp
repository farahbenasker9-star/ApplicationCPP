#include "equipementAI.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFont>

// ═══════════════════════════════════════════════════════════════════════════════
//  Constructor
// ═══════════════════════════════════════════════════════════════════════════════
EquipementAIPanel::EquipementAIPanel(const QString   &apiKey,
                                     QComboBox       *typeCombo,
                                     QPlainTextEdit  *outputEdit,
                                     QWidget         *container,
                                     QObject         *parent)
    : QObject(parent)
    , m_apiKey(apiKey)
    , m_typeCombo(typeCombo)
    , m_outputEdit(outputEdit)
    , m_container(container)
    , m_animTimer(new QTimer(this))
    , m_network(new QNetworkAccessManager(this))
{
    // ── Animation timer ───────────────────────────────────────────────────────
    m_animTimer->setInterval(500);
    connect(m_animTimer, &QTimer::timeout,
            this,        &EquipementAIPanel::onAnimationTimer);

    // ── Network reply handler ─────────────────────────────────────────────────
    connect(m_network, &QNetworkAccessManager::finished,
            this,      &EquipementAIPanel::onNetworkReply);

    // ── Build the robot label + Analyser button inside the container ──────────
    buildEmbeddedWidgets();

    // ── Prepare the output text area ──────────────────────────────────────────
    m_outputEdit->setReadOnly(true);
    m_outputEdit->setPlaceholderText(
        "Bonjour, veuillez sélectionner un type d'équipement pour commencer l'analyse.");

    // ── Populate the type combo from the DB ───────────────────────────────────
    loadEquipmentTypes();

    // ── Wire the combo so state changes are reflected immediately ─────────────
    connect(m_typeCombo, qOverload<int>(&QComboBox::currentIndexChanged),
            this,        &EquipementAIPanel::onEquipmentTypeChanged);

    // ── Start in idle (wave) state ────────────────────────────────────────────
    setRobotImage("wave.png");
}

// ═══════════════════════════════════════════════════════════════════════════════
//  buildEmbeddedWidgets
//  Creates the robot QLabel and "Analyser" QPushButton directly inside the
//  container widget.  They use fixed geometry so they fit within the gap
//  between the type combo (bottom ≈ y=70) and the output text area (top y=260).
//
//  Container (gb_stats_equipement_2) is 341 px wide.
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIPanel::buildEmbeddedWidgets()
{
    // ── Robot image label ─────────────────────────────────────────────────────
    // Centred horizontally: x = (341 - 140) / 2 = 100
    m_robotLabel = new QLabel(m_container);
    m_robotLabel->setGeometry(100, 78, ROBOT_W, ROBOT_H);
    m_robotLabel->setAlignment(Qt::AlignCenter);
    m_robotLabel->setScaledContents(false);   // we scale manually to keep ratio
    m_robotLabel->setStyleSheet("border: none; background: transparent;");
    m_robotLabel->show();

    // ── Analyser button ───────────────────────────────────────────────────────
    // Centred: x = (341 - 141) / 2 = 100,  placed just below the robot (y=225)
    m_analyzeBtn = new QPushButton("⚡  Analyser", m_container);
    m_analyzeBtn->setGeometry(100, 225, 141, 34);
    m_analyzeBtn->setEnabled(false);
    m_analyzeBtn->setStyleSheet(
        "QPushButton {"
        "  background-color: #1B4332; color: white;"
        "  border-radius: 6px; font-weight: bold; font-size: 12px;"
        "}"
        "QPushButton:hover    { background-color: #2E7D52; }"
        "QPushButton:pressed  { background-color: #145228; }"
        "QPushButton:disabled { background-color: #BDBDBD; color: #777; }");
    m_analyzeBtn->show();

    connect(m_analyzeBtn, &QPushButton::clicked,
            this,         &EquipementAIPanel::onAnalyzeClicked);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  loadEquipmentTypes  —  populate combo from DB
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIPanel::loadEquipmentTypes()
{
    m_typeCombo->blockSignals(true);
    m_typeCombo->clear();
    m_typeCombo->addItem("— Sélectionner un type —", QString());

    QSqlQuery q;
    if (q.exec("SELECT DISTINCT EQUIPMENT_TYPE FROM EQUIPMENT "
               "WHERE EQUIPMENT_TYPE IS NOT NULL "
               "ORDER BY EQUIPMENT_TYPE")) {
        while (q.next()) {
            const QString t = q.value(0).toString().trimmed();
            if (!t.isEmpty())
                m_typeCombo->addItem(t, t);
        }
    } else {
        qDebug() << "EquipementAIPanel::loadEquipmentTypes error:"
                 << q.lastError().text();
    }

    m_typeCombo->blockSignals(false);

    // Trigger initial state evaluation
    onEquipmentTypeChanged(m_typeCombo->currentIndex());
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Slot: type combo changed
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIPanel::onEquipmentTypeChanged(int index)
{
    const bool hasSelection = (index > 0 && !m_typeCombo->currentData().toString().isEmpty());
    m_analyzeBtn->setEnabled(hasSelection && m_state != State::Thinking);

    if (!hasSelection) {
        // Return to idle state
        m_state = State::Idle;
        stopThinkingAnimation();
        setRobotImage("wave.png");
        m_outputEdit->clear();
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Slot: Analyser button clicked
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIPanel::onAnalyzeClicked()
{
    const QString type = m_typeCombo->currentData().toString().trimmed();
    if (type.isEmpty()) return;

    // Transition → Thinking
    m_state = State::Thinking;
    m_analyzeBtn->setEnabled(false);
    m_typeCombo->setEnabled(false);
    m_outputEdit->clear();
    m_outputEdit->setPlaceholderText("Analyse en cours, veuillez patienter…");
    startThinkingAnimation();

    // Build prompt and fire the API call
    callOpenAI(buildPrompt(type));
}

// ═══════════════════════════════════════════════════════════════════════════════
//  fetchEquipmentData  —  query DB and build a Markdown table for the prompt
// ═══════════════════════════════════════════════════════════════════════════════
QString EquipementAIPanel::fetchEquipmentData(const QString &equipmentType)
{
    QSqlQuery q;
    q.prepare(
        "SELECT EQ.EQUIPMENT_ID, "
        "       EQ.CIN, "
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
        qDebug() << "EquipementAIPanel::fetchEquipmentData error:" << q.lastError().text();
        return QString("Erreur DB : %1").arg(q.lastError().text());
    }

    if (!q.next()) {
        return QString("Aucun équipement de type \"%1\" trouvé.").arg(equipmentType);
    }

    QString table;
    table += "| ID | Assigné à | Fabricant | Statut | Dern. Maint. | Proch. Maint. | Notes |\n";
    table += "|----|-----------|-----------|--------|--------------|---------------|-------|\n";

    do {
        const QString id      = q.value(0).toString();
        const QString employe = q.value(2).toString().trimmed();
        const QString fab     = q.value(3).toString().trimmed();
        const QString statut  = q.value(4).toString().trimmed();
        const QString lastM   = q.value(5).toDateTime().toString("dd/MM/yyyy");
        const QString nextM   = q.value(6).toDateTime().toString("dd/MM/yyyy");

        // Sanitise notes: collapse whitespace, cap at 200 chars
        QString notes = q.value(7).toString().simplified();
        if (notes.length() > 200) notes = notes.left(197) + "…";

        table += QString("| %1 | %2 | %3 | %4 | %5 | %6 | %7 |\n")
                     .arg(id,
                          employe.isEmpty() ? "—" : employe,
                          fab.isEmpty()     ? "—" : fab,
                          statut.isEmpty()  ? "—" : statut,
                          lastM.isEmpty()   ? "—" : lastM,
                          nextM.isEmpty()   ? "—" : nextM,
                          notes.isEmpty()   ? "—" : notes);
    } while (q.next());

    return table;
}

// ═══════════════════════════════════════════════════════════════════════════════
//  buildPrompt  —  assemble the full system + user message
// ═══════════════════════════════════════════════════════════════════════════════
QString EquipementAIPanel::buildPrompt(const QString &equipmentType)
{
    const QString data = fetchEquipmentData(equipmentType);

    return QString(
               "Vous êtes un expert en gestion de parc d'équipements industriels "
               "spécialisé dans la maintenance préventive et corrective.\n\n"

               "Type d'équipement demandé : **%1**\n\n"

               "Voici l'inventaire complet de ce type, extrait de la base Oracle :\n\n"
               "%2\n\n"

               "Votre mission (répondez en français) :\n"
               "1. **Analyse NLP des notes** : Lisez chaque note. Identifiez les problèmes "
               "récurrents, pannes critiques, mots négatifs (\"défaillance\", \"bruit\", "
               "\"fuite\", \"surchauffe\"…) et positifs (\"révisé\", \"conforme\", "
               "\"performant\"…).\n"
               "2. **Évaluation par unité** : statut opérationnel, réputation du fabricant, "
               "historique de maintenance, analyse des notes.\n"
               "3. **Recommandation principale** : Indiquez clairement l'ID de l'équipement "
               "optimal et justifiez en 3–5 phrases.\n"
               "4. **Alertes urgentes** : Signalez tout équipement nécessitant une intervention "
               "immédiate (maintenance dépassée, statut critique, notes alarmantes).\n"
               "5. **Résumé** : Une ligne de synthèse courte.\n\n"
               "Soyez précis, structuré et professionnel."
               ).arg(equipmentType, data);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  callOpenAI  —  HTTP POST to the Chat Completions endpoint
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIPanel::callOpenAI(const QString &prompt)
{
    QNetworkRequest request(QUrl("https://api.openai.com/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization",
                         QString("Bearer %1").arg(m_apiKey).toUtf8());

    QJsonObject systemMsg;
    systemMsg["role"]    = "system";
    systemMsg["content"] = "You are an expert industrial equipment management analyst "
                           "with deep knowledge of maintenance planning and NLP-based "
                           "text analysis. Always respond in French.";

    QJsonObject userMsg;
    userMsg["role"]    = "user";
    userMsg["content"] = prompt;

    QJsonObject body;
    body["model"]       = "gpt-4o-mini";   // change to "gpt-4o" for best quality
    body["max_tokens"]  = 1024;
    body["temperature"] = 0.4;
    body["messages"]    = QJsonArray{ systemMsg, userMsg };

    m_network->post(request, QJsonDocument(body).toJson(QJsonDocument::Compact));
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Slot: network reply received
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIPanel::onNetworkReply(QNetworkReply *reply)
{
    // IMPORTANT: always read the body BEFORE calling reply->deleteLater().
    // OpenAI returns JSON error details (key revoked, quota, etc.) in the body
    // even for HTTP 4xx responses, so we must read it regardless of error status.
    const QByteArray raw = reply->readAll();
    const int httpStatus  = reply->attribute(
                                    QNetworkRequest::HttpStatusCodeAttribute).toInt();

    reply->deleteLater();

    stopThinkingAnimation();
    m_typeCombo->setEnabled(true);
    m_analyzeBtn->setEnabled(true);
    m_outputEdit->setPlaceholderText(
        "Bonjour, veuillez s\xc3\xa9lectionner un type d'\xc3\xa9quipement pour commencer l'analyse.");

    // ── Try to parse the body as JSON (works for both success and error) ──────
    QJsonParseError parseErr;
    QJsonDocument doc = QJsonDocument::fromJson(raw, &parseErr);
    const bool bodyIsJson = (parseErr.error == QJsonParseError::NoError);

    // ── Network/transport-level failure (no HTTP response at all) ─────────────
    if (reply->error() != QNetworkReply::NoError && httpStatus == 0) {
        m_state = State::Done;
        m_outputEdit->setPlainText(
            QString("\xe2\x9a\xa0\xef\xb8\x8f  Erreur r\xc3\xa9seau : %1\n\n"
                    "V\xc3\xa9rifiez votre connexion internet.\n"
                    "Si le probl\xc3\xa8me persiste, SSL est peut-\xc3\xaatre absent :\n"
                    "copiez libssl-1_1-x64.dll et libcrypto-1_1-x64.dll dans le dossier "
                    "de votre ex\xc3\xa9cutable.")
                .arg(reply->errorString()));
        setRobotImage("wave.png");
        return;
    }

    // ── HTTP-level error (4xx / 5xx) — read OpenAI error body ─────────────────
    if (reply->error() != QNetworkReply::NoError) {
        m_state = State::Done;
        QString detail;

        if (bodyIsJson && doc.object().contains("error")) {
            // OpenAI standard error envelope: { "error": { "message": "...", "type": "..." } }
            const QJsonObject err = doc.object()["error"].toObject();
            const QString msg  = err["message"].toString();
            const QString type = err["type"].toString();
            const QString code = err["code"].toString();
            detail = QString("Type  : %1\nCode  : %2\nD\xc3\xa9tail: %3")
                         .arg(type.isEmpty() ? "—" : type,
                              code.isEmpty() ? "—" : code,
                              msg.isEmpty()  ? "—" : msg);
        } else {
            detail = QString::fromUtf8(raw).trimmed();
            if (detail.isEmpty()) detail = reply->errorString();
        }

        // Key revoked hint
        const bool keyError = (httpStatus == 401 || httpStatus == 403 ||
                               detail.contains("revoked",  Qt::CaseInsensitive) ||
                               detail.contains("invalid",  Qt::CaseInsensitive) ||
                               detail.contains("Incorrect API key", Qt::CaseInsensitive));

        QString msg = QString("\xe2\x9a\xa0\xef\xb8\x8f  Erreur HTTP %1\n\n%2").arg(httpStatus).arg(detail);
        if (keyError)
            msg += "\n\n\xf0\x9f\x94\x91 Votre cl\xc3\xa9 API semble invalide ou r\xc3\xa9voqu\xc3\xa9e.\n"
                   "G\xc3\xa9n\xc3\xa9rez-en une nouvelle sur platform.openai.com "
                   "et mettez-la \xc3\xa0 jour dans mainwindow.cpp.";

        m_outputEdit->setPlainText(msg);
        setRobotImage("wave.png");
        return;
    }

    // ── Parse success body ────────────────────────────────────────────────────
    if (!bodyIsJson) {
        m_state = State::Done;
        m_outputEdit->setPlainText(
            QString("\xe2\x9a\xa0\xef\xb8\x8f  R\xc3\xa9ponse JSON invalide : %1\n\nBrut :\n%2")
                .arg(parseErr.errorString(), QString::fromUtf8(raw)));
        setRobotImage("wave.png");
        return;
    }

    // API-level error inside a 200 OK (rare but possible)
    if (doc.object().contains("error")) {
        m_state = State::Done;
        const QString apiErr =
            doc.object()["error"].toObject()["message"].toString();
        m_outputEdit->setPlainText(
            QString("\xe2\x9a\xa0\xef\xb8\x8f  Erreur API OpenAI :\n%1").arg(apiErr));
        setRobotImage("wave.png");
        return;
    }

    // Extract assistant message
    const QJsonArray choices = doc.object()["choices"].toArray();
    if (choices.isEmpty()) {
        m_state = State::Done;
        m_outputEdit->setPlainText(
            "\xe2\x9a\xa0\xef\xb8\x8f  Aucune r\xc3\xa9ponse re\xc3\xa7ue de l'API (tableau choices vide).");
        setRobotImage("wave.png");
        return;
    }

    const QString result =
        choices[0].toObject()["message"].toObject()["content"]
            .toString().trimmed();

    if (result.isEmpty()) {
        m_state = State::Done;
        m_outputEdit->setPlainText("\xe2\x9a\xa0\xef\xb8\x8f  L'IA a retourn\xc3\xa9 une r\xc3\xa9ponse vide.");
        setRobotImage("wave.png");
        return;
    }

    // ── Success ───────────────────────────────────────────────────────────────
    m_state = State::Done;
    m_outputEdit->setPlainText(result);
    setRobotImage("idea.png");
}


// ═══════════════════════════════════════════════════════════════════════════════
//  Robot image helpers
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIPanel::setRobotImage(const QString &resourceName)
{
    // Try every common QRC path so this works regardless of .qrc folder structure.
    // If images still don't load, check Qt Creator Application Output for the
    // "tried:" debug line and match one of those paths in your .qrc file.
    const QStringList candidates = {
        ":/"           + resourceName,   // <file alias="wave.png">
        ":/images/"    + resourceName,   // <file alias="images/wave.png">
        ":/resources/" + resourceName,   // <file alias="resources/wave.png">
        ":/assets/"    + resourceName,   // <file alias="assets/wave.png">
        resourceName                     // bare filesystem path (last resort)
    };

    QPixmap pix;
    for (const QString &path : candidates) {
        pix = QPixmap(path);
        if (!pix.isNull()) {
            qDebug() << "[AI] image loaded from:" << path;
            break;
        }
    }

    if (!pix.isNull()) {
        m_robotLabel->setText(QString());
        m_robotLabel->setStyleSheet("border: none; background: transparent;");
        m_robotLabel->setPixmap(
            pix.scaled(ROBOT_W, ROBOT_H, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        qDebug() << "[AI] image NOT found for" << resourceName
                 << "-- tried paths:" << candidates;
        m_robotLabel->setPixmap(QPixmap());
        m_robotLabel->setStyleSheet(
            "font-size: 64px; border: none; background: transparent;");
        m_robotLabel->setText(
            resourceName == "wave.png" ? "ð¤" :
                resourceName == "idea.png" ? "ð¡" : "ð");
    }
}

void EquipementAIPanel::startThinkingAnimation()
{
    m_animPhase = false;
    setRobotImage("think1.png");
    m_animTimer->start();
}

void EquipementAIPanel::stopThinkingAnimation()
{
    m_animTimer->stop();
}

void EquipementAIPanel::onAnimationTimer()
{
    m_animPhase = !m_animPhase;
    setRobotImage(m_animPhase ? "think2.png" : "think1.png");
}
