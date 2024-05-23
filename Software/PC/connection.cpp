#include "connection.hpp"
/*
class Arduino
{
private:
#ifdef _WIN32

#elif defined(__linux__)
#endif
};
*/

std::string fromArduino(std::string port)
{
    
    std::string readData;
#ifdef _WIN32
    HANDLE hSerial = CreateFile(port.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    DWORD bytesRead;
    char readBuffer[256];
    if (!ReadFile(hSerial, readBuffer, sizeof(readBuffer), &bytesRead, NULL)) {
        std::cerr << "Error reading from serial port!" << std::endl;
        CloseHandle(hSerial);
        return "Pusto";
    }
    readData = std::string(readBuffer, bytesRead);
    CloseHandle(hSerial);

#elif defined(__linux__)
  int fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1) {
        std::cerr << "Error opening serial port!" << std::endl;
        return "Pusto";
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B9600);
    cfsetospeed(&options, B9600);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    options.c_iflag &= ~(IXON | IXOFF | IXANY);
    options.c_oflag &= ~OPOST;

    tcsetattr(fd, TCSANOW, &options);

    fcntl(fd, F_SETFL, FNDELAY); // Non-blocking mode

    char readBuffer[256];
    ssize_t bytesRead = read(fd, readBuffer, sizeof(readBuffer));
    if (bytesRead < 0) {
        std::cerr << "Error reading from serial port!" << std::endl;
        close(fd);
        return "Pusto";
    }

    readData = std::string(readBuffer, bytesRead);
    close(fd);
#endif

    // Wyświetlamy otrzymaną odpowiedź
    std::cout << "Received: " << readData << std::endl;

    return readData;
}

int toArduino(std::string port)
{
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

    DWORD bytesWritten;
    char sendData[] = "H";
    if (!WriteFile(hSerial, sendData, sizeof(sendData), &bytesWritten, NULL) || bytesWritten != sizeof(sendData)) {
        std::cerr << "Error writing to serial port!" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    CloseHandle(hSerial);


#elif defined(__linux__)
    FILE* hSerial = fopen(port.c_str(), "r+");
    if (!hSerial) 
    {
        std::cerr << "Error opening serial port!" << std::endl;
        return 1;
    }

    ssize_t bytesWritten = fwrite("H", sizeof(char), 1, hSerial);
    
    if (bytesWritten != 1) {
        std::cerr << "Error writing to serial port!" << std::endl;
        fclose(hSerial);
        return 1;
    }
    fclose(hSerial);
    return 1;
#endif
}

#ifdef _WIN32
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

#elif defined(__linux__)
bool configureSerialPort(FILE* hSerial, int speed) 
{
    struct termios tty;
    if (tcgetattr(fileno(hSerial), &tty) != 0) {
        //std::cout<<fileno(hSerial)<<std::endl;
        //std::cerr << "Error getting serial port attributes!" << std::endl;
        return false;
    }
    cfsetospeed(&tty, speed);
    cfsetispeed(&tty, speed);

    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;

    tty.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    tty.c_iflag &= ~(IXON | IXOFF | IXANY);
    tty.c_oflag &= ~OPOST;

    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    if (tcsetattr(fileno(hSerial), TCSANOW, &tty) != 0) 
    {
        std::cerr << "Error setting serial port attributes!" << std::endl;
        return false;
    }
    return true;
}
#endif

bool testSerialPort(const std::string& portName, int speed, int maxAttempts, int delayBetweenAttemptsMillis) 
{
#ifdef _WIN32
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
#elif defined(__linux__)

    FILE* hSerial;
    for (int attempt = 1; attempt <= maxAttempts; ++attempt) 
    {
        hSerial = fopen(portName.c_str(), "r+");
        //std::cout<<portName<<" "<<hSerial<<std::endl;
        if (hSerial) 
        {
            bool result = configureSerialPort(hSerial, speed);
            fclose(hSerial);
            if (result) 
            {
                return true;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(delayBetweenAttemptsMillis));
    }
    return false;
#endif
}

#ifdef _WIN32
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
#elif defined(__linux__)
std::vector<std::string> findSerialPorts(const int speed)
{
    std::vector<std::string> ports;
    DIR* dir;
    struct dirent* entry;
    dir = opendir("/dev");

    if (dir != NULL) 
    {
        while ((entry = readdir(dir)) != NULL) 
        {
            struct stat st;
            std::string path = std::string("/dev/") + entry->d_name;
            stat(path.c_str(), &st);
            
            if (S_ISCHR(st.st_mode)) 
            {
                if (strncmp(entry->d_name, "ttyS", 4) == 0 || strncmp(entry->d_name, "ttyUSB", 6) == 0 || strncmp(entry->d_name, "ttyACM", 6) == 0) 
                {
                    //std::cout<<entry->d_name<<" "<<st.st_mode<<" "<<std::endl;
                    ports.push_back(std::string("/dev/") + entry->d_name);
                }
            }
        }
        closedir(dir);
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

std::string portSelection(std::vector<std::string> ports)
{
	int num =1;
	std::cout<<"Chose port"<<std::endl;
    
	for(int i=0;i<ports.size();i++)
	{
		std::cout<<i+1<<". "<<ports[i]<<std::endl;
	}
    
	std::cin >> num;
    
    while (std::cin.fail() || num < 1 || num > ports.size())
    {
        std::cout << "Wrong input" << std::endl;
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cin >> num;
    }

	return ports[num-1];
}
