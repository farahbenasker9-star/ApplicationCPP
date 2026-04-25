#ifndef EQUIPEMENTAI_H
#define EQUIPEMENTAI_H

// ═══════════════════════════════════════════════════════════════════════════════
//  EquipementAIPanel  —  AI-powered equipment recommendation panel
//
//  This is NOT a dialog.  It wires itself into three existing UI widgets that
//  are already defined in mainwindow.ui inside the "Assignation Optimale (AI)"
//  tab:
//
//      ui->Eq_type_2     (QComboBox)      — user picks the equipment type
//      ui->plainTextEdit (QPlainTextEdit) — receives the AI recommendation
//      ui->gb_stats_equipement_2 (QGroupBox / QWidget) — container; the panel
//                                          creates a robot QLabel and an
//                                          "Analyser" QPushButton inside it.
//
//  Integration in MainWindow constructor
//  ─────────────────────────────────────
//      #include "equipementAI.h"           // add to mainwindow.cpp includes
//
//      // after  equipement_refreshStats();  add:
//      new EquipementAIPanel(
//              "sk-YOUR-KEY-HERE",          // OpenAI secret key
//              ui->Eq_type_2,
//              ui->plainTextEdit,
//              ui->gb_stats_equipement_2,
//              this);                       // parent (MainWindow owns it)
//
//  Resource images (add to your .qrc):
//      <file alias="wave.png">  images/wave.png  </file>
//      <file alias="think1.png">images/think1.png</file>
//      <file alias="think2.png">images/think2.png</file>
//      <file alias="idea.png">  images/idea.png  </file>
//  Accessed as  :/wave.png,  :/think1.png, etc.
//  If an image is missing the label shows a fallback emoji — no crash.
// ═══════════════════════════════════════════════════════════════════════════════

#include <QObject>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QString>
#include <QWidget>

class EquipementAIPanel : public QObject
{
    Q_OBJECT

public:
    // apiKey      – OpenAI secret key ("sk-…").  Never hard-code in production;
    //               read it from an env-var or a .ini config file instead.
    // typeCombo   – ui->Eq_type_2
    // outputEdit  – ui->plainTextEdit
    // container   – ui->gb_stats_equipement_2  (used to embed the robot label
    //               and the Analyser button at fixed geometry positions)
    // parent      – usual QObject parent (e.g. the MainWindow instance)
    explicit EquipementAIPanel(const QString   &apiKey,
                               QComboBox       *typeCombo,
                               QPlainTextEdit  *outputEdit,
                               QWidget         *container,
                               QObject         *parent = nullptr);

private slots:
    // Fires when the user picks a different item in Eq_type_2.
    void onEquipmentTypeChanged(int index);

    // Fires when the "Analyser" button is clicked.
    void onAnalyzeClicked();

    // Receives the OpenAI HTTP response.
    void onNetworkReply(QNetworkReply *reply);

    // Drives the think1 / think2 frame alternation at 500 ms.
    void onAnimationTimer();

private:
    // ── Setup ─────────────────────────────────────────────────────────────────
    // Repopulates m_typeCombo from the DB (clears the hardcoded .ui items).
    void loadEquipmentTypes();

    // Creates the robot QLabel and Analyser button inside m_container.
    void buildEmbeddedWidgets();

    // ── AI pipeline ───────────────────────────────────────────────────────────
    QString fetchEquipmentData(const QString &equipmentType);
    QString buildPrompt(const QString &equipmentType);
    void    callOpenAI(const QString &prompt);

    // ── Robot animation ───────────────────────────────────────────────────────
    // Loads ":/<name>" from Qt resources; falls back to emoji on failure.
    void setRobotImage(const QString &resourceName);
    void startThinkingAnimation();
    void stopThinkingAnimation();

    // ── State ─────────────────────────────────────────────────────────────────
    enum class State { Idle, Thinking, Done };
    State m_state = State::Idle;

    // ── Pointers to existing UI widgets ──────────────────────────────────────
    QComboBox      *m_typeCombo;
    QPlainTextEdit *m_outputEdit;
    QWidget        *m_container;

    // ── Dynamically created widgets (live inside m_container) ─────────────────
    QLabel      *m_robotLabel  = nullptr;
    QPushButton *m_analyzeBtn  = nullptr;

    // ── Animation ─────────────────────────────────────────────────────────────
    QTimer *m_animTimer;
    bool    m_animPhase = false;   // false → think1.png, true → think2.png

    // ── Network ───────────────────────────────────────────────────────────────
    QNetworkAccessManager *m_network;
    QString                m_apiKey;

    // Robot image size (px) — fits inside the ~190 px gap in the UI
    static constexpr int ROBOT_W = 140;
    static constexpr int ROBOT_H = 140;
};

#endif // EQUIPEMENTAI_H
