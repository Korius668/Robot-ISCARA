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
    vector <QString> ports;
    QString currentPort = NULL;
    int portCount;

    int baudRate = 9600;

public:
    SerialCommunication();

    int portCounter(){return portCount;}
    QString portName(int id){return ports[id];}

    void getPorts();
    void setPort();
};

#endif // SERIALCOMMUNICATION_H
