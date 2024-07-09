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

    int                 parseString     (std::string command, std::vector<std::string>* parsedString);
    int                 checkSyntax     (const std::vector<std::string>* parsedString, int *functionNumber, int *functionType);
    std::vector<double> str2db          (const std::vector<std::string>* parsedString);
    std::string         transfer        (int functionNumber, std::vector<double> parameters);

    RobotVariables* robotVariables;

public:

    Interpreter(RobotVariables *RobotVariables);

    int                     interpretation  (std::string command, std::string *response);
    std::vector<Function>   getFunctionList () { return functionList;}

};

#endif // INTERPRETER_H
