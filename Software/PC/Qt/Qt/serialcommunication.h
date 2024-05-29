#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <iostream>
#include <vector>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QObject>
#include <QCoreApplication>
#include <QTimer>
#include <QApplication>
using namespace std;

class SerialCommunication
{
private:
    vector <QString> ports; // available ports names
    int portCount;          // available ports count
    QString currentPort;    // current port name
    QSerialPort *serialPort;
    bool connectionStatus;

public:
    SerialCommunication();

    int portCounter(){return portCount;}
    QString portName(int id){return ports[id];}

    bool getPorts();
    void setPort(QString portName);

    bool openSerialPort();
    bool closeSerialPort();

    void writeData(const QByteArray &data);
};

#endif // SERIALCOMMUNICATION_H
