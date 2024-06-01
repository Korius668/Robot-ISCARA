#include <Servo.h>
#include <stdio.h>
#include <string.h>
#include "leOS2.h"
#include <math.h>
#include <Hashtable.h>

struct Joint
{
    double z;       // Postępowy ruch góra-dół
    double theta1;  // Kąt obrotu pierwszego członu
    double theta2;  // Kąt obrotu drugiego członu
    double theta_g; // Kąt obrotu chwytaka
} ;

struct Cartesian
{
    double x;  // Współrzędna X końcówki robota
    double y;  // Współrzędna Y końcówki robota
    double z;  // Współrzędna Z końcówki robota
    double zr; // Kąt orientacji chwytaka
};

Hashtable <String, Joint> jvar;     //  Joint variables
Hashtable <String, Cartesian> cvar; //  Cartesian variables

const int servoPin = 9;
leOS2 threads;  
Servo servoMotor;
String receivedString = "";
Cartesian cpos; //  Cartesian current position
Joint jpos;     //  Joint current position

enum Funct{ MOVE,MOVEJ,POINT,POINTJ,UNKNOWN,CONNECT };

enum Funct getFunct(const char *str) 
{
    if(strcmp(str, "MOVE") == 0 || strcmp(str, "move") == 0) return MOVE;
    if(strcmp(str, "MOVEJ") == 0 || strcmp(str, "movej") == 0) return MOVEJ;
    if(strcmp(str, "POINT") == 0 || strcmp(str, "point") == 0) return POINT;
    if(strcmp(str, "POINTJ") == 0 || strcmp(str, "pointj") == 0) return POINTJ;
    if(strcmp(str, "CONNECT")==0 || strcmp(str, "connect") == 0) return CONNECT;
    return UNKNOWN;
}

Joint inverseKinematics(Cartesian target) 
{
    Joint config;

    // Zakładamy, że długości ramion są znane:
    double L1 = 214; // długość pierwszego członu obrotowego
    double L2 = 97.5; // długość drugiego członu obrotowego

    // Ustawiamy postępowy ruch z
    config.z = target.z;

    // Obliczanie kąta obrotu chwytaka
    config.theta_g = target.zr;

    // Obliczanie kątów obrotowych członów
    double r = sqrt(target.x * target.x + target.y * target.y); // odległość od osi Z
    double phi = atan2(target.y, target.x);

    // Używamy twierdzenia cosinusów do obliczenia kątów
    double cos_alpha = (r * r + L1 * L1 - L2 * L2) / (2 * r * L1);
    if(cos_alpha>1||cos_alpha<-1)
    {
      Serial.print("Error: ruch poza zasięgiem robota");
      return jpos;
    }
    double alpha = acos(cos_alpha);
    double cos_beta = (L1 * L1 + L2 * L2 - r * r) / (2 * L1 * L2);
    if(cos_beta>1||cos_beta<-1)
    {
      Serial.print("Error: ruch poza zasięgiem robota \n");
      return jpos;
    }
    double beta = acos(cos_beta);
    // Kąt obrotu pierwszego członu
    config.theta1 = target.zr - alpha;
    // Kąt obrotu drugiego członu
    config.theta2 = PI - beta;
    return config;
}


void communication()
{
  
  if (Serial.available())
  {
    String receivedString = Serial.readStringUntil('\n'); // Dodanie odczytanego znaku do otrzymanej linii tekstu
      //Serial.print(receivedString);
      interpretacja(receivedString);
      receivedString = "";
  }
}

int move(double x, double y, double z, double zr)
{
  Cartesian target;
  target.x = x;
  target.y = y;
  target.z = z;
  target.zr = 2*PI*zr/360;
  Joint config = inverseKinematics(target);
  Serial.print("Postępowy ruch (z): "+String(config.z-jpos.z)+"\n");
  Serial.print("Kąt obrotu członu 1 (theta1): "+String(config.theta1-jpos.theta1)+"rad\n");
  Serial.print("Kąt obrotu członu 2 (theta2): "+String(config.theta2-jpos.theta2)+"rad\n");
  Serial.print("Kąt obrotu chwytaka (theta_g): "+String(config.theta_g-jpos.theta_g)+"rad\n");
  return 1;
}

int move(Cartesian target)
{
  target.zr = 2*PI*target.zr/360;
  Joint config = inverseKinematics(target);
  Serial.print("Postępowy ruch (z): "+String(config.z-jpos.z)+"\n");
  Serial.print("Kąt obrotu członu 1 (theta1): "+String(config.theta1-jpos.theta1)+"rad\n");
  Serial.print("Kąt obrotu członu 2 (theta2): "+String(config.theta2-jpos.theta2)+"rad\n");
  Serial.print("Kąt obrotu chwytaka (theta_g): "+String(config.theta_g-jpos.theta_g)+"rad\n");
  return 1;
}

int movej(double x, double y, double z, double zr)
{
  Joint config;
  Serial.print("Postępowy ruch (z): "+String(config.z-jpos.z)+"\n");
  Serial.print("Kąt obrotu członu 1 (theta1): "+String(config.theta1-jpos.theta1)+"rad\n");
  Serial.print("Kąt obrotu członu 2 (theta2): "+String(config.theta2-jpos.theta2)+"rad\n");
  Serial.print("Kąt obrotu chwytaka (theta_g): "+String(config.theta_g-jpos.theta_g)+"rad\n");
  return 1;
}

