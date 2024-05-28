#include "serialcommunication.h"
using namespace std;


SerialCommunication::SerialCommunication() {
    getPorts();
}

void SerialCommunication::getPorts(){
    const auto serialPortInfos = QSerialPortInfo::availablePorts();
    int portsSize = 0;

    // adding new ports
    for(const QSerialPortInfo &info : serialPortInfos ){
        if (info.vendorIdentifier() > 0){
            portsSize++;
            bool flag = false;
            for (int i = 0; i < portCount; i++){
                if (ports[i] == info.portName())
                    flag = true;
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
            if (flag == false)
                ports.erase(ports.begin() + i);
        }
    }
    this->portCount = portsSize;
}


