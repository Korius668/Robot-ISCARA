#include "serialcommunication.h"
using namespace std;


SerialCommunication::SerialCommunication() {
}

void SerialCommunication::getPorts(){

    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    for(const QSerialPortInfo &info : serialPortInfos )
    {
        ports.push_back(info.portName());

    }
    cout << ports.size() << endl;
}

