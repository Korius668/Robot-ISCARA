#include "connection.hpp"

int main()
{
    
    const int speed = 9600; //Defining speed of comunication
 	std::vector<std::string> ports = findSerialPorts(speed);    //Searching for accessible ports on PC

    if(ports.size()==0) //If there is no ports available program will terminate
	{
        std::cout << "Serial ports not found" << std::endl;
        return 0;
    }
    std::string port;   
    std::string readData="Pusto";   
    std::string writeData="H";
    
    port= portSelection(ports); 

    std::cout<<"Wybrales "<<port<<std::endl;
    Arduino a(port,&readData,&writeData);
    a.writeToPort();
    a.readFromPort();
    std::cout<<readData;
    return 0;
}
