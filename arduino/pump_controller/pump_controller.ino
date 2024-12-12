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

const int Qv = 2; // debit ml / s
const int stepsPerRev= 200; // 200 full step, 400 half step , 800 1/4, 1600 1/8, 3200 microstep 1/16 
const int steps_1ml = stepsPerRev*6/8 ;  //information peut changer avec le nouveau pousse seringe 
float currentPosition1ml = 0.0;
float currentPosition2ml = 0.0;


//B is motor 2 and A is motor 1
AccelStepper motor1(motorInterfaceType, m1StepPin, m1DirPin); //we specify we are using a driver
AccelStepper motor2(motorInterfaceType, m2StepPin, m2DirPin);



 // variables to hold the parsed data
float floatMotor1 = 0.0;
float floatMotor2 = 0.0;


//calibrates the seringue to 0ml PROBABY WE WILL USE A BUTTON
/*
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
*/


//function checks if amount goes over seringue capacity
bool amountAllowed(float f1, float f2){

  if((f1 + f2) >10 || (f1 + f2 <0)){
  
    Serial.println("Amount not allowed, try again");
    return 0;
  
  }
  else return 1;
} 

void setup() {

  preSetup();
  enableMotors();
  
  motor1.setMaxSpeed(1000);
  motor2.setMaxSpeed(1000);

  Serial.begin(9600);                     // set up Serial library at 9600 bps
  Serial.println("Code start");
  Serial.println("This demo expects 2 float values. The amount to pull or push from the seringe. Both values need to be either positive or negative");
  Serial.println("Send the first value, enter, then the second value, then enter. ");
  Serial.println();
  //calibrate();

}

void loop() {
  disableMotors();
  Serial.println("Start loop");
  Serial.println("Please write data 1: ");
  Serial.println();

  //this section receives the info from the Raspberry Pi through the Serial
  while (!Serial.available()) {
    // Do nothing, just wait for input
  }
  floatMotor1 = Serial.parseFloat();
  Serial.println(floatMotor1);
  
  Serial.println("Please write data 2: ");
  Serial.println();
  while (!Serial.available()) {
    // Do nothing, just wait for input
  }
  floatMotor2 = Serial.parseFloat();
  Serial.println(floatMotor1);

  //converting the volume into steps
  int steps1 = floatMotor1 * steps_1ml; //no change!
  int steps2 = floatMotor2 * steps_1ml; //no change!

   // v1 and v2 are the respective speeds/debits for each seringue (steps/second)
  const int t = (floatMotor1 + floatMotor2) / Qv;
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
    motor1.move(steps1);
    motor2.move(steps2);

    //the while runs until they reach the Target Position
    while((motor1.distanceToGo() != 0) ||( motor1.distanceToGo()!=0)){
      motor1.setSpeed(v1);
      motor1.runSpeedToPosition();
      motor2.setSpeed(v2);
      motor2.runSpeedToPosition();
    }
  
    delay(1000);
    
  }
 

  delay(2000); // Add a delay before the next loop iteration
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

}

void enableMotors(){
  digitalWrite(m1EnablePin, LOW); // LOW to enable motors
  digitalWrite(m2EnablePin, LOW);
  
}

void disableMotors(){
  digitalWrite(m1EnablePin, HIGH); // HIGH to disable motors
  digitalWrite(m2EnablePin, HIGH);

}



//============ 
/*
void recvWithStartEndMarkers() {
    static boolean recvInProgress = false;
    static byte ndx = 0;
    char startMarker = '<';
    char endMarker = '>';
    char rc;

    while (Serial.available() > 0 && newData == false) {
        rc = Serial.read();

        if (recvInProgress == true) {
            if (rc != endMarker) {
                receivedChars[ndx] = rc;
                ndx++;
                if (ndx >= numChars) {
                    ndx = numChars - 1;
                }
            }
            else {
                receivedChars[ndx] = '\0'; // terminate the string
                recvInProgress = false;
                ndx = 0;
                newData = true;
            }
        }

        else if (rc == startMarker) {
            recvInProgress = true;
        }
    }
}

//============

void parseData() {      // split the data into its parts

    char * strtokIndx; // this is used by strtok() as an index

    strtokIndx = strtok(tempChars,",");      // get the first part - the string
    floatMotor1 = atof(strtokIndx); // copy it to messageFromPC
 
    
    strtokIndx = strtok(NULL, ",");
    floatMotor2 = atof(strtokIndx);     // convert this part to a float

}

//============

void showParsedData() {
    
    Serial.print("Float 1 ");
    Serial.println(floatMotor1);
    Serial.print("Float 2 ");
    Serial.println(floatMotor2);
}*/
