#include <AccelStepper.h>
#include <MultiStepper.h>

//pin connections 
const int dirPin = 3;
const int stepPin = 5;
int pos = 0 ;
#define motorInterfaceType 1

AccelStepper  myStepper(motorInterfaceType,stepPin,dirPin);



void setup() {
  // set the maximum speed ,acceleration factor
  // initial speed and target position 
  myStepper.setMaxSpeed(1000);
  myStepper.setAcceleration(100);
  myStepper.setSpeed(200);
  
//  myStepper.moveTo(200);
}

void loop() {

//Position based movement  
  // myStepper.moveTo(pos);
  // Move the motor one step
  // myStepper.run();  
  // myStepper.setCurrentPosition(0);
//Speed based movement
  myStepper.runSpeed();
  
}
