#include "robotvariables.hpp"
#include <cmath>

#include <iostream>
using namespace std;

RobotVariables::RobotVariables() {

}


std::vector<double> RobotVariables::inverseKinematics(std::vector<double> cartesian){
    std::vector<double> joint;
}

std::vector<double> RobotVariables::forwardKinematics(std::vector<double> joint){
    std::vector<double> cartesian;
    cartesian.clear();
    double x, y, z, yaw, Ny, Nx;
    double  d1 = joint[0],
            th2 = deg2rad( joint[1] ),
            th3 = deg2rad( joint[2] ),
            th4 = deg2rad( joint[3] );

    x = a1 + cos(th2)*a2 + cos(th2)*cos(th3)*a3 - sin(th2)*sin(th3)*a3;
    y = sin(th2)*a2 + cos(th2)*sin(th3)*a3 + cos(th3)*sin(th2)*a3;
    z = d1 - d4;

    Nx = -cos(th4)*(sin(th2)*sin(th3) - cos(th2)*cos(th3)) - sin(th4)*(cos(th2)*sin(th3) + cos(th3)*sin(th2));
    Ny = cos(th4)*(cos(th2)*sin(th3) + cos(th3)*sin(th2)) - sin(th4)*(sin(th2)*sin(th3) - cos(th2)*cos(th3));
    yaw = rad2deg( atan2( Ny, Nx ) );

    cartesian.push_back(x);
    cartesian.push_back(y);
    cartesian.push_back(z);
    cartesian.push_back(yaw);

    return cartesian;
}

