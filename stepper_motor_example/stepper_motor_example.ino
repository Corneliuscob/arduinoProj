#include <AccelStepper.h>
#include <MultiStepper.h>

//pin connections 
const int dirPin = 3;
const int stepPin = 5;

#define motorInterfaceType 1

AccelStepper  myStepper(motorInterfaceType,stepPin,dirPin);



void setup() {
  // set the maximum speed ,acceleration factor
  // initial speed and target position 
  myStepper.setMaxSpeed(500);
  myStepper.setAcceleration(100);
  myStepper.setSpeed(100);
  
  myStepper.moveTo(200);
}

void loop() {
  // change the direction once the motor reaches the target position 
  if (myStepper.distanceToGo() == 0) 
    myStepper.moveTo(-myStepper.currentPosition());

  // Move the motor one step
  myStepper.run();  

}
