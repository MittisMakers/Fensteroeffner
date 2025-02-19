#include "arduino_secrets.h"
#include <ezButton.h>
#include <AccelStepper.h>
#include "thingProperties.h"

//Stepper Pins
#define DIR_PIN 17
#define STEP_PIN 16
#define ENABLE_PIN 25
#define MOTOR_INTERFACE_TYPE 1

//Stepper init
AccelStepper stepper(MOTOR_INTERFACE_TYPE, STEP_PIN, DIR_PIN);

//Stepper Values
const double maxSpeedValue = 4000;
const double acceleration = 500;

//Values Oeffner
bool stop = false;

//endswitch Pins
#define END_SWITCH_PIN_1 14
#define END_SWITCH_PIN_2 27

void setup() {
  Serial.begin(9600);
  delay(1500); 
  initProperties();

  //pin modes
  pinMode(END_SWITCH_PIN_1, INPUT_PULLUP);
  pinMode(END_SWITCH_PIN_2, INPUT_PULLUP);

  //motor config
    stepper.setMaxSpeed(maxSpeedValue);
    stepper.setAcceleration(acceleration);
    delay(10);
    digitalWrite(ENABLE_PIN,LOW);
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
}

void loop() {
  ArduinoCloud.update();
}

int switchCheck(){
  int n = 0;
  if(digitalRead(END_SWITCH_PIN_1)==HIGH){
    n = 1;
  }
  if(digitalRead(END_SWITCH_PIN_2)==HIGH) {
    n = 2;
  } 
  return n;
}

void fo_open(){ //open
  digitalWrite(ENABLE_PIN,HIGH);
  stepper.setSpeed(maxSpeedValue);
  if(switchCheck()==2){
    stop = true;
  }
  while(!stop){
      if(switchCheck()==2){
        stop = true;
      }
    stepper.runSpeed();
  }
  digitalWrite(ENABLE_PIN,LOW);
 }

void fo_close(){ //close
  digitalWrite(ENABLE_PIN,HIGH);
  stepper.setSpeed(maxSpeedValue*(-1));
  if(switchCheck()==1){
    stop = true;
  }
  while(!stop){
      if(switchCheck()==1){
        stop = true;
      }
    stepper.runSpeed();
  }
  digitalWrite(ENABLE_PIN,LOW);
}

void onFoStatusChange()  {
  switch(fo_status){
    case true:
      stop = false;
      fo_open();
      break;
    case false:
      stop = false;
      fo_close();
      break;
  }
}