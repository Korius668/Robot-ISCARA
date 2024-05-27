#ifdef _WIN32

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <limits>
#include <string.h>
#include <windows.h>

class Arduino
{
private:
std::string port;
HANDLE hSerial;
std::string* writeData;
std::string* readData;
char readBuffer[256];

public:
    Arduino(std::string port, std::string* readData, std::string* writeData);
    ~Arduino();
    void writeToPort();
    void writeToPort(std::string* data);
    std::string readFromPort();
};

bool configureSerialPort(HANDLE hSerial, int speed);
bool testSerialPort(const std::string& portName, int speed, int maxAttempts, int delayBetweenAttemptsMillis);
std::vector<std::string> findSerialPorts(int speed);

#endif