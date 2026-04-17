#ifndef EQUIPEMENTAI_H
#define EQUIPEMENTAI_H

// ═══════════════════════════════════════════════════════════════════════════════
//  EquipementAIDialog  —  AI-powered equipment recommendation dialog
//
//  Features:
//    • Loads distinct EQUIPMENT_TYPE values from the Oracle DB into a QComboBox.
//    • Queries all equipment of the chosen type (status, fabricant, notes…).
//    • Calls the OpenAI Chat Completions API (GPT-4o) with a carefully crafted
//      prompt that instructs the model to use NLP to interpret maintenance notes
//      and to recommend the single best equipment unit.
//    • Displays an animated robot PNG to indicate the current state:
//        – wave.png   : idle (no type selected yet)
//        – think1.png / think2.png : alternating every 500 ms while the API call
//                                    is in flight
//        – idea.png   : result ready
//
//  Image resources:
//    Add the following entries to your .qrc file:
//        <file alias="wave.png">  images/wave.png  </file>
//        <file alias="think1.png">images/think1.png</file>
//        <file alias="think2.png">images/think2.png</file>
//        <file alias="idea.png">  images/idea.png  </file>
//    The dialog looks them up as  :/wave.png,  :/think1.png, etc.
//    If a file is missing the label is simply left blank.
//
//  Integration (MainWindow):
//  ─────────────────────────────────────────────────────────────────────────────
//    1. Add to mainwindow.h:      #include "equipementAI.h"
//    2. In the constructor, after equipement_setupStatsUI():
//         connect(ui->btn_ai_equipement, &QPushButton::clicked, this, [this]() {
//             EquipementAIDialog dlg("sk-...", this);
//             dlg.exec();
//         });
//    Replace "sk-..." with your actual OpenAI API key, or read it from a config
//    file / environment variable.
// ═══════════════════════════════════════════════════════════════════════════════

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QString>

class EquipementAIDialog : public QDialog
{
    Q_OBJECT

public:
    // apiKey  – your OpenAI secret key ("sk-…").  Keep it out of source control;
    //           prefer reading it from an environment variable or a config file.
    explicit EquipementAIDialog(const QString &apiKey,
                                QWidget       *parent = nullptr);

private slots:
    // Called when the user picks a different equipment type.
    void onEquipmentTypeChanged(int index);

    // Triggered by the "Analyser" button.
    void onAnalyzeClicked();

    // Receives the HTTP response from the OpenAI API.
    void onNetworkReply(QNetworkReply *reply);

    // Drives the think1/think2 animation at 500 ms intervals.
    void onAnimationTimer();

private:
    // ── UI construction ───────────────────────────────────────────────────────
    void setupUI();

    // Populates m_typeCombo with distinct EQUIPMENT_TYPE values from the DB.
    void loadEquipmentTypes();

    // ── AI pipeline ───────────────────────────────────────────────────────────
    // Queries the DB for all equipment of the given type and formats the result
    // as a Markdown table string for inclusion in the prompt.
    QString fetchEquipmentData(const QString &equipmentType);

    // Assembles the full system + user prompt that is sent to the API.
    QString buildPrompt(const QString &equipmentType);

    // Fires off the HTTP POST to api.openai.com/v1/chat/completions.
    void callOpenAI(const QString &prompt);

    // ── Robot animation helpers ───────────────────────────────────────────────
    // Loads and displays the named resource image (e.g. "wave.png").
    void setRobotImage(const QString &resourceName);

    // Starts the think1/think2 alternating animation.
    void startThinkingAnimation();

    // Stops the animation and leaves the last frame in place.
    void stopThinkingAnimation();

    // ── State machine ─────────────────────────────────────────────────────────
    enum class State { Idle, Thinking, Done };
    State m_state = State::Idle;

    // ── Widgets ───────────────────────────────────────────────────────────────
    QLabel        *m_robotLabel;     // displays wave / think / idea PNG
    QComboBox     *m_typeCombo;      // equipment-type picker
    QPushButton   *m_analyzeBtn;     // "Analyser" button
    QPlainTextEdit *m_outputEdit;    // read-only AI recommendation text
    QLabel        *m_statusLabel;    // small status line below the button

    // ── Animation ─────────────────────────────────────────────────────────────
    QTimer *m_animTimer;
    bool    m_animPhase = false;     // false → think1.png, true → think2.png

    // ── Network ───────────────────────────────────────────────────────────────
    QNetworkAccessManager *m_network;
    QString                m_apiKey;

    // Robot image dimensions (px)
    static constexpr int ROBOT_W = 140;
    static constexpr int ROBOT_H = 140;
};

#endif // EQUIPEMENTAI_H
