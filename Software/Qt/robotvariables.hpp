#ifndef ROBOTVARIABLES_HPP
#define ROBOTVARIABLES_HPP

#include <vector>

class RobotVariables
{
private:
    const double    a1 = 20,
                    a2 = 200,
                    a3 = 100,
                    d4 = 150;

    const double    max_d1  =   600,
                    max_th2 =   90,     // +-
                    max_th3 =   180,    // +-
                    max_th4 =   180,    // +-
                    min_d1  =   200,
                    min_th2 =   -90,     // +-
                    min_th3 =   -180,    // +-
                    min_th4 =   -180,    // +-
                    max_percentage = 100,
                    min_percentage = 1;

    const double gripper[2] =
    {
        {0},
        {120}
    };

    double  current_d1,
            current_th2,
            current_th3,
            current_th4,
            current_x,
            current_y,
            current_z,
            current_yaw;

    unsigned int gripperId; // type of gripper
    bool kinematicsConfig;  // kinematics congifuration preferred


public:
            RobotVariables();

    void    setCurrent_d1(double d1)    {this->current_d1 = d1;}
    void    setCurrent_th2(double th2)  {this->current_th2 = th2;}
    void    setCurrent_th3(double th3)  {this->current_th3 = th3;}
    void    setCurrent_th4(double th4)  {this->current_th4 = th4;}
    void    setCurrent_x(double x)      {this->current_x = x;}
    void    setCurrent_y(double y)      {this->current_y = y;}
    void    setCurrent_z(double z)      {this->current_z = z;}
    void    setCurrent_yaw(double yaw)  {this->current_yaw = yaw;}
    void  setKinematicsConfig(bool con) {this->kinematicsConfig = con;}


    double  getCurrent_d1()             {return current_d1;}
    double  getCurrent_th2()            {return current_th2;}
    double  getCurrent_th3()            {return current_th3;}
    double  getCurrent_th4()            {return current_th4;}
    double  getCurrent_x()              {return current_x;}
    double  getCurrent_y()              {return current_y;}
    double  getCurrent_z()              {return current_z;}
    double  getCurrent_yaw()            {return current_yaw;}

    double  getMax_d1()                 {return max_d1 - gripper[ getGripperId() ];}
    double  getMax_th2()                {return max_th2;}
    double  getMax_th3()                {return max_th3;}
    double  getMax_th4()                {return max_th4;}
    double  getMin_d1()                 {return min_d1;}
    double  getMin_th2()                {return min_th2;}
    double  getMin_th3()                {return min_th3;}
    double  getMin_th4()                {return min_th4;}
    double  getMaxPercentage()          {return max_percentage;}
    double  getMinPercentage()          {return min_percentage;}
    bool    getKinematicsConfig()       {return kinematicsConfig;}


private:
    void                    setGripperId(unsigned int Id)   { this->gripperId = Id; }
    unsigned int            getGripperId()                  { return gripperId; }
    double                  deg2rad(double deg)             { return (deg/180) * 3.141592653589; }
    double                  rad2deg(double rad)             { return (rad / 3.141592653589) * 180; }

public:
    int                     inverseKinematics(std::vector<double>* cartesian, std::vector<double>* joint);
    int                     forwardKinematics(std::vector<double>* joint, std::vector<double>* cartesian);

};

#endif // ROBOTVARIABLES_HPP
