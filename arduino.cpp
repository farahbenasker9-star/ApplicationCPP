/*#include "arduino.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// ═══════════════════════════════════════════════════════════════════════════════
//  ArduinoMonitor — Constructor / Destructor
// ═══════════════════════════════════════════════════════════════════════════════
ArduinoMonitor::ArduinoMonitor(int binId, const QString &portName, QObject *parent)
    : QObject(parent)
    , m_binId(binId)
    , m_portName(portName)
    , m_serial(new QSerialPort(this))
{
    // Mirror the Arduino sketch: 9600 baud, 8-N-1
    m_serial->setPortName(m_portName);
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(m_serial, &QSerialPort::readyRead,
            this,     &ArduinoMonitor::onReadyRead);
    connect(m_serial, &QSerialPort::errorOccurred,
            this,     &ArduinoMonitor::onSerialError);
}

ArduinoMonitor::~ArduinoMonitor()
{
    stop();
}

// ═══════════════════════════════════════════════════════════════════════════════
//  start / stop
// ═══════════════════════════════════════════════════════════════════════════════
bool ArduinoMonitor::start()
{
    if (m_serial->isOpen()) return true;

    if (!m_serial->open(QIODevice::ReadOnly)) {
        m_lastError = QString("Impossible d'ouvrir %1 : %2")
        .arg(m_portName, m_serial->errorString());
        qDebug() << "ArduinoMonitor::start -" << m_lastError;
        emit serialError(m_lastError);
        return false;
    }

    qDebug() << "ArduinoMonitor: port" << m_portName << "ouvert. Surveillance bin" << m_binId;
    return true;
}

void ArduinoMonitor::stop()
{
    if (m_serial && m_serial->isOpen()) {
        m_serial->close();
        qDebug() << "ArduinoMonitor: port" << m_portName << "fermé.";
    }
}

bool ArduinoMonitor::isRunning() const
{
    return m_serial && m_serial->isOpen();
}

// ═══════════════════════════════════════════════════════════════════════════════
//  onReadyRead  —  called by Qt whenever bytes arrive on the serial port
// ═══════════════════════════════════════════════════════════════════════════════
void ArduinoMonitor::onReadyRead()
{
    // Accumulate bytes into a buffer; process complete lines only.
    // The Arduino sketch does Serial.println(niveau) which appends "\r\n".
    m_readBuffer += m_serial->readAll();

    while (true) {
        // Find the first newline in the buffer
        int nlPos = m_readBuffer.indexOf('\n');
        if (nlPos < 0) break;   // no complete line yet — wait for more data

        // Extract the line (trim \r\n / whitespace)
        const QByteArray line = m_readBuffer.left(nlPos).trimmed();
        m_readBuffer.remove(0, nlPos + 1);

        if (line.isEmpty()) continue;

        // Parse the integer sent by the Arduino
        bool ok = false;
        int level = line.toInt(&ok);
        if (!ok) {
            qDebug() << "ArduinoMonitor: valeur non-entière reçue:" << line;
            continue;
        }

        // Clamp to [0, 100] (defensive; the Arduino already does this)
        level = qBound(0, level, 100);

        qDebug() << "ArduinoMonitor: bin" << m_binId << "→ niveau" << level << "%";

        // ── 1. Persist to the database ────────────────────────────────────────
        updateDatabase(level);

        // ── 2. Notify listeners (table refresh, form sync, …) ────────────────
        emit levelUpdated(m_binId, level);

        // ── 3. Rising-edge alert ──────────────────────────────────────────────
        if (level >= m_threshold) {
            if (!m_alertFired) {
                m_alertFired = true;
                emit alertTriggered(m_binId, level);
            }
        } else {
            // Level dropped back below threshold — allow the alert to fire again
            m_alertFired = false;
        }
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  updateDatabase  —  UPDATE NIVEAU_REMPLISSAGE for the watched bin
// ═══════════════════════════════════════════════════════════════════════════════
void ArduinoMonitor::updateDatabase(int level)
{
    QSqlQuery q;
    q.prepare("UPDATE POUBELLE_INTELLIGENTE "
              "SET    NIVEAU_REMPLISSAGE = :niv "
              "WHERE  ID_POUBELLE = :id");
    q.bindValue(":niv", level);
    q.bindValue(":id",  m_binId);

    if (!q.exec()) {
        const QString err = q.lastError().text();
        qDebug() << "ArduinoMonitor::updateDatabase error:" << err;
        emit serialError(QString("Erreur DB lors de la mise à jour: %1").arg(err));
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//  onSerialError  —  QSerialPort error passthrough
// ═══════════════════════════════════════════════════════════════════════════════
void ArduinoMonitor::onSerialError(QSerialPort::SerialPortError error)
{
    // NoError is emitted on successful open — ignore it
    if (error == QSerialPort::NoError) return;

    m_lastError = m_serial->errorString();
    qDebug() << "ArduinoMonitor: erreur port série:" << m_lastError;
    emit serialError(m_lastError);
}


// ═══════════════════════════════════════════════════════════════════════════════
//  Arduino — Constructor / Destructor
// ═══════════════════════════════════════════════════════════════════════════════
Arduino::Arduino(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    is_available = false;
    port_name = "";
}

Arduino::~Arduino()
{
    if (serial->isOpen()) {
        serial->close();
    }
}

void Arduino::setupArduino()
{
    // 1. DÉFINISSEZ LE PORT DE LA RFID ICI :
    QString portRFID = "COM12";

    // 2. Chercher si ce port est bien branché
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        if (serialPortInfo.portName() == portRFID) {
            qDebug() << "Port RFID detecté:" << serialPortInfo.portName();
            port_name = serialPortInfo.portName();
            is_available = true;
            break;
        }
    }

    // 3. Ouvrir le port
    if (is_available) {
        serial->setPortName(port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            connect(serial, &QSerialPort::readyRead, this, &Arduino::readData);
            qDebug() << "Arduino RFID connecté avec succès au port" << port_name;
        } else {
            qDebug() << "Impossible d'ouvrir le port série RFID :" << serial->errorString();
        }
    } else {
        qDebug() << "Arduino RFID non trouvé sur le port" << portRFID;
    }
}
void Arduino::readData()
{
    QByteArray data = serial->readAll();
    qDebug() << "Trame brute reçue :" << data;
    serialData += data;

    while (serialData.contains('\n')) {
        int index = serialData.indexOf('\n');
        QString ligne = QString::fromUtf8(serialData.left(index)).trimmed();
        serialData.remove(0, index + 1);

        if (ligne.isEmpty()) continue;

        if (ligne == "Systeme pret...") {
            qDebug() << "Arduino est bien initialisé.";
            continue;
        }

        qDebug() << "=== UID COMPLET LU ===" << ligne;
        emit uidRead(ligne);
    }
}

void Arduino::write(const QByteArray &data)
{
    if(serial->isWritable()){
        serial->write(data);
        serial->flush();
    }else{
        qDebug() << "Impossible d'écrire sur l'Arduino.";
    }
}*/
#include "arduino.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

