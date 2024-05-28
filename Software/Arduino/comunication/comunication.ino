#include <Servo.h>
#include "leOS2.h"


leOS2 threads;  
Servo servoMotor;
String receivedString = "";

const int servoPin = 9;

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
        receivedString = "";
      }
    }
  }
}

void interpretacja(String komenda)
{
  int bracket1 = komenda.indexOf('(');
  String k = command.substring(0,bracket1-1);
  switch k
  {
    case "MOVE", "move":

    break;

    default:
    Serial.println("Wrong format");
  }
}

void setup()
{
  // Attach the servo to the pin
  servoMotor.attach(servoPin);
   Serial.begin(9600);
   threads.begin();
   threads.addTask(communication, threads.convertMs(1000));
   threads.addTask(interpretacja, threads.convertMs(1000));
}