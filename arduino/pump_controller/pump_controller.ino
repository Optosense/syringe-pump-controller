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

#define buttonPin1 12
#define buttonPin2 13

//VARIABLES

const float Qv = 1; // debit ml / s
const int stepsPerRev= 828; // 200 full step, 400 half step , 800 1/4, 1600 1/8, 3200 microstep 1/16 
// Setting on the A4988 Driver
// MS1  MS2  MS3     Microstep resolution   Steps per rev Nema 17:
// L    L    L        Full step             200
// H    L    L        Half Step             400
// L    H    L        Quarter Step          800
// H    H    L        1/8 Step              1600
// H    H    H        1/16 Step             3200

const float steps_1ml = stepsPerRev*5/8 ;  //information peut changer avec le nouveau pousse seringe 
float currentPosition1ml = -12.0;
float currentPosition2ml = -12.0;
int buttonState1 = 0; 
int buttonState2 = 0; 

// 6cm -> 12 mL
// 800step -> 8mm

// 1ml -> 5mm

// steps_1ml = 5*stepsPerRev/8


//Motor 1 is A
//Motor 2 is B
AccelStepper motor1(motorInterfaceType, m1StepPin, m1DirPin); //we specify we are using a driver
AccelStepper motor2(motorInterfaceType, m2StepPin, m2DirPin);



 // variables to hold the parsed data
float floatMotor1 = 0.0; //A
float floatMotor2 = 0.0; //B


//calibrates the seringue to 12ml PROBABY WE WILL USE A BUTTON

void calibrate1(){
   buttonState1 = digitalRead(buttonPin1);
   if (buttonState1 == LOW) {
    ;//Serial.println("M1 déja calibré à 12ml");
  } 
  else {
    //Serial.println("calibrating M1...");
    
    enableMotors();
    while(digitalRead(buttonPin1) == HIGH){
      motor1.setSpeed(-stepsPerRev);
      motor1.runSpeed();
    }
    disableMotors();
    
  }
  //Serial.println("done");
  currentPosition1ml = -12.0;
}


void calibrate2(){
   buttonState2 = digitalRead(buttonPin2);
   if (buttonState2 == LOW) {
    ;//Serial.println("M2 déja calibré à 12ml");
  } 
  else {
    // Serial.println("calibrating M2...");
    
    enableMotors();
    while(digitalRead(buttonPin2) == HIGH){
      motor2.setSpeed(-stepsPerRev);
      motor2.runSpeed();
    }
    disableMotors();
    
  }
  //Serial.println("done");
  currentPosition2ml = -12.0;
}



//function checks if amount goes over seringue capacity
bool amountAllowed(float f1, float f2){

  if((f1 + f2) <-12.0 || (f1 + f2 >0.0)){
  
    Serial.println("Amount not allowed, try again");
    return 0;
  
  }
  else return 1;
} 

void setup() {

  preSetup();
  disableMotors();
  
  motor1.setMaxSpeed(1000);
  motor2.setMaxSpeed(1000);

  Serial.begin(9600);                     // set up Serial library at 9600 bps
  /*
  Serial.println("Code start");
  Serial.println("This demo expects 2 float values. The amount to pull or push from the seringe. Both values need to be either positive or negative");
  Serial.println("Send the first value, enter, then the second value, then enter. ");
  Serial.println();

  Serial.println("calibrate motors? y/n");
  Serial.println();
  */

  //this section receives the info from the Raspberry Pi through the Serial
  //Serial.print("expecting n")
  while (!Serial);

  /*
  while (!Serial.available()) {
    // Do nothing, just wait for input
  }
  char input = Serial.read();
  if(input == 'y'){
    calibrate1();
    calibrate2();
  }
  */
}

void loop() {

  //this section receives the info from the Raspberry Pi through the Serial
  /*
  while (!Serial.available()) {
    // Do nothing, just wait for input
  }
  floatMotor1 = Serial.parseFloat();
  
  while (!Serial.available()) {
    // Do nothing, just wait for input
  }
  floatMotor2 = Serial.parseFloat();
  */
  // Expected string format : "y2.5#3.0\n"
  while (!Serial.available());

  char calibrate_char = Serial.read();
  if (calibrate_char == 'y') {
    calibrate1();
    calibrate2();
  }
  
  floatMotor1 = Serial.parseFloat();
  Serial.println(floatMotor1);
  floatMotor2 = Serial.parseFloat();
  Serial.println(floatMotor2);

  if (Serial.available()) {
    Serial.read(); // consume the extra \n
  }
  

  //converting the volume into steps
  float steps1 = floatMotor1 * steps_1ml; //no change!
  float steps2 = floatMotor2 * steps_1ml; //no change!

   // v1 and v2 are the respective speeds/debits for each seringue (steps/second)
  const float t = (floatMotor1 + floatMotor2) / Qv;
  float v1 = steps1/t;
  float v2 = steps2/t;


  if(amountAllowed(currentPosition1ml, floatMotor1) && (currentPosition2ml, floatMotor2)){
    enableMotors();
    currentPosition1ml += floatMotor1;
    currentPosition2ml += floatMotor2;
  
      
  
    Serial.println("moving motor 1 steps and speed : " );
    Serial.println(steps1);
    Serial.println(v1);
    
    Serial.println("moving motor 2 steps and speed : " );
    Serial.println(steps2);
    Serial.println(v2);

    //we tell the motors the position to move 
    motor1.move((int)steps1);
    motor2.move((int)steps2);

    //the while runs until they reach the Target Position
    while((motor1.distanceToGo() != 0) ||( motor2.distanceToGo()!=0)){
      motor1.setSpeed(-v1);  // watch for speed value when its above 1000 step/s
      motor1.runSpeedToPosition();
      motor2.setSpeed(-v2);
      motor2.runSpeedToPosition();
    }
    
    disableMotors();
    
  }
 

  // delay(1000); // Add a delay before the next loop iteration
  Serial.println("End loop");
}

//to avoid problems with the pins
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

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

}

void enableMotors(){
  digitalWrite(m1EnablePin, LOW); // LOW to enable motors
  digitalWrite(m2EnablePin, LOW);
  
}

void disableMotors(){
  digitalWrite(m1EnablePin, HIGH); // HIGH to disable motors
  digitalWrite(m2EnablePin, HIGH);

}
