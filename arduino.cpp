#include "arduino.h"

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
    foreach (const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()) {
        if (serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()) {
            if(serialPortInfo.portName()!="COM6"){
            qDebug() << "Port detecté:" << serialPortInfo.portName();
            port_name = serialPortInfo.portName();
            is_available = true;
            break;
        }
        }}

    if (is_available) {
        serial->setPortName(port_name);
        if (serial->open(QSerialPort::ReadWrite)) {
            serial->setBaudRate(QSerialPort::Baud9600);
            serial->setDataBits(QSerialPort::Data8);
            serial->setParity(QSerialPort::NoParity);
            serial->setStopBits(QSerialPort::OneStop);
            serial->setFlowControl(QSerialPort::NoFlowControl);
            connect(serial, &QSerialPort::readyRead, this, &Arduino::readData);
            qDebug() << "Arduino connecté avec succès au port" << port_name;
        } else {
            qDebug() << "Impossible d'ouvrir le port série :" << serial->errorString();
        }
    } else {
        qDebug() << "Arduino non trouvé !";
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
}
