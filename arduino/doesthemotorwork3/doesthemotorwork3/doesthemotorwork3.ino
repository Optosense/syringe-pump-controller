//  CODE TO CONTROL BOTH MOTORS FOR SERINGUE PUMP, 
#include <AccelStepper.h>


// here modify Digital pins as needed
#define m1StepPin 3
#define m1DirPin  2
#define m2StepPin 5
#define m2DirPin  4
#define motorInterfaceType 1 // must be set to 1 when using a driver

#define m1EnablePin 8
#define m2EnablePin 9

//VARIABLES

//B is motor 2 and A is motor 1

AccelStepper motor1(motorInterfaceType, m1StepPin, m1DirPin); //we specify we are using a driver
AccelStepper motor2(motorInterfaceType, m2StepPin, m2DirPin);



void preSetup() {
  // Set pin modes for step and direction
  pinMode(m1StepPin, OUTPUT);
  pinMode(m1DirPin, OUTPUT);
  digitalWrite(m1StepPin, LOW); // Ensure step pin is LOW
  digitalWrite(m1DirPin, LOW);  // Set direction to a default state

  pinMode(m2StepPin, OUTPUT);
  pinMode(m2DirPin, OUTPUT);
  digitalWrite(m2StepPin, LOW);
  digitalWrite(m2DirPin, LOW);

  pinMode(m1EnablePin, OUTPUT);
  pinMode(m2EnablePin, OUTPUT);
  digitalWrite(m1EnablePin, HIGH); // HIGH to disable motors
  digitalWrite(m2EnablePin, HIGH);

}


void setup() {
  preSetup();
  digitalWrite(m1EnablePin, LOW); // LOW to enable motors
  digitalWrite(m2EnablePin, LOW);
  
  Serial.begin(9600);                     // set up Serial library at 9600 bps
  Serial.println("Code start");
  
  motor1.setMaxSpeed(1000);
 
  motor2.setMaxSpeed(1000);
  
}

void loop() {
  motor1.move(-200);
  motor2.move(-200);
  Serial.println("-200");
  while(motor1.distanceToGo() != 0){
     motor1.setSpeed(100);
     motor1.runSpeedToPosition();
     motor2.setSpeed(50);
     motor2.runSpeedToPosition();
  }
  delay(2000);
  motor2.move(200);
  motor1.move(200);
  Serial.println("+200");
  while(motor1.distanceToGo() != 0){
     motor1.setSpeed(100);
     motor2.setSpeed(50);
     motor1.runSpeedToPosition();
     motor2.runSpeedToPosition();
     
  }
  delay(2000);
  
}
