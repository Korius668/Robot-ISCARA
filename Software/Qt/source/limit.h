#ifndef LIMIT_H
#define LIMIT_H

#include <string>

class Limit
{
private:
    const std::string name;
    const double max_value;
    const double min_value;
public:
    Limit (std::string nam, double max_val, double min_val) : name(nam), max_value(max_val), min_value(min_val){}

    std::string getName(){ return name; }
    const double getMaxValue(){ return max_value; }
    const double getMinVaule(){ return min_value; }
};

#endif // LIMIT_H