int movej(Joint config)
{
  Serial.print("Postępowy ruch (z): "+String(config.z-jpos.z)+"\n");
  Serial.print("Kąt obrotu członu 1 (theta1): "+String(config.theta1-jpos.theta1)+"rad\n");
  Serial.print("Kąt obrotu członu 2 (theta2): "+String(config.theta2-jpos.theta2)+"rad\n");
  Serial.print("Kąt obrotu chwytaka (theta_g): "+String(config.theta_g-jpos.theta_g)+"rad\n");
  return 1;
}

void interpretacja(String &komenda)
{
  SimpleVector<int> hash(1);
  
  hash.push_back(komenda.indexOf('#'));
  while(hash.back()>-1)
  {
    hash.push_back(komenda.indexOf('#',hash.back()+1));
  }
  
  String* k1=new String(komenda.substring(hash[0]+1,hash[1]));
  //arg = komenda.substring(bracket1+1,bracket2-1);
  switch (getFunct(k1->c_str()))
  {
    case CONNECT:
    Serial.println("#CONNECT#");
    break;

    case POINT:
      if(hash.size()-1==3)
      {
        String* k2=new String(komenda.substring(hash[1]+1,hash[2]));
        cvar.put(k2->c_str(),cpos);
         delete k2;
      }
      else if(hash.size()-1==7)
      {
        String* k2=new String(komenda.substring(hash[1]+1,hash[2]));
        Cartesian tmp;
        tmp.x=komenda.substring(hash[2]+1,hash[3]).toDouble();
        tmp.y=komenda.substring(hash[3]+1,hash[4]).toDouble();
        tmp.z=komenda.substring(hash[4]+1,hash[5]).toDouble();
        tmp.zr=komenda.substring(hash[5]+1,hash[6]).toDouble();
        cvar.put(k2->c_str(),tmp);
        delete k2;
      }
      else
      {
        Serial.println("Wrong number of arguments\n");
      }
    break;

    case POINTJ:
      if(hash.size()-1==3)
      {
        String* k2=new String(komenda.substring(hash[1]+1,hash[2]));
        jvar.put(k2->c_str(),jpos);
        delete k2;
      }
      else if(hash.size()-1==7)
      {
        String* k2=new String(komenda.substring(hash[1]+1,hash[2]));
        Joint tmp;
        tmp.z=komenda.substring(hash[2]+1,hash[3]).toDouble();
        tmp.theta1=komenda.substring(hash[3]+1,hash[4]).toDouble();
        tmp.theta2=komenda.substring(hash[4]+1,hash[5]).toDouble();
        tmp.theta_g=komenda.substring(hash[5]+1,hash[6]).toDouble();
        jvar.put(k2->c_str(),tmp);
        delete k2;
      }
      else
      {
        Serial.println("Wrong number of arguments\n");
      }
    break;

    case MOVE:
      if(hash.size()-1==3)
      {
        String* k2=new String(komenda.substring(hash[1]+1,hash[2]));
        
        Cartesian C;
        
        if(cvar.exists(k2->c_str()))
        {
          C=cvar.getElement(k2->c_str());
          move(C);
        }
        else
        {
          Serial.println("There is no variable like "+*k2+"\n");
        }
        delete k2;
      }
      else if(hash.size()-1==6)
      {
        int x,y,z,zr;
        x=komenda.substring(hash[1]+1,hash[2]).toDouble();
        y=komenda.substring(hash[2]+1,hash[3]).toDouble();
        z=komenda.substring(hash[3]+1,hash[4]).toDouble();
        zr=komenda.substring(hash[4]+1,hash[5]).toDouble();
        move(x,y,z,zr);
      }
      else
      {
        Serial.println("Wrong number of arguments\n");
      }

    break;
    case MOVEJ:
      if(hash.size()-1==3)
      {
        String* k2=new String(komenda.substring(hash[1]+1,hash[2]));
      
        Joint J;
       if(jvar.exists(k2->c_str()))
        {
          J=jvar.getElement(k2->c_str());
          movej(J);
        }
        else
        {
          Serial.println("There is no joint variable like "+*k2+"\n");
        }
        delete k2;
      }
      else if(hash.size()-1==6)
      {
        int x,y,z,zr;
        x=komenda.substring(hash[1]+1,hash[2]).toDouble();
        y=komenda.substring(hash[2]+1,hash[3]).toDouble();
        z=komenda.substring(hash[3]+1,hash[4]).toDouble();
        zr=komenda.substring(hash[4]+1,hash[5]).toDouble();
        move(x,y,z,zr);
      }
      else
      {
        Serial.println("Wrong number of arguments\n");
      }

    break;
    
    default:
    Serial.println("Wrong format \n");
  }
  delete k1;
  
}

void setup()
{
  servoMotor.attach(servoPin);  // Attach the servo to the pin
  Serial.begin(9600,SERIAL_8N2);
  threads.begin();
  threads.addTask(communication, threads.convertMs(500));
}

void loop()
{}