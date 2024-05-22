#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <limits>

#ifdef _WIN32
#include <windows.h>
#elif defined(__linux__)
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <dirent.h>
#include <sys/stat.h>
#endif

bool configureSerialPort(HANDLE hSerial, int speed);
bool testSerialPort(const std::string& portName, int speed, int maxAttempts, int delayBetweenAttemptsMillis);
std::vector<std::string> findSerialPorts(int speed);
std::string portSelection(std::vector<std::string> ports);