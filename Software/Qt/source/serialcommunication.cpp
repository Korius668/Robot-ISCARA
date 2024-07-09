#include "serialcommunication.h"

SerialCommunication::SerialCommunication() {
    this->portCount = 0;
    this->currentPort = NULL;
    this->dataReaded = NULL;
    this->dataReadedBuffor = NULL;
    this->dataSent = NULL;
    getPorts();

    this->serialPort = new QSerialPort;
    connect(serialPort , &QSerialPort::readyRead, this, &SerialCommunication::readDataSymbol);

    this->timerSymbolRead = new QTimer;
    connect(timerSymbolRead, &QTimer::timeout, this, &SerialCommunication::readDataReady);

    this->commandTimer = new QTimer;
    connect(commandTimer, &QTimer::timeout, this, &SerialCommunication::checkConnectionStatus);
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


void SerialCommunication::openSerialPort(){
    //settings
    serialPort->setPortName( currentPort );
    serialPort->setBaudRate(QSerialPort::Baud9600);
    serialPort->setParity(QSerialPort::Parity::NoParity);
    serialPort->setDataBits(QSerialPort::DataBits::Data8);
    serialPort->setStopBits(QSerialPort::StopBits::TwoStop);
    serialPort->open(QIODevice::ReadWrite);
    checkConnection();
}

void SerialCommunication::closeSerialPort(){
    serialPort->close();
    checkConnection();
}

void SerialCommunication::checkConnection(){
    dataReaded = "Random";
    writeData("Communication Test");
    commandTimer->start(COMMAND_CHECK_TIME);
}

void SerialCommunication::checkConnectionStatus(){
    bool status = false;
    commandTimer->stop();
    if (dataReaded == dataSent)
        status = true;
    emit connectionTestStatus(status);
}

void SerialCommunication::writeData(QString data){
    dataSent = NULL;
    const QByteArray convertedData = data.toUtf8();
     //   serialPort->clear();
        const qint64 written = serialPort->write(convertedData);
        if (written == data.size()) {
            dataSent = data;
        } else {
            emit infoDataAvailable("Error. Part of data is lost.");
        }
}

void SerialCommunication::readDataSymbol()
{
    dataReaded = NULL;
    timerSymbolRead->start(SYMBOL_READ_TIME);
    const QByteArray data = serialPort->readAll();
    QString convertedData = QString::fromStdString(data.toStdString());
    dataReadedBuffor.append(convertedData);
}
void SerialCommunication::readDataReady(){
    timerSymbolRead->stop();
    dataReadedBuffor = dataReadedBuffor.trimmed();
    dataReaded = dataReadedBuffor;
    emit readDataAvailable(dataReadedBuffor);
    dataReadedBuffor = NULL;
}
