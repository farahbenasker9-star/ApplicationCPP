#include "chatbot.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>
#include <QScrollBar>
#include <QTimer>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QSslSocket>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QSqlRecord>

Chatbot::Chatbot(QWidget *parent)
    : QWidget(parent)
    , m_isWaiting(false)
{
    m_apiKey = "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";
    m_networkManager = new QNetworkAccessManager(this);
    this->setFixedWidth(380);
    setupUI();
    
    addMessage("EcoBot", "Bonjour! Je suis EcoBot Experts. J'ai maintenant un accès complet aux détails de votre base de données EcoCycle. Posez-moi vos questions précises sur les employés, contrats, équipements ou produits !", false);
}

Chatbot::~Chatbot()
{
}

void Chatbot::setupUI()
{
    QVBoxLayout *outerLayout = new QVBoxLayout(this);
    outerLayout->setContentsMargins(10, 10, 10, 10);

    QFrame *mainFrame = new QFrame();
    mainFrame->setObjectName("mainFrame");
    mainFrame->setStyleSheet(
        "QFrame#mainFrame { background-color: #ffffff; border-radius: 20px; border: 1px solid #e0e0e0; }"
    );

    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect();
    shadow->setBlurRadius(20);
    shadow->setXOffset(0);
    shadow->setYOffset(5);
    shadow->setColor(QColor(0, 0, 0, 40));
    mainFrame->setGraphicsEffect(shadow);

    QVBoxLayout *mainLayout = new QVBoxLayout(mainFrame);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    QFrame *header = new QFrame();
    header->setFixedHeight(70);
    header->setStyleSheet(
        "QFrame { background: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #1b5e20, stop:1 #2e7d32); border-top-left-radius: 20px; border-top-right-radius: 20px; }"
    );
    
    QHBoxLayout *headerLayout = new QHBoxLayout(header);
    headerLayout->setContentsMargins(20, 0, 20, 0);

    QWidget *statusDot = new QWidget();
    statusDot->setFixedSize(10, 10);
    statusDot->setStyleSheet("background-color: #4dfd4d; border-radius: 5px;");
    
    QLabel *title = new QLabel("EcoBot Expert AI");
    title->setStyleSheet("font-weight: bold; font-size: 18px; color: white; background: transparent;");
    
    headerLayout->addWidget(statusDot);
    headerLayout->addWidget(title);
    headerLayout->addStretch();
    
    mainLayout->addWidget(header);

    m_scrollArea = new QScrollArea();
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setStyleSheet("background: transparent; border: none;");
    
    m_scrollContent = new QWidget();
    m_scrollContent->setStyleSheet("background: transparent;");
    m_chatLayout = new QVBoxLayout(m_scrollContent);
    m_chatLayout->setAlignment(Qt::AlignTop);
    m_chatLayout->setContentsMargins(15, 20, 15, 20);
    m_chatLayout->setSpacing(15);
    
    m_scrollArea->setWidget(m_scrollContent);
    mainLayout->addWidget(m_scrollArea, 1);

    QFrame *inputFrame = new QFrame();
    inputFrame->setFixedHeight(80);
    inputFrame->setStyleSheet("background-color: #f8f9fa; border-bottom-left-radius: 20px; border-bottom-right-radius: 20px; border-top: 1px solid #eee;");
    
    QHBoxLayout *inputLayout = new QHBoxLayout(inputFrame);
    inputLayout->setContentsMargins(15, 10, 15, 15);
    inputLayout->setSpacing(10);

    m_inputField = new QLineEdit();
    m_inputField->setPlaceholderText("Question sur la base de données...");
    m_inputField->setStyleSheet(
        "QLineEdit { border: 1px solid #ddd; border-radius: 22px; padding: 8px 18px; background-color: white; font-size: 14px; color: #333; }"
        "QLineEdit:focus { border: 2px solid #2E7D32; }"
    );
    connect(m_inputField, &QLineEdit::returnPressed, this, &Chatbot::onSendClicked);
    
    m_sendButton = new QPushButton("➤");
    m_sendButton->setFixedSize(44, 44);
    m_sendButton->setCursor(Qt::PointingHandCursor);
    m_sendButton->setStyleSheet(
        "QPushButton { background-color: #2E7D32; color: white; border-radius: 22px; font-size: 18px; border: none; }"
        "QPushButton:hover { background-color: #1b5e20; }"
        "QPushButton:disabled { background-color: #ccc; }"
    );
    connect(m_sendButton, &QPushButton::clicked, this, &Chatbot::onSendClicked);

    inputLayout->addWidget(m_inputField);
    inputLayout->addWidget(m_sendButton);
    
    mainLayout->addWidget(inputFrame);
    outerLayout->addWidget(mainFrame);
    this->setLayout(outerLayout);
}

void Chatbot::addMessage(const QString &sender, const QString &message, bool isUser)
{
    QWidget *msgContainer = new QWidget();
    QHBoxLayout *msgLayout = new QHBoxLayout(msgContainer);
    msgLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *msgLabel = new QLabel(message);
    msgLabel->setWordWrap(true);
    msgLabel->setMaximumWidth(260);
    msgLabel->setTextFormat(Qt::MarkdownText);
    
    if (isUser) {
        msgLabel->setStyleSheet("background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #2E7D32, stop:1 #1b5e20); color: white; border-radius: 18px; border-top-right-radius: 4px; padding: 12px 16px; font-size: 13px;");
        msgLayout->addStretch();
        msgLayout->addWidget(msgLabel);
    } else {
        msgLabel->setStyleSheet("background-color: #f1f3f5; color: #212529; border-radius: 18px; border-top-left-radius: 4px; padding: 12px 16px; font-size: 13px; border: 1px solid #e9ecef;");
        msgLayout->addWidget(msgLabel);
        msgLayout->addStretch();
    }

    m_chatLayout->addWidget(msgContainer);
    QTimer::singleShot(100, [this]() { m_scrollArea->verticalScrollBar()->setValue(m_scrollArea->verticalScrollBar()->maximum()); });
}

