#ifndef SERIALCOMMUNICATION_H
#define SERIALCOMMUNICATION_H
#include <vector>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QDebug>
#include <QObject>
#include <QCoreApplication>
#include <QTimer>
#include <QApplication>
#include <QTextCodec>
#include <QTimer>

#define SYMBOL_READ_TIME    100
#define COMMAND_CHECK_TIME  2000

using namespace std;

class SerialCommunication : public QSerialPort
{
    Q_OBJECT
private:
    QSerialPort *serialPort;    // serial port object

    vector <QString> ports;     // available ports names
    QString currentPort;        // current port name
    int portCount;              // available ports count
\
    QString dataSent;           // last data sent
    QString dataReaded;         // last data red
    QString dataReadedBuffor;   // support variable

    QTimer *timerSymbolRead;    // set max time between passing symbols (if timer time has passed a full sentence is created)
    QTimer *commandTimer;       // set max time between sending command and recieving confirmation

    void checkConnection();     // serial port connection test

public:
    SerialCommunication();

    // functions for setting port combo box
    int portCounter(){return portCount;}
    QString portName(int id){return ports[id];}
    QString currentPortName(){return currentPort;}
    void setPort(QString portName){currentPort = portName;}
    bool getPorts();                    // returns information about disappeared ports


    void openSerialPort();              // open serial port connection
    void closeSerialPort();             // close serial port connection

public slots:
    void readDataSymbol();              // reads small part of data symbol from serial port
    void readDataReady();               // saves big chunk of red data into dataReaded variable and emits it to terminal
    void writeData(QString data);       // writes data to serial port

    void checkConnectionStatus();       // checkConnection() slot


signals:
    void readDataAvailable(QString);    // read data available signal (for terminal)
    void infoDataAvailable(QString);    // info data available signal (for terminal)

    void connectionTestStatus(bool);    // checkConnection() signal

};

#endif // SERIALCOMMUNICATION_H
