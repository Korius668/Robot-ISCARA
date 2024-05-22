#include "connection.hpp"


bool configureSerialPort(HANDLE hSerial, int speed) {
#ifdef _WIN32
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
#elif defined(__linux__)
    struct termios tty;
    if (tcgetattr(fileno((FILE*)hSerial), &tty) != 0) {
        std::cerr << "Error getting serial port attributes!" << std::endl;
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

    if (tcsetattr(fileno((FILE*)hSerial), TCSANOW, &tty) != 0) {
        std::cerr << "Error setting serial port attributes!" << std::endl;
        return false;
    }
    return true;
#endif
}

bool testSerialPort(const std::string& portName, int speed, int maxAttempts, int delayBetweenAttemptsMillis) {
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
    for (int attempt = 1; attempt <= maxAttempts; ++attempt) {
        hSerial = fopen(portName.c_str(), "r+");
        if (hSerial) {
            bool result = configureSerialPort(hSerial, speed);
            fclose(hSerial);
            if (result) {
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
std::vector<std::string> findSerialPorts(int speed)
{
    std::vector<std::string> ports;
    DIR* dir;
    struct dirent* entry;
    dir = opendir("/dev");

    if (dir != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            struct stat st;
            stat(entry->d_name, &st);
            if (S_ISCHR(st.st_mode)) {
                if (strncmp(entry->d_name, "ttyS", 4) == 0 || strncmp(entry->d_name, "ttyUSB", 6) == 0) {
                    ports.push_back(std::string("/dev/") + entry->d_name);
                }
            }
        }
        closedir(dir);
    }

    std::vector<std::string>  goodPorts;
	int* i=new int(1);

    for (const std::string& porttmp : ports)
	{
        if (testSerialPort(porttmp, speed,1,1))
		{
            goodPorts.push_back(porttmp);
            *i=*i++;
        }
    }
    delete i;

    return goodPorts;
}
#endif

std::string portSelection(std::vector<std::string> ports)
{
	int num;
	std::cout<<"Chose port"<<std::endl;
	for(int i=0;i<ports.size();i++)
	{
		std::cout<<i+1<<". "<<ports[i]<<std::endl;
	}

	do
	{
		std::cin>>num;
		if (std::cin.fail() || num < 1 || num > ports.size())
		{
	        std::cout << "Wrong input" << std::endl;
	        std::cin.clear();
	        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	    }
	}
	while(std::cin.fail() || num < 1 || num > ports.size());
	std::string port=ports[num-1];
	return port;
}
