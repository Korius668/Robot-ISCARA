#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H

#include <iostream>
#include <vector>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QObject>
#include <QCoreApplication>

using namespace std;

class SerialCommunication
{
private:
    vector <QString> ports;

public:
    SerialCommunication();

    void getPorts();
};

#endif // SERIALCOMMUNICATION_H
