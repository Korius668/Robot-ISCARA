#include <Servo.h>
#include <stdio.h>
#include <string.h>
#include "leOS2.h"
#include <math.h>

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

const int servoPin = 9;
leOS2 threads;  
Servo servoMotor;
String receivedString = "";
Cartesian cposition;
Joint jposition;

enum Funct{ MOVE, UNKNOWN };

enum Funct getFunct(const char *str) 
{
    if(strcmp(str, "MOVE") == 0 || strcmp(str, "move") == 0) return MOVE;
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
      return jposition;
    }
    double alpha = acos(cos_alpha);
    double cos_beta = (L1 * L1 + L2 * L2 - r * r) / (2 * L1 * L2);
    if(cos_beta>1||cos_beta<-1)
    {
      Serial.print("Error: ruch poza zasięgiem robota \n");
      return jposition;
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
  if (Serial.available() > 0)
  {
    while (Serial.available() > 0)
    {
      char receivedChar = Serial.read(); // Odczytanie pojedynczego znaku
      if (receivedChar != '\n')
      {
        receivedString += receivedChar; // Dodanie odczytanego znaku do otrzymanej linii tekstu
      }
      else
      {
        Serial.print("Received: ");
        Serial.println(receivedString);
        interpretacja(receivedString);
        receivedString = "";
      }
    }
  }
}

int move(double x, double y, double z, double zr)
{
  //Serial.print("move( "+String(x)+" , "+String(y)+" , "+String(z)+" , "+String(zr)+" )"+"\n");
  Cartesian target;

    // Przykładowe współrzędne docelowe chwytaka
    target.x = x;
    target.y = y;
    target.z = z;
    target.zr = 2*PI*zr/360;

    Joint config = inverseKinematics(target);

    Serial.print("Postępowy ruch (z): "+String(jposition.z-config.z)+"\n");
    Serial.print("Kąt obrotu członu 1 (theta1): "+String(config.theta1-jposition.theta1)+"rad\n");
    Serial.print("Kąt obrotu członu 2 (theta2): "+String(config.theta2-jposition.theta2)+"rad\n");
    Serial.print("Kąt obrotu chwytaka (theta_g): "+String(config.theta_g-jposition.theta_g)+"rad\n");

 return 1;
}

void interpretacja(String &komenda)
{
  int hash0 = komenda.indexOf('#');
  int hash1 = komenda.indexOf('#',hash0+1);
  String* k1=new String(komenda.substring(hash0+1,hash1));
  // Serial.print(*k1);
  //arg = komenda.substring(bracket1+1,bracket2-1);
  switch (getFunct(k1->c_str()))
  {
    case MOVE:
      int hash2,hash3,hash4,hash5,x,y,z,zr;
      hash2=komenda.indexOf('#',hash1+1);
      hash3=komenda.indexOf('#',hash2+1);
      hash4=komenda.indexOf('#',hash3+1);
      hash5=komenda.indexOf('#',hash4+1);
      x=komenda.substring(hash1+1,hash2).toDouble();
      y=komenda.substring(hash2+1,hash3).toDouble();
      z=komenda.substring(hash3+1,hash4).toDouble();
      zr=komenda.substring(hash4+1,hash5).toDouble();
      move(x,y,z,zr);
    break;

    default:
    Serial.println("Wrong format");
  }
  delete k1;
}

void setup()
{
  servoMotor.attach(servoPin);  // Attach the servo to the pin
  Serial.begin(9600);
  threads.begin();
  threads.addTask(communication, threads.convertMs(1000));
}

void loop()
{
  
}