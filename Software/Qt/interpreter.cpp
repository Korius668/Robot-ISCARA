#include "interpreter.h"
#include <QDebug>
#include <iostream>
#include <string>

using namespace std;


Interpreter::Interpreter(RobotVariables *RobotVariables) {

    this->robotVariables = RobotVariables;

    //FUNCTION LIST
    functionList.push_back( { "START",      0 } );
    functionList.push_back( { "STOP",       0 } );
    functionList.push_back( { "SPEED",      1 } );
    functionList.push_back( { "ACCEL",      1 } );
    functionList.push_back( { "GRAB",       1 } );
    functionList.push_back( { "MOVE.Q",     4 } );
    functionList.push_back( { "MOVEJ.Q",    4 } );
    functionList.push_back( { "MOVEL.Q",    4 } );
    functionList.push_back( { "MOVE.P",     4 } );
    functionList.push_back( { "MOVEJ.P",    4 } );
    functionList.push_back( { "MOVEL.P",    4 } );

}


int Interpreter::interpretation(string command){

    vector<string> parsedString;
    vector<double> parameters;
    int functionNumber = -1;
    int error = 0;
    bool inversedKinematics = false;

    parsedString.clear();

    // PARSE COMMAND
    error = parseString(command, &parsedString);

    // CHECK SYNTAX
    if( error == 0 )
        error += checkSyntax(&parsedString, &functionNumber, &inversedKinematics);

    // CONVERT PARAMETERS FROM STRING TO DOUBLE
    if (error == 0)
        parameters = str2db(&parsedString);

    // CONVERT PARAMETERS USING INVERSED KINEMATICS
    if( error == 0 && inversedKinematics == 1);

    // CHECK LIMITATIONS


    // TRANSFER TO MICROCONTROLLER COMMAND

    return error;
}

int Interpreter::parseString(string command, std::vector<string>* parsedString){

    int error = 0;
    string  nameFun,
            parameters,
            parameter;

    size_t  begin = command.find( '(' ),
        end = command.find( ')' );

    if ( begin != -1 && end != -1 && command.size() == (end + 1) ){

        nameFun     =   command.substr(0 , begin);
        parameters  =   command.substr( begin + 1, end - begin - 1);

        parsedString->push_back(nameFun);
        int k = 0;
        char c;
        while ( k != parameters.length() ){
            c = parameters[k];
            if ( c == ','){
                for (unsigned int j = 0; j < parameter.size(); j++)
                    if ( isblank(parameter[j]) == 0){
                        parsedString->push_back( parameter );
                        break;
                    }
                parameter.clear();
            }
            else if ( isblank(c) == 0 )

            if ( isdigit(c) || c == '.' )
                parameter.push_back( c );
            else{
                parameter.clear();
                error = 1;
                break;
            }
            k++;
        }
        for (unsigned int j = 0; j < parameter.size(); j++)
            if ( isblank(parameter[j]) == 0){
                parsedString->push_back( parameter );
                break;
            }
    }
    else{
        error = 1;
    }

    return error;
}

int Interpreter::checkSyntax(const std::vector<std::string>* parsedString, int* functionNumber, bool* inversedKinematics){

    string name = *parsedString->begin();
    int error = 0;
    bool nameError = true;
    bool parametersError = true;
    unsigned int k;
    for (k = 0; k < functionList.size(); k++)
        if (name == functionList[k].getName()){
            nameError = false;
            break;
        }

    if ( (nameError == false) && (parsedString->size() - 1) == functionList[k].getParametersNumber() )
        parametersError = false;

    if (nameError | parametersError != 0)
        error = 2;
    else
        *functionNumber = k;

    if ( error == 0 && (name.find(".P") != -1) ){
        *inversedKinematics = true;
    }

    return error;
}

std::vector<double> Interpreter::str2db(const std::vector<std::string>* parsedString){
    std::vector<double> params;
    params.clear();
    for (unsigned int i = 1; i < parsedString->size(); i++) params.push_back( std::stod( parsedString->at(i) ) );
    return params;
}




int Interpreter::checkLimits(const std::vector<std::string>* parsedString, int* functionNumber){
    int error = 0;

    return error;
}


string Interpreter::transfer(string command){
    string commandValidated;


    return commandValidated;
}
