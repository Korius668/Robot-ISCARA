#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "robotvariables.hpp"
#include "function.h"
#include <QObject>
#include <QFile>
#include <vector>

class Interpreter

{
private:

    std::vector<Function> functionList;

    int                 parseString     (std::string command,                               std::vector<std::string>* parsedString);
    int                 checkSyntax     (const std::vector<std::string>* parsedString,      int *functionNumber,                        bool* inversedKinematics);
    std::vector<double> str2db          (const std::vector<std::string>* parsedString);


    int checkLimits(const std::vector<std::string>* parsedString, int* functionNumber);
    std::string transfer(std::string command);

    RobotVariables* robotVariables;

public:
    Interpreter(RobotVariables *RobotVariables);
    int interpretation(std::string command);


};

#endif // INTERPRETER_H
