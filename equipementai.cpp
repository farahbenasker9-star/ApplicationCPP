#include "equipementAI.h"

#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFont>
#include <QRegularExpression>

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
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIPanel::buildEmbeddedWidgets()
{
    // Centred horizontally: x = (341 - 140) / 2 = 100
    m_robotLabel = new QLabel(m_container);
    m_robotLabel->setGeometry(100, 78, ROBOT_W, ROBOT_H);
    m_robotLabel->setAlignment(Qt::AlignCenter);
    m_robotLabel->setScaledContents(false);
    m_robotLabel->setStyleSheet("border: none; background: transparent;");
    m_robotLabel->show();

    // Centred: x = (341 - 141) / 2 = 100, just below the robot (y=225)
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

    m_state = State::Thinking;
    m_analyzeBtn->setEnabled(false);
    m_typeCombo->setEnabled(false);
    m_outputEdit->clear();
    m_outputEdit->setPlaceholderText("Analyse en cours, veuillez patienter…");
    startThinkingAnimation();

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
//  buildPrompt  —  assemble the full prompt text
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
               "Soyez précis, structuré et professionnel(in english, your output MUST be in 6 lines max)."
               ).arg(equipmentType, data);
}

// ═══════════════════════════════════════════════════════════════════════════════
//  callOpenAI  —  HTTP POST to the Gemini generateContent endpoint
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIPanel::callOpenAI(const QString &prompt)
{
    // 1. We use gemini-1.5-flash-latest to satisfy Google's servers
    QString urlStr = "https://generativelanguage.googleapis.com/v1/models/gemini-2.5-flash:generateContent?key=" + m_apiKey.trimmed();

    // 2. Debug print to the console so we can PROVE Qt is using our new code
    qDebug() << "--- DEBUG AI ---";
    qDebug() << "Model URL being used:" << urlStr.left(85) << "...[KEY HIDDEN]";

    // 3. Setup the request
    QNetworkRequest request((QUrl(urlStr)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    // ── Gemini JSON body ──────────────────────────────────────────────────────
    QString fullPrompt =
        "You are an expert industrial equipment management analyst "
        "with deep knowledge of maintenance planning and NLP-based "
        "text analysis. Always respond in French.\n\n" + prompt;

    QJsonObject part;
    part["text"] = fullPrompt;

    QJsonObject content;
    content["parts"] = QJsonArray{ part };

    QJsonObject generationConfig;
    generationConfig["maxOutputTokens"] = 10000;
    generationConfig["temperature"]     = 0.4;

    QJsonObject body;
    body["contents"]         = QJsonArray{ content };
    body["generationConfig"] = generationConfig;

    m_network->post(request, QJsonDocument(body).toJson(QJsonDocument::Compact));
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Slot: network reply received
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIPanel::onNetworkReply(QNetworkReply *reply)
{
    // Read body BEFORE deleteLater() — Gemini returns JSON error details
    // (bad key, quota exceeded, etc.) even for HTTP 4xx responses.
    const QByteArray raw       = reply->readAll();
    const int        httpStatus = reply->attribute(
                                      QNetworkRequest::HttpStatusCodeAttribute).toInt();

    reply->deleteLater();

    stopThinkingAnimation();
    m_typeCombo->setEnabled(true);
    m_analyzeBtn->setEnabled(true);
    m_outputEdit->setPlaceholderText(
        "Bonjour, veuillez sélectionner un type d'équipement pour commencer l'analyse.");

    // Try to parse body as JSON (works for both success and Gemini error bodies)
    QJsonParseError parseErr;
    QJsonDocument   doc        = QJsonDocument::fromJson(raw, &parseErr);
    const bool      bodyIsJson = (parseErr.error == QJsonParseError::NoError);

    // ── Network / transport-level failure (no HTTP response at all) ───────────
    if (reply->error() != QNetworkReply::NoError && httpStatus == 0) {
        m_state = State::Done;
        m_outputEdit->setPlainText(
            QString("⚠️  Erreur réseau : %1\n\n"
                    "Vérifiez votre connexion internet.\n"
                    "Si le problème persiste, SSL est peut-être absent :\n"
                    "copiez libssl-1_1-x64.dll et libcrypto-1_1-x64.dll dans le "
                    "dossier de votre exécutable.")
            .arg(reply->errorString()));
        setRobotImage("wave.png");
        return;
    }

    // ── HTTP-level error (4xx / 5xx) — read Gemini error body ────────────────
    if (reply->error() != QNetworkReply::NoError) {
        m_state = State::Done;
        QString detail;

        if (bodyIsJson && doc.object().contains("error")) {
            // Gemini error envelope: { "error": { "code": 400, "message": "...", "status": "..." } }
            const QJsonObject err = doc.object()["error"].toObject();
            const QString msg    = err["message"].toString();
            const QString status = err["status"].toString();
            const int     code   = err["code"].toInt();
            detail = QString("Status : %1\nCode   : %2\nDétail : %3")
                         .arg(status.isEmpty() ? "—" : status)
                         .arg(code)
                         .arg(msg.isEmpty() ? "—" : msg);
        } else {
            detail = QString::fromUtf8(raw).trimmed();
            if (detail.isEmpty()) detail = reply->errorString();
        }

        const bool keyError = (httpStatus == 400 || httpStatus == 401 ||
                               httpStatus == 403 ||
                               detail.contains("API_KEY", Qt::CaseInsensitive) ||
                               detail.contains("invalid", Qt::CaseInsensitive) ||
                               detail.contains("API key", Qt::CaseInsensitive));

        QString msg = QString("⚠️  Erreur HTTP %1\n\n%2").arg(httpStatus).arg(detail);
        if (keyError)
            msg += "\n\n🔑 Votre clé API Gemini semble invalide.\n"
                   "Générez-en une nouvelle sur aistudio.google.com "
                   "et mettez-la à jour dans mainwindow.cpp.";

        m_outputEdit->setPlainText(msg);
        setRobotImage("wave.png");
        return;
    }

    // ── Parse success body ────────────────────────────────────────────────────
    if (!bodyIsJson) {
        m_state = State::Done;
        m_outputEdit->setPlainText(
            QString("⚠️  Réponse JSON invalide : %1\n\nBrut :\n%2")
            .arg(parseErr.errorString(), QString::fromUtf8(raw)));
        setRobotImage("wave.png");
        return;
    }

    // API-level error inside a 200 OK
    if (doc.object().contains("error")) {
        m_state = State::Done;
        const QString apiErr =
            doc.object()["error"].toObject()["message"].toString();
        m_outputEdit->setPlainText(
            QString("⚠️  Erreur API Gemini :\n%1").arg(apiErr));
        setRobotImage("wave.png");
        return;
    }

    // ── Extract Gemini response ───────────────────────────────────────────────
    // Gemini path: candidates[0] → content → parts[0] → text
    const QJsonArray candidates = doc.object()["candidates"].toArray();
    if (candidates.isEmpty()) {
        m_state = State::Done;
        m_outputEdit->setPlainText(
            "⚠️  Aucune réponse reçue de l'API (tableau candidates vide).");
        setRobotImage("wave.png");
        return;
    }

    const QString result =
        candidates[0].toObject()["content"].toObject()
                     ["parts"].toArray()[0].toObject()
                     ["text"].toString().trimmed();

    if (result.isEmpty()) {
        m_state = State::Done;
        m_outputEdit->setPlainText("⚠️  L'IA a retourné une réponse vide.");
        setRobotImage("wave.png");
        return;
    }

    // ── Strip Markdown (QPlainTextEdit is plain-text only) ───────────────────
    QString cleaned = result;
    // Remove headers: "### Title" → "Title"
    cleaned.replace(QRegularExpression("^#{1,6}\\s*",
                        QRegularExpression::MultilineOption), "");
    // Replace horizontal rules with a visual divider
    cleaned.replace(QRegularExpression("^-{3,}\\s*$",
                        QRegularExpression::MultilineOption),
                    "────────────────────────────");
    // Remove bold/italic markers keeping inner text: **text** → text
    cleaned.replace(QRegularExpression("\\*{1,3}([^*\\n]+)\\*{1,3}"), "\\1");
    // Remove inline code backticks: `code` → code
    cleaned.replace(QRegularExpression("`([^`]+)`"), "\\1");
    // Collapse 3+ blank lines into 2
    cleaned.replace(QRegularExpression("\\n{3,}"), "\n\n");

    // ── Success ───────────────────────────────────────────────────────────────
    m_state = State::Done;
    m_outputEdit->setPlainText(cleaned.trimmed());
    setRobotImage("idea.png");
}

// ═══════════════════════════════════════════════════════════════════════════════
//  Robot image helpers
// ═══════════════════════════════════════════════════════════════════════════════
void EquipementAIPanel::setRobotImage(const QString &resourceName)
{
    const QStringList candidates = {
        ":/"           + resourceName,
        ":/images/"    + resourceName,
        ":/resources/" + resourceName,
        ":/assets/"    + resourceName,
        resourceName
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
                 << "-- tried:" << candidates;
        m_robotLabel->setPixmap(QPixmap());
        m_robotLabel->setStyleSheet(
            "font-size: 64px; border: none; background: transparent;");
        m_robotLabel->setText(
            resourceName == "wave.png" ? "🤖" :
            resourceName == "idea.png" ? "💡" : "🔄");
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