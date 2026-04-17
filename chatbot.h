#ifndef CHATBOT_H
#define CHATBOT_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollArea>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPropertyAnimation>

class Chatbot : public QWidget
{
    Q_OBJECT

public:
    explicit Chatbot(QWidget *parent = nullptr);
    ~Chatbot();

    void addMessage(const QString &sender, const QString &message, bool isUser);
    void setApiKey(const QString &key) { m_apiKey = key; }

private slots:
    void onSendClicked();
    void handleApiResponse();

private:
    void setupUI();
    QString getDatabaseContext();
    void sendToLlama(const QString &userMessage);

    QVBoxLayout *m_chatLayout;
    QScrollArea *m_scrollArea;
    QWidget *m_scrollContent;
    QLineEdit *m_inputField;
    QPushButton *m_sendButton;
    QNetworkAccessManager *m_networkManager;
    QString m_apiKey;
    bool m_isWaiting;
};

#endif // CHATBOT_H
