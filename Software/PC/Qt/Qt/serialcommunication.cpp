#include "serialcommunication.h"
using namespace std;


SerialCommunication::SerialCommunication() {
    this->portCount = 0;
    this->currentPort = NULL;
    this->connectionStatus = false;
    getPorts();

    this->serialPort = new QSerialPort;
}

bool SerialCommunication::getPorts(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    bool connectedPortDisappeared = false;
    int portsSize = 0;

    // adding new ports
    for(const QSerialPortInfo &info : serialPortInfos ){
        if (info.vendorIdentifier() > 0){
            portsSize++;
            bool flag = false;
            for (int i = 0; i < portCount; i++){
                if (ports[i] == info.portName()){
                    flag = true;
                }
            }
            if (flag == false)
                ports.push_back(info.portName());
        }
    }

    // deleting old ports
    if (portsSize < portCount){
        for (int i = 0; i < portCount; i++){
            bool flag = false;
            for(const QSerialPortInfo &info : serialPortInfos ){
                if (info.vendorIdentifier() > 0){
                    if (ports[i] == info.portName())
                        flag = true;
                }
            }
            if (flag == false){
                if (ports[i] == currentPort)
                    connectedPortDisappeared = true;

                ports.erase(ports.begin() + i);
            }
        }
    }
    this->portCount = portsSize;
    return connectedPortDisappeared;
}


void SerialCommunication::setPort(QString portName){
    this->currentPort = portName;
}

bool SerialCommunication::openSerialPort(){
    bool returnStatus = false;
    if (connectionStatus == false){
        //settings
        serialPort->setPortName( currentPort );
        serialPort->setBaudRate(QSerialPort::Baud9600);
        serialPort->setParity(QSerialPort::Parity::NoParity);
        serialPort->setDataBits(QSerialPort::DataBits::Data8);
        serialPort->setStopBits(QSerialPort::StopBits::TwoStop);
        if (serialPort->open(QIODevice::ReadWrite)) {
            connectionStatus = true;
            returnStatus = true;
        }
    }
    return returnStatus;
}

bool SerialCommunication::closeSerialPort(){
    bool returnStatus = false;
    if (connectionStatus == true){
        serialPort->close();
        connectionStatus = false;
        currentPort = NULL;
        returnStatus = true;
    }
    return returnStatus;
}

void SerialCommunication::writeData(const QByteArray &data){
    if (connectionStatus == true){
        const qint64 written = serialPort->write(data);
        if (written == data.size()) {
        } else {
            printf("Error\n");
        }
    }
}
