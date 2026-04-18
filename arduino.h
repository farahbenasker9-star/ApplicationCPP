#ifndef ARDUINO_H
#define ARDUINO_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QString>

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