// =============================================================================
//  CLASSE 0 : PoubelleAlertDelegate
// =============================================================================
PoubelleAlertDelegate::PoubelleAlertDelegate(QAbstractItemView *view, QObject *parent)
    : QStyledItemDelegate(parent), m_view(view) {}

void PoubelleAlertDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const int level = index.model() ? index.model()->index(index.row(), 5).data(Qt::EditRole).toInt() : -1;
    QStyleOptionViewItem opt = option;
    initStyleOption(&opt, index);

    if (level >= ALERT_THRESHOLD) {
        const QColor bg = (opt.state & QStyle::State_Selected) ? QColor(200, 50, 50) : QColor(255, 160, 160);
        painter->save();
        painter->fillRect(opt.rect, bg);
        painter->restore();
        opt.state &= ~QStyle::State_Selected;
        opt.features &= ~QStyleOptionViewItem::Alternate;
        opt.backgroundBrush = QBrush(Qt::transparent);
    }
    QStyledItemDelegate::paint(painter, opt, index);
}

// =============================================================================
//  CLASSE 1 : ArduinoMonitor (Poubelle)
// =============================================================================
ArduinoMonitor::ArduinoMonitor(int binId, const QString &portName, QObject *parent)
    : QObject(parent), m_binId(binId), m_portName(portName), m_serial(new QSerialPort(this))
{
    m_serial->setPortName(m_portName);
    m_serial->setBaudRate(QSerialPort::Baud9600);
    m_serial->setDataBits(QSerialPort::Data8);
    m_serial->setParity(QSerialPort::NoParity);
    m_serial->setStopBits(QSerialPort::OneStop);
    m_serial->setFlowControl(QSerialPort::NoFlowControl);

    connect(m_serial, &QSerialPort::readyRead, this, &ArduinoMonitor::onReadyRead);
    connect(m_serial, &QSerialPort::errorOccurred, this, &ArduinoMonitor::onSerialError);
}

ArduinoMonitor::~ArduinoMonitor() { stop(); }

bool ArduinoMonitor::start()
{
    if (m_serial->isOpen()) return true;
    if (!m_serial->open(QIODevice::ReadOnly)) {
        m_lastError = QString("Impossible d'ouvrir %1 : %2").arg(m_portName, m_serial->errorString());
        emit serialError(m_lastError);
        return false;
    }
    qDebug() << "Arduino Poubelle: port" << m_portName << "ouvert. Surveillance bin" << m_binId;
    return true;
}

