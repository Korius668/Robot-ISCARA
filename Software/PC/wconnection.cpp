#include "wconnection.hpp"

#ifdef _WIN32

Arduino::Arduino(std::string port, std::string* readData, std::string* writeData):
port(port),readData(readData),writeData(writeData)
{
    HANDLE hSerial = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

}

Arduino::~Arduino()
{
    CloseHandle(hSerial);
}

void Arduino::writeToPort() 
{
DWORD bytesWritten;
WriteFile(hSerial, writeData, writeData->size(), &bytesWritten, NULL);
}

void Arduino::writeToPort(std::string* data) 
{
DWORD bytesWritten;
WriteFile(hSerial, data, data->size(), &bytesWritten, NULL);
}


std::string Arduino::readFromPort() 
{
DWORD bytesRead;
ReadFile(hSerial, readBuffer, sizeof(readBuffer), &bytesRead, NULL);
*readData = std::string(readBuffer, bytesRead);
return *readData;
}


bool configureSerialPort(HANDLE hSerial, int speed) 
{
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams))
	{
		std::cerr << "Error getting serial port state!" << std::endl;
        return false;
	}

    dcbSerialParams.BaudRate = speed;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state!" << std::endl;
        return false;
    }
    return true;
}

bool testSerialPort(const std::string& portName, int speed, int maxAttempts, int delayBetweenAttemptsMillis) 
{

    HANDLE hSerial;

    for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
        hSerial = CreateFile(portName.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
        if (hSerial != INVALID_HANDLE_VALUE) {
            bool result = configureSerialPort(hSerial, speed);
            if (result) {
                CloseHandle(hSerial);
                return true;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delayBetweenAttemptsMillis));
    }
    return false;
}

std::vector<std::string> findSerialPorts(int speed)
{
	const std::string portPrefix = "\\\\.\\COM";
	std::vector<std::string> ports;

    for (int i = 1; i <= 256; ++i)
	{
        ports.push_back(portPrefix + std::to_string(i));
    }

	std::vector<std::string>  goodPorts;

    for (const std::string& porttmp : ports)
	{
        if (testSerialPort(porttmp, speed,1,1))
		{
            goodPorts.push_back(porttmp);
        }
    }
    return goodPorts;
}

#endif