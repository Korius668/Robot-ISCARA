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
    std::string port;
    
    port= portSelection(ports);

    std::cout<<"Wybrales "<<port<<std::endl;

    toArduino(port);
    std::cout<<fromArduino(port);

    return 0;
}
