#include <Servo.h>
#include <stdio.h>
#include <string.h>
#include "leOS2.h"

const int servoPin = 9;
leOS2 threads;  
Servo servoMotor;
String receivedString = "";

enum Funct{ MOVE, UNKNOWN };

enum Funct getFunct(const char *str) 
{
    if(strcmp(str, "MOVE") == 0 || strcmp(str, "move") == 0) return MOVE;
    return UNKNOWN;
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

int move(int x, int y, int z)
{
 Serial.print("move( "+String(x)+" , "+String(y)+" , "+String(z)+" )"+"\n");
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
    int hash2,hash3,hash4,x,y,z;
    hash2=komenda.indexOf('#',hash1+1);
    hash3=komenda.indexOf('#',hash2+1);
    hash4=komenda.indexOf('#',hash3+1);
    x=komenda.substring(hash1+1,hash2).toInt();
    y=komenda.substring(hash2+1,hash3).toInt();
    z=komenda.substring(hash3+1,hash4).toInt();
    move(x,y,z);
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