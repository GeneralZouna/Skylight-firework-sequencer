int program_steps[][2] = {{1,1000},      //program that defines launch times
                     {4,1000},           // {pin,waiting time}
                     {7,1000},
                     {5,1000},
                     {8,1000},
                     {6,1000},
                     {3,1000},
                     {1,1000},
                     {5,1000},
                     {6,1000},
                     {7,1000},
                     {8,1000},
                     {7,1000},
                     {9,1000},
                     {9,1000},
                     {0,0}}; // the list must end with pin = 0 or there will be undefined behaviour

long nextStep;
int stepIndex = 0;

void Start_launch()//   4 start             starts the program
{

  Serial.println((int) sizeof(program_steps));
  if (millis()>= nextStep){
    SetPinOn(program_steps[stepIndex][0]);
    nextStep = millis() + (long) program_steps[stepIndex][1];
    stepIndex ++;
  }

  if (program_steps[stepIndex][0] == 0){
    stepIndex = 0;
    ProgramActive = false;
  }
}                     
