#include "connection.hpp"


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
