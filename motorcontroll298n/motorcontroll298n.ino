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
#define SERIAL_RX = 14;
#define SERIAL_TX = 15;
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
}

void loop()
{
  Serial.println("Motors are stopped now");
  Serial.println("Set direction FORWARD");
  delay(2000);

  setMotorDirectionForward(motorOne);
  setMotorDirectionForward(motorTwo);
  setMotorDirectionForward(motorThree);
  setMotorDirectionForward(motorFour);

  Serial.println("Gradually increase motors speed to max");

  increaseMotorsSpeed();
  
  Serial.println("Motors are on full speed now");
  delay(2000);
  Serial.println("Gradually decrease motors speed to min");

  decreaseMotorsSpeed();

  Serial.println("Motors are stopped now");
  Serial.println("Set direction BACKWARD");
  delay(2000);

  setMotorDirectionBackward(motorOne);
  setMotorDirectionBackward(motorTwo);
  setMotorDirectionBackward(motorThree);
  setMotorDirectionBackward(motorFour);

  Serial.println("Gradually increase motors speed to max");

  increaseMotorsSpeed();

  Serial.println("Motors are on full speed now");
  delay(2000);
  Serial.println("Gradually decrease motors speed to min");

  decreaseMotorsSpeed();

}
void driveForwards(){

  int speed =200;
    setMotorSpeed(motorOne, speed);
    setMotorSpeed(motorTwo, speed);
    setMotorSpeed(motorThree, speed);
    setMotorSpeed(motorFour, speed);
    sendToMotorOne();
    sendToMotorTwo();
    sendToMotorThree();
    sendToMotorFour();

    delay(2000);
  for (speed; speed >= 0; speed--) {
  

    setMotorSpeed(motorOne, speed);
    setMotorSpeed(motorTwo, speed);
    setMotorSpeed(motorThree, speed);
    setMotorSpeed(motorFour, speed);
      
    sendToMotorOne();
    sendToMotorTwo();
    sendToMotorThree();
    sendToMotorFour();
  
  }

}

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

