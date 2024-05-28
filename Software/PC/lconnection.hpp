#ifdef __linux__

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <limits>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <dirent.h>
#include <sys/stat.h>

class Arduino
{
private:
std::string port;
int fd;
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

bool testSerialPort(const std::string& portName, int speed, int maxAttempts, int delayBetweenAttemptsMillis);
std::vector<std::string> findSerialPorts(int speed);
std::string fromArduino(std::string port);
int toArduino(std::string port);
#endif