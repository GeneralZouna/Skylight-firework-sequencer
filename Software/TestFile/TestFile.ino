#define DEBUG

/*
Pins for connection
*/
#define DATA 11
#define CLK 12
#define LATCH 8
#define R_PIN 9

#define FLIPPED_OUTPUTS //pins are NC

/*
Set true if every board is flipped by pins:
Q7 is first LSB; Q1 is MSB
*/
#define FLIPPED_PORTS true  

/*
Number of pins that may be on at one time
WARNING:Bigger number solows down code
*/
#define MAX_ON_PINS 50

//On time for each output in ms
int ON_TIME = 500;  

//number of outputs
#define OUTPUTS 32

/*
Get data from controller
Read SD
Update outputs
*/

int TimerArr_time[MAX_ON_PINS]; //array for storing ticks before switching off
int TimerArr_pin[MAX_ON_PINS];  //array for storing what pin is connected to a timer

int PortLength = OUTPUTS;
unsigned long old_time;

 /*
  *   Input commands:
  *
  *   ports: <int>      Sets number of outputs
  *   mode: <byte/flag> chooses mode Program/Manual/Random
  *   stop              turns all outputs to 0, stops execution of programs
  *   start             starts the program
  *   on_time           sets ON_TIME
  *  
  *
  */




void setup() {
  Serial.begin(9600);
  
}

void loop() {
  //InputRead();
  for (int i = 0; i < PortLength; i++){
  SetPinOn(i);
  for(int j =0; j < 10; j++){
  updateOutputs();
  old_time = millis();
  delay(100);
  }
  Serial.println("");
 }

}





void SetPinOn(int pin,int onTime = ON_TIME){
 if (pin == 0) {return;}

 for (int i = 0; i < MAX_ON_PINS; i++) {
    /*
    *  If you find pin reset timer,
    *  else find empty spot and update it
    */
    if (TimerArr_pin[i] == pin){
     TimerArr_time[i] = onTime;
     return;
    }
    else if (TimerArr_pin[i] == 0) {
      TimerArr_time[i] = onTime;
      TimerArr_pin[i] = pin;
      return;
    }
  }
}



bool TestOutput(int pin) {
  int deltaTime = (int)(millis() - old_time);
  // pin 0 doesn't exist --> code for empty
  if (pin == 0) {
    return false;
  }

  //Find pin and timer; if it's time is pass update it
  for (int i = 0; i < MAX_ON_PINS; i++) {

    /*
    *  If you find pin updte it
    *  If time has elapsed turn it off
    */

    if (TimerArr_pin[i] == pin) {
      deltaTime = (int)(millis() - old_time);
      TimerArr_time[i] -= deltaTime;
      if (TimerArr_time[i] <= 0) {
        TimerArr_pin[i] = 0;
        TimerArr_time[i] = 0;
        return false;
      }
      return true;
    }
  }

  return false;
}


void updateOutputs() {
  char outputbyte = 0;
  char addBit = 0;

  //digitalWrite(R_PIN,HIGH);
  //digitalWrite(R_PIN,LOW);

 
  for (int i = PortLength ; i > 0; i--) {
    addBit = TestOutput(i) ? 1 : 0;
    outputbyte = outputbyte << 1 + addBit;


    
    if ( (i-1) % 8 == 7) {
     
#ifdef DEBUG
    Serial.print(" ");
    Serial.print(i);
    Serial.print(": ");
#endif
     
#ifdef FLIPPED_OUTPUTS
    outputbyte = outputbyte ^ 255;
#endif
      
      shiftOut(DATA, CLK, (FLIPPED_PORTS)? LSBFIRST:MSBFIRST, outputbyte);

      outputbyte = 0;
    }
#ifdef DEBUG
    Serial.print(addBit, BIN);
#endif
  }
  digitalWrite(LATCH,LOW);
  digitalWrite(LATCH,HIGH);
 #ifdef DEBUG
    Serial.println("");
#endif
}
