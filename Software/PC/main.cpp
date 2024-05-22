#include "connection.hpp"

int main()
{

    const int speed = 9600;
 	std::vector<std::string> ports = findSerialPorts(speed);

    if(ports.size()==0)
	{
        std::cout << "Serial ports not found" << std::endl;
        return 0;
    }

    std::string port = portSelection(ports);

#ifdef _WIN32

    HANDLE hSerial = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE)
	{
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            std::cerr << "Port not available!" << std::endl;
        } else {
            std::cerr << "Error while opening serial port!" << std::endl;
        }
        return 1;
    }
#elif defined(__linux__)


    FILE* hSerial = fopen(port, "r+");
    if (!hSerial) {
        std::cerr << "Error opening serial port!" << std::endl;
        return 1;
    }
#endif

    // Konfiguracja parametrów portu szeregowego
#ifdef _WIN32
    if (!configureSerialPort(hSerial, 9600)) {
        CloseHandle(hSerial);
        return 1;
    }
#elif defined(__linux__)
    if (!configureSerialPort(hSerial, B9600)) {
        fclose(hSerial);
        return 1;
    }
#endif

    // Wysyłamy zapytanie do Arduino
#ifdef _WIN32
    DWORD bytesWritten;
    char sendData[] = "H";
    if (!WriteFile(hSerial, sendData, sizeof(sendData), &bytesWritten, NULL)) {
        std::cerr << "Error writing to serial port!" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }
#elif defined(__linux__)
    ssize_t bytesWritten = fwrite("H", sizeof(char), 1, hSerial);
    if (bytesWritten != 1) {
        std::cerr << "Error writing to serial port!" << std::endl;
        fclose(hSerial);
        return 1;
    }
#endif

    // Odczytujemy odpowiedź z Arduino
    std::string readData;
#ifdef _WIN32
    DWORD bytesRead;
    char readBuffer[256];
    if (!ReadFile(hSerial, readBuffer, sizeof(readBuffer), &bytesRead, NULL)) {
        std::cerr << "Error reading from serial port!" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }
    readData = std::string(readBuffer, bytesRead);
#elif defined(__linux__)
    char readBuffer[256];
    ssize_t bytesRead = read(fileno(hSerial), readBuffer, sizeof(readBuffer));
    if (bytesRead < 0) {
        std::cerr << "Error reading from serial port!" << std::endl;
        fclose(hSerial);
        return 1;
    }
    readData = std::string(readBuffer, bytesRead);
#endif

    // Wyświetlamy otrzymaną odpowiedź
    std::cout << "Received: " << readData << std::endl;

#ifdef _WIN32
    CloseHandle(hSerial);
#elif defined(__linux__)
    fclose(hSerial);
#endif

    return 0;
}
