#include "arduino.h"
#include <QDebug>

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
            qDebug() << "Port detecté:" << serialPortInfo.portName();
            port_name = serialPortInfo.portName();
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
