#ifndef ARDUINO_H
#define ARDUINO_H

// ═══════════════════════════════════════════════════════════════════════════════
//  ArduinoMonitor  —  Serial reader + DB updater for the HC-SR04 bin sensor
//
//  Responsibilities:
//    1. Open a QSerialPort at 9600 baud (same as the Arduino sketch).
//    2. Read each newline-terminated integer the Arduino sends (0-100).
//    3. Write the new NIVEAU_REMPLISSAGE to the DB for the watched bin ID.
//    4. Emit levelUpdated() every tick so the UI can stay in sync.
//    5. Emit alertTriggered() the FIRST time the level reaches the threshold
//       in a rising edge (avoids spamming the popup every second).
//
//  Required .pro addition:
//      QT += serialport
//
//  Integration (MainWindow constructor, after poubelle table setup):
//  ──────────────────────────────────────────────────────────────────
//      #include "arduinomonitor.h"
//
//      // Install red-highlight delegate on the bin table
//      ui->tab_poubelle->setItemDelegate(
//          new PoubelleAlertDelegate(ui->tab_poubelle, this));
//
//      // Start monitoring bin ID 1 on COM3 (Windows) or /dev/ttyUSB0 (Linux)
//      m_arduino = new ArduinoMonitor(1, "COM3", this);
//      connect(m_arduino, &ArduinoMonitor::levelUpdated,
//              this, &MainWindow::onArduinoLevelUpdated);
//      connect(m_arduino, &ArduinoMonitor::alertTriggered,
//              this, &MainWindow::onArduinoAlert);
//      m_arduino->start();
// ═══════════════════════════════════════════════════════════════════════════════

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QString>
#include <QStyledItemDelegate>
#include <QAbstractItemView>
#include <QPainter>
#include <QStyle>

// ─────────────────────────────────────────────────────────────────────────────
//  PoubelleAlertDelegate
//  Paints any row whose NIVEAU_REMPLISSAGE (column 5) >= 80 with a red tint.
//  Install once on tab_poubelle; it survives future setModel() calls.
// ─────────────────────────────────────────────────────────────────────────────
class PoubelleAlertDelegate : public QStyledItemDelegate
{
public:
    explicit PoubelleAlertDelegate(QAbstractItemView *view,
                                   QObject *parent = nullptr)
        : QStyledItemDelegate(parent), m_view(view)
    {}

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option,
               const QModelIndex &index) const override
    {
        // Column 5 = NIVEAU_REMPLISSAGE
        const int level = index.model()
                              ? index.model()->index(index.row(), 5).data(Qt::EditRole).toInt()
                              : -1;

        QStyleOptionViewItem opt = option;
        initStyleOption(&opt, index);

        if (level >= 80) {
            // Red tint background
            const QColor bg = (opt.state & QStyle::State_Selected)
                                  ? QColor(200, 50, 50)   // darker red when selected
                                  : QColor(255, 160, 160); // soft red

            painter->save();
            painter->fillRect(opt.rect, bg);
            painter->restore();

            // Strip selection so Qt doesn't overwrite our background
            opt.state  &= ~QStyle::State_Selected;
            opt.features &= ~QStyleOptionViewItem::Alternate;
            opt.backgroundBrush = QBrush(Qt::transparent);
        }

        QStyledItemDelegate::paint(painter, opt, index);
    }

private:
    QAbstractItemView *m_view;
    static constexpr int ALERT_THRESHOLD = 80;
};


// ─────────────────────────────────────────────────────────────────────────────
//  ArduinoMonitor
// ─────────────────────────────────────────────────────────────────────────────
class ArduinoMonitor : public QObject
{
    Q_OBJECT

public:
    // binId      – POUBELLE_INTELLIGENTE.ID_POUBELLE to update
    // portName   – "COM3", "COM4" … on Windows | "/dev/ttyUSB0" on Linux/Mac
    // parent     – QObject parent (typically the MainWindow)
    explicit ArduinoMonitor(int           binId,
                            const QString &portName,
                            QObject       *parent = nullptr);
    ~ArduinoMonitor() override;

    // Opens the serial port and starts listening.
    // Returns false if the port cannot be opened (check errorString()).
    bool start();

    // Closes the port gracefully.
    void stop();

    bool isRunning() const;
    QString errorString() const { return m_lastError; }

    // Change which bin is being monitored (takes effect immediately).
    void setBinId(int id) { m_binId = id; }
    int  binId()    const { return m_binId; }

    // Change the alert threshold (default 80 %).
    void setThreshold(int pct) { m_threshold = pct; }
    int  threshold()     const { return m_threshold; }

signals:
    // Emitted every time the Arduino sends a new reading.
    // level is guaranteed to be in [0, 100].
    void levelUpdated(int binId, int level);

    // Emitted on the RISING EDGE when level first crosses the threshold.
    // Not repeated until level drops back below threshold first.
    void alertTriggered(int binId, int level);

    // Emitted on any serial port error.
    void serialError(const QString &message);

private slots:
    void onReadyRead();
    void onSerialError(QSerialPort::SerialPortError error);

private:
    // Writes the new fill level to the DB.
    void updateDatabase(int level);

    int           m_binId;
    int           m_threshold  = 80;
    bool          m_alertFired = false;   // rising-edge guard
    QString       m_portName;
    QString       m_lastError;
    QSerialPort  *m_serial     = nullptr;
    QByteArray    m_readBuffer;
};


// ─────────────────────────────────────────────────────────────────────────────
//  Arduino  —  Serial reader for RFID UID
// ─────────────────────────────────────────────────────────────────────────────
class Arduino : public QObject
{
    Q_OBJECT
public:
    explicit Arduino(QObject *parent = nullptr);
    ~Arduino();

    void setupArduino();
    void write(const QByteArray &data);

signals:
    void uidRead(const QString &uid);

private slots:
    void readData();

private:
    QSerialPort *serial;
    QString port_name;
    bool is_available;
    QByteArray serialData;
};

#endif // ARDUINO_H
