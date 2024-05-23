#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <limits>
#include <string.h>

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

#ifdef _WIN32
bool configureSerialPort(HANDLE hSerial, int speed);
#elif defined(__linux__)
bool configureSerialPort(FILE* hSerial, int speed);
#endif

bool testSerialPort(const std::string& portName, int speed, int maxAttempts, int delayBetweenAttemptsMillis);
std::vector<std::string> findSerialPorts(int speed);
std::string portSelection(std::vector<std::string> ports);
void test();
std::string fromArduino(std::string port);
int toArduino(std::string port);
