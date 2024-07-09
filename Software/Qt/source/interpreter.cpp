#include "interpreter.h"
#include <QDebug>
#include <iostream>
#include <string>

using namespace std;


Interpreter::Interpreter(RobotVariables *RobotVariables) {

    this->robotVariables = RobotVariables;

    // FUNCTION LIST
    functionList.push_back( { "START",      0 } );
    functionList.push_back( { "STOP",       0 } );
    functionList.push_back( { "SPEED.H",    1 } );
    functionList.push_back( { "ACCEL.H",    1 } );
    functionList.push_back( { "GRAB.B",     1 } );
    functionList.push_back( { "MOVE.Q",     4 } );
    functionList.push_back( { "MOVEJ.Q",    4 } );
    functionList.push_back( { "MOVEL.Q",    4 } );
    functionList.push_back( { "MOVE.P",     4 } );
    functionList.push_back( { "MOVEJ.P",    4 } );
    functionList.push_back( { "MOVEL.P",    4 } );
}

int Interpreter::interpretation(string command, string* response){

    vector<string> parsedString;
    vector<double> parameters, jointParams, cartesianParams;

    int functionNumber = -1;
    int error = 0;
    int functionType = 0;

    parsedString.clear();
    parameters.clear();
    jointParams.clear();
    cartesianParams.clear();

    // parse command
    error = parseString(command, &parsedString);

    // check syntax
    if( error == 0 )
        error += checkSyntax(&parsedString, &functionNumber, &functionType);

    // convert parameters from stirng to double
    if (error == 0)
        parameters = str2db(&parsedString);

    // kinematics and limit check
    if (error == 0){

        // forward kinematics
        if(functionType == 1) {
            jointParams = parameters;
            error = robotVariables->forwardKinematics(&jointParams, &cartesianParams);
        }

        // inverse kinematics
        else if (functionType == 2) {
            cartesianParams = parameters;
            error = robotVariables->inverseKinematics(&cartesianParams, &jointParams);
            parameters = jointParams;
        }

        // check percentage arguments
        else if  (functionType == 3)
            if ( parameters[0] > robotVariables->getMaxPercentage() || parameters[0] < robotVariables->getMinPercentage() ) error = 3;

        // check bool arguments
        else if (functionType == 4)
                if ( parameters[0] != 1 && parameters[0] != 0 ) error = 3;

    }

    // prapare output function
    if (error == 0){
        *response = transfer(functionNumber, parameters);
    }

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

int Interpreter::checkSyntax(const std::vector<std::string>* parsedString, int* functionNumber, int* functionType){

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

    if (error == 0 && (name.find(".") != -1) ){

        if ( (name.find(".Q") != -1) ) *functionType = 1;

        if ( (name.find(".P") != -1) ) *functionType = 2;

        if ( (name.find(".H") != -1) ) *functionType = 3;

        if ( (name.find(".B") != -1) ) *functionType = 4;

    }
    return error;
}

std::vector<double> Interpreter::str2db(const std::vector<std::string>* parsedString){
    std::vector<double> params;
    params.clear();
    for (unsigned int i = 1; i < parsedString->size(); i++) params.push_back( std::stod( parsedString->at(i) ) );
    return params;
}

string Interpreter::transfer(int functionNumber, vector<double> parameters){
    string output;
    string ph;
    size_t cut;

    output.clear();
    output.append( "#" );
    ph = functionList[functionNumber].getName();
    cut = ph.find( '.' );
    if ( cut != -1 )
        ph = ph.substr(0, cut);
    output.append( ph );
    output.append( "#" );

    for (unsigned int i = 0; i < functionList[functionNumber].getParametersNumber(); i++){
        ph = to_string(parameters[i]);
        ph.erase ( ph.find_last_not_of('0') + 1, std::string::npos );
        ph.erase ( ph.find_last_not_of('.') + 1, std::string::npos );
        cut = ph.find( '.' );
        if (cut != -1){
            cut = ph.size() - cut;
            while (cut > 3) {ph.pop_back(); cut--;}
        }
        output.append( ph );
        output.append( "#" );
    }    return output;
}