void ArduinoMonitor::stop()
{
    if (m_serial && m_serial->isOpen()) m_serial->close();
}

bool ArduinoMonitor::isRunning() const { return m_serial && m_serial->isOpen(); }

void ArduinoMonitor::onReadyRead()
{
    m_readBuffer += m_serial->readAll();
    while (true) {
        int nlPos = m_readBuffer.indexOf('\n');
        if (nlPos < 0) break;

        const QByteArray line = m_readBuffer.left(nlPos).trimmed();
        m_readBuffer.remove(0, nlPos + 1);
        if (line.isEmpty()) continue;

        bool ok = false;
        int level = line.toInt(&ok);
        if (!ok) continue;

        level = qBound(0, level, 100);
        updateDatabase(level);
        emit levelUpdated(m_binId, level);

        if (level >= m_threshold) {
            if (!m_alertFired) {
                m_alertFired = true;
                emit alertTriggered(m_binId, level);
            }
        } else {
            m_alertFired = false;
        }
    }
}

void ArduinoMonitor::updateDatabase(int level)
{
    QSqlQuery q;
    q.prepare("UPDATE POUBELLE_INTELLIGENTE SET NIVEAU_REMPLISSAGE = :niv WHERE ID_POUBELLE = :id");
    q.bindValue(":niv", level);
    q.bindValue(":id", m_binId);
    if (!q.exec()) emit serialError("Erreur DB lors de la mise à jour");
}

void ArduinoMonitor::onSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::NoError) return;
    m_lastError = m_serial->errorString();
    emit serialError(m_lastError);
}

// =============================================================================
//  CLASSE 2 : ArduinoRFID
// =============================================================================
ArduinoRFID::ArduinoRFID(QObject *parent) : QObject(parent)
{
    serial = new QSerialPort(this);
    is_available = false;
}

ArduinoRFID::~ArduinoRFID()
{
    if (serial->isOpen()) serial->close();
}

void ArduinoRFID::setupArduino(const QString &portName)
{
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.portName() == portName) {
            port_name = info.portName();
            is_available = true;
            break;
        }
    }

    if (is_available) {
        serial->setPortName(port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            connect(serial, &QSerialPort::readyRead, this, &ArduinoRFID::readData);
            qDebug() << "Arduino RFID connecté au port" << port_name;
        }
    } else {
        qDebug() << "Arduino RFID non trouvé sur le port" << portName;
    }
}

void ArduinoRFID::readData()
{
    serialData += serial->readAll();
    while (serialData.contains('\n')) {
        int index = serialData.indexOf('\n');
        QString ligne = QString::fromUtf8(serialData.left(index)).trimmed();
        serialData.remove(0, index + 1);

        if (ligne.isEmpty() || ligne == "Systeme pret...") continue;
        emit uidRead(ligne);
    }
}

void ArduinoRFID::write(const QByteArray &data)
{
    if(serial->isWritable()) {
        serial->write(data);
        serial->flush();
    }
}

// =============================================================================
//  CLASSE 3 : ArduinoClient (Clavier/Audio du Collègue)
// =============================================================================
ArduinoClient::ArduinoClient(QObject *parent) : QObject(parent), connected(false)
{
    serial = new QSerialPort(this);
}

ArduinoClient::~ArduinoClient() { closeArduino(); }

void ArduinoClient::connectArduino(const QString &portName)
{
    if (serial->isOpen()) serial->close();

    bool portTrouve = false;
    foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) {
        if (info.portName() == portName) {
            portTrouve = true;
            break;
        }
    }

    if (!portTrouve) {
        qDebug() << "Arduino Client introuvable sur le port" << portName;
        return;
    }

    serial->setPortName(portName);
    serial->setBaudRate(QSerialPort::Baud9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);

    if (serial->open(QIODevice::ReadWrite)) {
        connected = true;
        connect(serial, &QSerialPort::readyRead, this, &ArduinoClient::readFromArduino);
        qDebug() << "Arduino Client connecté sur" << portName;
    }
}

void ArduinoClient::closeArduino()
{
    if (serial->isOpen()) {
        serial->close();
        connected = false;
    }
}

void ArduinoClient::writeToArduino(const QByteArray &data)
{
    if (serial->isOpen() && serial->isWritable()) {
        serial->write(data);
        serial->flush();
    }
}

bool ArduinoClient::isConnected() const { return connected; }

void ArduinoClient::readFromArduino()
{
    while (serial->canReadLine()) {
        QByteArray line = serial->readLine();
        QString data = QString::fromStdString(line.toStdString()).trimmed();

        if (!data.isEmpty()) {
            QRegularExpression rx("\\d+");
            QRegularExpressionMatch match = rx.match(data);

            if (match.hasMatch()) {
                QString numeroExtract = match.captured(0);
                emit numeroRecu(numeroExtract);
            }
        }
    }
}
