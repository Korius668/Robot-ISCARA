#include "robotvariables.hpp"
#include <cmath>

#include <iostream>
using namespace std;

RobotVariables::RobotVariables() {
    setGripperId(0);
    setKinematicsConfig(true);
}


int RobotVariables::inverseKinematics(std::vector<double>* cartesian, std::vector<double>* joint){
    int error = 0;
    double d1, th2, th3, th4;
    double l, th, cosA, cosB, A, B;
    double  x = *(cartesian->begin()),
            y = *(cartesian->begin()+1),
            z = *(cartesian->begin()+2),
            yaw = *(cartesian->begin()+3);

    // support parameters
    l = sqrt( pow( x - a1, 2) + pow( y, 2) );
    th = rad2deg( atan2(y, x - a1) );
    cosA = ( pow(a2, 2) + pow(l, 2) - pow(a3, 2) ) / ( 2 * a2 * l );
    cosB = ( pow(a2, 2) + pow(a3, 2) - pow(l, 2) ) / ( 2 * a2 * a3 );

    if ( ( cosA > 1 || cosA < -1 ) || ( cosB > 1 || cosB < -1 ) ) error = 3;

    if ( error == 0 ){

        A = rad2deg(acos(cosA));
        B = rad2deg(acos(cosB));

        d1 = z + d4;

        if (getKinematicsConfig() == false){
            th2 = A + th;
            th3 = -180 + B;
        }else{
            th2 = th - A;
            th3 = 180 - B;
        }

        if ( ( th2 > getMax_th2() || th2 < getMin_th2()) && getKinematicsConfig() == false ){
            th2 = th - A;
            th3 = 180 - B;
        }
        if ( ( th2 > getMax_th2() || th2 < getMin_th2()) && getKinematicsConfig() == true ){
            th2 = A + th;
            th3 = -180 + B;
        }

        th4 = yaw - th2 - th3;
        while (th4 < -180) th4 += 360;
        while (th4 > 180) th4 -= 360;

        // CHECK LIMITS
        if      ( d1  > getMax_d1()  || d1  < getMin_d1()  )    error = 4;
        else if ( th2 > getMax_th2() || th2 < getMin_th2() )    error = 4;
        else if ( th3 > getMax_th3() || th3 < getMin_th3() )    error = 4;
        else if ( th4 > getMax_th4() || th4 < getMin_th4() )    error = 4;

        if ( error == 0 ){
            joint->clear();
            joint->push_back(d1);
            joint->push_back(th2);
            joint->push_back(th3);
            joint->push_back(th4);
        }

    }

    return error;
}

int RobotVariables::forwardKinematics(std::vector<double>* joint, std::vector<double>* cartesian){
    int error = 0;
    double x, y, z, yaw, Ny, Nx;
    double      d1 = *(joint->begin()),
                th2 = *(joint->begin()+1),
                th3 = *(joint->begin()+2),
                th4 = *(joint->begin()+3);

    // CHECK LIMITS
    if      ( d1  > getMax_d1()  || d1  < getMin_d1()  )    error = 4;
    else if ( th2 > getMax_th2() || th2 < getMin_th2() )    error = 4;
    else if ( th3 > getMax_th3() || th3 < getMin_th3() )    error = 4;
    else if ( th4 > getMax_th4() || th4 < getMin_th4() )    error = 4;

    if ( error == 0){

        th2 = deg2rad(th2);
        th3 = deg2rad(th3);
        th4 = deg2rad(th4);

        x = a1 + cos(th2)*a2 + cos(th2)*cos(th3)*a3 - sin(th2)*sin(th3)*a3;
        y = sin(th2)*a2 + cos(th2)*sin(th3)*a3 + cos(th3)*sin(th2)*a3;
        z = d1 - d4;

        Nx = -cos(th4)*(sin(th2)*sin(th3) - cos(th2)*cos(th3)) - sin(th4)*(cos(th2)*sin(th3) + cos(th3)*sin(th2));
        Ny = cos(th4)*(cos(th2)*sin(th3) + cos(th3)*sin(th2)) - sin(th4)*(sin(th2)*sin(th3) - cos(th2)*cos(th3));
        yaw = rad2deg( atan2( Ny, Nx ) );

        cartesian->clear();
        cartesian->push_back(x);
        cartesian->push_back(y);
        cartesian->push_back(z);
        cartesian->push_back(yaw);

    }
    return error;
}

