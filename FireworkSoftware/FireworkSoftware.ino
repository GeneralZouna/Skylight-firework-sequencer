#define DEBUG

#define DATA 1
#define CLK 2
#define LATCH 3
#define R_PIN 4

#define MAX_ON_PINS 300 //bigger number solows down code
int ON_TIME = 5000;  
//Get data from controller
//Read SD
//Update outputs

int TimerArr_time[MAX_ON_PINS];
int TimerArr_pin[MAX_ON_PINS];

int PortLength = 8;
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
  InputRead();
  updateOutputs();
  old_time = millis();
  delay(1000);
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

  digitalWrite(R_PIN,HIGH);
  digitalWrite(R_PIN,LOW);

  for (int i = 1; i < PortLength+1; i++) {
    addBit = TestOutput(i) ? 1 : 0;
    outputbyte = outputbyte << 1 + addBit;

#ifdef DEBUG
    Serial.print( (int) addBit);
#endif

    if (i-1 % 8 == 7) {
      shiftOut(DATA, CLK, MSBFIRST, outputbyte);


      outputbyte = 0;
    }
  }
  digitalWrite(LATCH,LOW);
  digitalWrite(LATCH,HIGH);
  Serial.println("");
}
