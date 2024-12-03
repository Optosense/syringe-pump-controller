
#include <AccelStepper.h>



const int stepsPerRev= 200; //amount for NEMA 17
const int steps_1ml = stepsPerRev*6/8 ;

// Define stepper motor connections and steps per revolution
// here modify Digital pins as needed
#define motor1StepPin 2
#define motor1DirPin  3
#define motor2StepPin 4
#define motor2DirPin  5

// #define sensor A0 // pin for the sensor

AccelStepper motor1(AccelStepper::DRIVER, motor1StepPin, motor1DirPin); //we specify we are using a driver
AccelStepper motor2(AccelStepper::DRIVER, motor2StepPin, motor2DirPin);

float currentPosition = 0;

//calibrates the seringue to 0ml
void calibrate(){
  int read_value = analogRead(sensor);
  if(read_value!=0){
    Serial.println("déja calibré à 0ml");
  }
  else{
    while(analogRead(sensor)==0){
      Serial.println("calibrating...");
      motor1.move(-2);
      motor1.run();
    }
    Serial.println("calibré");
    currentPosition = 0;
  }
   
}

bool amountAllowed(float currentPosition, float value){
  if((currentPosition + value) >10 || (currentPosition +value <0)){
    Serial.println("Amount not allowed, try again");
    return 0;
  }
  else return 1;
}

void setup() {
  motor1.setMaxSpeed(1000);
  motor1.setAcceleration(500); //if acceleration is not specified, motor will run at maxspeed
  
  motor2.setMaxSpeed(1000);
  motor2.setAcceleration(500);
  
  Serial.begin(9600);                     // set up Serial library at 9600 bps
  Serial.println("Test: code version 5, on va calibrer");
  calibrate();
  
}


void loop() {
  Serial.println("PULL or PUSH syringe?");
  
  // Wait for input1
  while (!Serial.available()) {
    // Do nothing, just wait for input
  }
  String input1 = Serial.readString();
  input1.trim(); // Remove any \r \n whitespace at the end of the String

  Serial.println("Please write volume in mL: ");
  
  // Wait for input2
  while (!Serial.available()) {
    // Do nothing, just wait for input
  }
  floa`+t value = Serial.parseFloat();
  Serial.println(value);

  int steps = value * steps_1ml; //no change!

  if(input1=="PUSH"){
    value*=-1;
    steps*=-1;
  }
  
 
  if(amountAllowed(currentPosition, value)){
    
      currentPosition += value;
    
      if (input1 == "PUSH") {
        Serial.println("Pushing... amount of steps:");
     }
      if (input1 == "PULL") {
        Serial.println("Pulling...amount of steps: ");
      }
      Serial.println(steps);
      motor1.move(steps);
      motor1.run();
      delay(1000);
  }
  
  
  delay(2000); // Add a delay before the next loop iteration
}
