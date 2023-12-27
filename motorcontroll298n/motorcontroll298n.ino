// code modified from  <https://www.hibit.dev>
// and some other BT serial code on the internet
// -------------------------------------------------


#include <SoftwareSerial.h>
#include "parseActions.h"
#include "motorStruct.h"
#include "directionGroup.h"
#include "motorGroup.h"
#include "speedGroup.h"


//init software serial pins as A0 and A1 as 14,15
#define SERIAL_RX  14
#define SERIAL_TX  15
SoftwareSerial BTserial(SERIAL_RX, SERIAL_TX); // RX | TX


// Motor 2
#define ENA_PIN_FRONT 5 //PWM
#define IN1_PIN_FRONT 2
#define IN2_PIN_FRONT 3

// Motor 1
#define ENB_PIN_FRONT 6 //PWM
#define IN3_PIN_FRONT 4
#define IN4_PIN_FRONT 7


//motor 4 
#define ENA_PIN_REAR 10
#define IN1_PIN_REAR 8
#define IN2_PIN_REAR 9

// MOTOR 3 
#define ENB_PIN_REAR 11
#define IN3_PIN_REAR 12
#define IN4_PIN_REAR 13



//motor 1  is front right 
//motor 2 is front left 
//motor 3 is back right
//motor 4 is back left
motor motorOne, motorTwo, motorThree,motorFour;

void setup()
{ 

  Serial.begin(9600);

  // Set PWM & direction pins to output for front motor Driver
  // motor 2 
  pinMode(ENA_PIN_FRONT, OUTPUT);
  pinMode(IN1_PIN_FRONT, OUTPUT);
  pinMode(IN2_PIN_FRONT, OUTPUT);
 // motor 1 
  pinMode(ENB_PIN_FRONT, OUTPUT);
  pinMode(IN3_PIN_FRONT, OUTPUT);
  pinMode(IN4_PIN_FRONT, OUTPUT);

  // Set PWM & direction pins to output for back motor Driver
  // motor 4 
  pinMode(ENA_PIN_REAR, OUTPUT);
  pinMode(IN1_PIN_REAR, OUTPUT);
  pinMode(IN2_PIN_REAR, OUTPUT);
 // motor 3
  pinMode(ENB_PIN_REAR, OUTPUT);
  pinMode(IN3_PIN_REAR, OUTPUT);
  pinMode(IN4_PIN_REAR, OUTPUT);



  // Init with default values
  sendToMotorOne();
  sendToMotorTwo();
  sendToMotorThree();
  sendToMotorFour();

  BTserial.begin(9600);  

  BTserial.println("Bluetooth Serial Started");
}

void loop()
{
     String receivedString = "blank";
    // Keep reading from HC-06 and send to Arduino Serial Monitor
    if (BTserial.available()>0)
    {  
        int numChars = BTserial.available();
        // Call the function to convert the available characters to a string
        receivedString = readSerialString(numChars);
        // Process the received string
        // processReceivedString(receivedString);
        // displayAction(receivedString);
        int driveCode = performAction(receivedString);

         if (driveCode == 1) {
            // Action 1
            driveForwards();
          } else if (driveCode == 2) {
            // Action 2
            driveBackwards();
          } else if (driveCode == 3) {
            // Action 3
            driveLeft();
          } else if (driveCode == 4) {
            // Action 4
            driveRight();
    }

    }
         
        // BTserial.write("processed");
    // Keep reading from Arduino Serial Monitor and send to HC-06
    if (Serial.available())
    {
        BTserial.write(Serial.read());
    }
  delay(60);

}

///////////////////////////Motor functions///////////////////////////////
void driveForwards(){
 BTserial.println("driving forwards.");
  for (int speed = 50; speed <= 255; speed++) {
    setMotorDirectionForward(motorOne);
    setMotorDirectionForward(motorTwo);
    setMotorDirectionForward(motorThree);
    setMotorDirectionForward(motorFour);

    setMotorSpeed(motorOne, speed);
    setMotorSpeed(motorTwo, speed);
    setMotorSpeed(motorThree, speed);
    setMotorSpeed(motorFour, speed);
      
    sendToMotorOne();
    sendToMotorTwo();
    sendToMotorThree();
    sendToMotorFour();

    delay(20); // Add small delay between changes
  }

    delay(2000);
  for (int speed = 255; speed >= 0; speed--) {
  

    setMotorSpeed(motorOne, speed);
    setMotorSpeed(motorTwo, speed);
    setMotorSpeed(motorThree, speed);
    setMotorSpeed(motorFour, speed);
      
    sendToMotorOne();
    sendToMotorTwo();
    sendToMotorThree();
    sendToMotorFour();
  delay(20);
  }
BTserial.println("driving forwards complete");
}

void driveBackwards(){
  return 0;
};
// Action 3
void driveLeft(){
  return 0;
};
// Action 4
void driveRight(){
  return 0;
};

void sendToMotorTwo(){
  sendToMotor(motorTwo, ENA_PIN_FRONT, IN1_PIN_FRONT, IN2_PIN_FRONT);
}

void sendToMotorOne(){
  sendToMotor(motorOne, ENB_PIN_FRONT, IN3_PIN_FRONT, IN4_PIN_FRONT);
}

void sendToMotorFour(){
  sendToMotor(motorFour, ENA_PIN_REAR, IN1_PIN_REAR, IN2_PIN_REAR);
}

void sendToMotorThree(){
  sendToMotor(motorThree, ENB_PIN_REAR, IN3_PIN_REAR, IN4_PIN_REAR);
}



void increaseMotorsSpeed()
{
  for (int speed = 50; speed <= 255; speed++) {
    setMotorSpeed(motorOne, speed);
    setMotorSpeed(motorTwo, speed);
    setMotorSpeed(motorThree, speed);
    setMotorSpeed(motorFour, speed);
      
    sendToMotorOne();
    sendToMotorTwo();
    sendToMotorThree();
    sendToMotorFour();

    delay(20); // Add small delay between changes
  }
}


void decreaseMotorsSpeed()
{
  for (int speed = 255; speed >= 0; speed--) {
    setMotorSpeed(motorOne, speed);
    setMotorSpeed(motorTwo, speed);
    setMotorSpeed(motorThree, speed);
    setMotorSpeed(motorFour, speed);
      
    sendToMotorOne();
    sendToMotorTwo();
    sendToMotorThree();
    sendToMotorFour();


    delay(20); // Add small delay between changes
  }
}

/////////////////////////////Bluetooth functions 
String readSerialString(int numChars) {
  String result = ""; // Initialize an empty String

  // Read each character from the serial buffer and append it to the result string
  for (int i = 0; i < numChars; ++i) {
    char incomingByte = BTserial.read();
    result += incomingByte;
  }

  return result;
}

// void processReceivedString(String str) {
//   // Process the received string as needed
  
//   Serial.println((int)str);
// }