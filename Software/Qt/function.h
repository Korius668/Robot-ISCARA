#ifndef FUNCTION_H
#define FUNCTION_H

#include "function.h"
#include <string>

class Function
{
private:
    const std::string name;
    const int parameters_number;
public:
    Function(std::string name, int parameters_number) : name(name), parameters_number(parameters_number){}

    std::string getName()               { return name; }
    int         getParametersNumber()   { return parameters_number; }
};

#endif // FUNCTION_H
