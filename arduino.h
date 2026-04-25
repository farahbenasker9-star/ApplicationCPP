#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QString>
#include <QStyledItemDelegate>
#include <QAbstractItemView>
#include <QPainter>
#include <QStyle>
#include <QDebug>
#include <QMessageBox>
#include <QRegularExpression>

// ═══════════════════════════════════════════════════════════════════════════════
//  CLASSE 0 : Délégué d'affichage pour la Poubelle (UI)
// ═══════════════════════════════════════════════════════════════════════════════
class PoubelleAlertDelegate : public QStyledItemDelegate
{
public:
    explicit PoubelleAlertDelegate(QAbstractItemView *view, QObject *parent = nullptr);
    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
private:
    QAbstractItemView *m_view;
    static constexpr int ALERT_THRESHOLD = 80;
};

// ═══════════════════════════════════════════════════════════════════════════════
//  CLASSE 1 : ArduinoMonitor (Gestion de la Poubelle)
// ═══════════════════════════════════════════════════════════════════════════════
class ArduinoMonitor : public QObject
{
    Q_OBJECT
public:
    explicit ArduinoMonitor(int binId, const QString &portName, QObject *parent = nullptr);
    ~ArduinoMonitor() override;

    bool start();
    void stop();
    bool isRunning() const;
    QString errorString() const { return m_lastError; }

    void setBinId(int id) { m_binId = id; }
    int  binId() const { return m_binId; }
    void setThreshold(int pct) { m_threshold = pct; }
    int  threshold() const { return m_threshold; }

signals:
    void levelUpdated(int binId, int level);
    void alertTriggered(int binId, int level);
    void serialError(const QString &message);

private slots:
    void onReadyRead();
    void onSerialError(QSerialPort::SerialPortError error);

private:
    void updateDatabase(int level);

    int           m_binId;
    int           m_threshold  = 80;
    bool          m_alertFired = false;
    QString       m_portName;
    QString       m_lastError;
    QSerialPort  *m_serial     = nullptr;
    QByteArray    m_readBuffer;
};

// ═══════════════════════════════════════════════════════════════════════════════
//  CLASSE 2 : ArduinoRFID (Gestion du Lecteur de Badge)
// ═══════════════════════════════════════════════════════════════════════════════
class ArduinoRFID : public QObject
{
    Q_OBJECT
public:
    explicit ArduinoRFID(QObject *parent = nullptr);
    ~ArduinoRFID();

    // Permet de choisir le port (ex: "COM4")
    void setupArduino(const QString &portName);
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

// ═══════════════════════════════════════════════════════════════════════════════
//  CLASSE 3 : ArduinoClient (Gestion du Clavier et de l'Audio du collègue)
// ═══════════════════════════════════════════════════════════════════════════════
class ArduinoClient : public QObject
{
    Q_OBJECT
public:
    explicit ArduinoClient(QObject *parent = nullptr);
    ~ArduinoClient();

    // Permet de choisir le port (ex: "COM5")
    void connectArduino(const QString &portName);
    void closeArduino();
    void writeToArduino(const QByteArray &data);
    bool isConnected() const;

signals:
    void numeroRecu(const QString &numero);

private slots:
    void readFromArduino();

private:
    QSerialPort *serial;
    bool connected;
};

#endif // ARDUINO_H