void Chatbot::onSendClicked()
{
    QString text = m_inputField->text().trimmed();
    if (text.isEmpty() || m_isWaiting) return;
    m_inputField->clear();
    addMessage("Vous", text, true);
    m_isWaiting = true;
    m_sendButton->setEnabled(false);
    sendToLlama(text);
}

QString fetchTableSample(const QString &tableName, int limit = 200) {
    QSqlQuery q;
    // Check if table exists to avoid SQL errors fed to the LLM
    if (!q.exec(QString("SELECT count(*) FROM %1").arg(tableName))) {
        return "";
    }
    
    QString result = QString("\n--- TABLE %1 ---\n").arg(tableName);
    
    if (q.exec(QString("SELECT * FROM %1 WHERE ROWNUM <= %2").arg(tableName).arg(limit))) {
        QSqlRecord rec = q.record();
        // Headers
        for (int i = 0; i < rec.count(); ++i) {
            result += rec.fieldName(i) + (i == rec.count() - 1 ? "" : " | ");
        }
        result += "\n";
        
        // Data
        while (q.next()) {
            for (int i = 0; i < rec.count(); ++i) {
                result += q.value(i).toString() + (i == rec.count() - 1 ? "" : " | ");
            }
            result += "\n";
        }
    }
    return result;
}

QString Chatbot::getDatabaseContext()
{
    QString context = "[ECOCYCLE SYSTEM BRAIN DUMP]\n";
    context += fetchTableSample("CLIENT");
    context += fetchTableSample("CONTRAT");
    context += fetchTableSample("EMPLOYE");
    context += fetchTableSample("EQUIPMENT"); // Le nom réel en BDD
    context += fetchTableSample("POUBELLE_INTELLIGENTE"); // Le nom réel en BDD
    context += fetchTableSample("PRODUIT");
    return context;
}

void Chatbot::sendToLlama(const QString &userMessage)
{
    QString context = getDatabaseContext();
    
    QNetworkRequest request(QUrl("https://openrouter.ai/api/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer " + m_apiKey.toUtf8());
    request.setRawHeader("HTTP-Referer", "http://localhost");
    request.setRawHeader("X-Title", "EcoCycleApp");

    QJsonObject systemMsg;
    systemMsg["role"] = "system";
    systemMsg["content"] = 
        "Tu es EcoBot, l'Assistant Virtuel Professionnel d'EcoCycle. Tu as accès aux données exactes des tables : Employé, Client, Contrat, Produit, Équipement, Poubelle.\n"
        "RÈGLES DE COMMUNICATION :\n"
        "1. ADOPTE UN TON PROFESSIONNEL : Formule tes réponses sous forme de phrases claires, polies et structurées (ex: 'Bonjour, le système compte actuellement 3 équipements.').\n"
        "2. NI TROP BREF, NI TROP DÉTAILLÉ : Ne réponds pas juste avec un mot ou chiffre, mais évite de faire de longs paragraphes. Va à l'essentiel avec courtoisie.\n"
        "3. PRÉCISION ABSOLUE : Utilise scrupuleusement les données pour répondre aux questions sur n'importe quel élément de la BDD.\n"
        "4. AUCUNE TRACE DE CALCUL : Ne montre jamais tes étapes de réflexion.\n"
        "5. HORS SUJET : Si la question ne concerne pas EcoCycle, réponds poliment : 'Je suis navré, mais je gère uniquement les données liées à EcoCycle.'";

    QJsonObject userMsg;
    userMsg["role"] = "user";
    userMsg["content"] = QString("CONTEXTE DATA:\n%1\n\nQUESTION: %2").arg(context).arg(userMessage);

    QJsonArray messages;
    messages.append(systemMsg);
    messages.append(userMsg);

    QJsonObject body;
    // deepseek-chat est la version ultra-rapide (sans 20s d'attente de réflexion)
    body["model"] = "deepseek/deepseek-chat";
    body["messages"] = messages;

    QNetworkReply *reply = m_networkManager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, &Chatbot::handleApiResponse);
}

void Chatbot::handleApiResponse()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;

    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument response = QJsonDocument::fromJson(reply->readAll());
        QString answer = response.object()["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();
        
        // Supprimer le bloc de réflexion (think) si on utilise par erreur DeepSeek-R1
        int thinkIndex = answer.indexOf("</think>");
        if (thinkIndex != -1) {
            answer = answer.mid(thinkIndex + 8).trimmed();
        }
        
        addMessage("EcoBot", answer, false);
    } else {
        QString err = reply->errorString();
        int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        addMessage("EcoBot", QString("⚠️ Erreur (%1): %2").arg(statusCode).arg(err), false);
        if (!QSslSocket::supportsSsl()) addMessage("EcoBot", "💡 Erreur SSL détectée.", false);
        else if (statusCode == 429) addMessage("EcoBot", "💡 Service saturé, réessayez dans 1 min.", false);
    }

    m_isWaiting = false;
    m_sendButton->setEnabled(true);
    reply->deleteLater();
}
