/*
  Example sketch to control a stepper motor with A4988 stepper motor driver, 
  AccelStepper library and Arduino: continuous rotation. 
  More info: https://www.makerguides.com 
*/

// Include the AccelStepper library:
#include "AccelStepper.h"

// Define stepper motor connections and motor interface type. 
// Motor interface type must be set to 1 when using a driver
#define dirPin 2
#define stepPin 3
#define dirPin2 4
#define stepPin2 5
#define motorInterfaceType 1

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin); //motor b
AccelStepper stepper2 = AccelStepper(motorInterfaceType, stepPin2, dirPin2); //motor b

void setup() {
  // Set the maximum speed in steps per second:
  stepper.setMaxSpeed(1000);
  stepper2.setMaxSpeed(1000);
}

void loop() {
  // Set the speed in steps per second:
  stepper.setSpeed(100);
  stepper2.setSpeed(50);//positive is ccw)
  // Step the motor with a constant speed as set by setSpeed():
  stepper.runSpeed();
  stepper2.runSpeed();
}
