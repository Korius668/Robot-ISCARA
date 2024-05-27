#ifdef _WIN32

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <limits>
#include <string.h>
#include <windows.h>

bool configureSerialPort(HANDLE hSerial, int speed);
bool testSerialPort(const std::string& portName, int speed, int maxAttempts, int delayBetweenAttemptsMillis);
std::vector<std::string> findSerialPorts(int speed);
std::string fromArduino(std::string port);
int toArduino(std::string port);
#endif