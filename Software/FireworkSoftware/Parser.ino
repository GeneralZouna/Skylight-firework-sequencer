
void InputRead(){
  
  /*
  * check for data
  * read data
  * interpret data
  * send data to do what must be done
  * exit
  */



  /*
  *   Input commands:
  *
  *   0 No Code /empty
  *
  *   1 ports: <int>      Sets number of outputs
  *   2 mode: <byte/flag> chooses mode Program/Manual/Random
  *   3 stop              turns all outputs to 0, stops execution of programs
  *   4 start             starts the program
  *   5 on_time <int>           sets ON_TIME
  *   6 on_port <int>     set how many ports are allowed to be on at the same time
  *  
  */
  
  //if no data skip execution
  if(Serial.available() == 0){return;}
    
    char command_code =  0;
    int flag = 0;
    String ReadData = "";

    while (Serial.available() > 0){
      char a = Serial.read();
      ReadData += a;
      
      ReadData.trim();

      if (ReadData.startsWith("ports")) {command_code = 1; ReadData = "";}
      else if (ReadData.startsWith("mode")) {command_code = 2; ReadData = "";}
      else if (ReadData.startsWith("stop")) {command_code = 3; ReadData = "";}
      else if (ReadData.startsWith("start")) {command_code = 4; ReadData = "";}
      else if (ReadData.startsWith("on_time")) {command_code = 5; ReadData = "";}
      else if (ReadData.startsWith("on_port")) {command_code = 6; ReadData = "";}
  
    }
    flag = ReadData.toInt();

    Serial.print("Command: ");
    Serial.print(command_code, DEC);

    Serial.print("\tFlag: ");
    Serial.print(flag, DEC);

    Serial.print("\tRemainder: ");
    Serial.println(ReadData);

  //if command is not oke skip
  if(command_code <= 0 || command_code >6){return;}

  switch (command_code){
    
    case 1:
        SetPorts(flag);
      break;
    case 2:
      break;
    case 3:
      break;
    case 4: 
      break;
    case 5: 
        SetOnTime(flag);
      break;  
  }
   /* if (ReadData.toInt() >  PortLength || ReadData.toInt() <= 0){return;}
    for(int i = 0; i < MAX_ON_PINS; i++){
      if(TimerArr[i][0] == 0){
        TimerArr[i][1] = ON_TIME;
        TimerArr[i][0] = ReadData.toInt();
        break;
      }
    }
    */
}



void SetPorts(int ports)//   1 ports: <int>      Sets number of outputs
{
  if(ports >= 1 && ports < 1000){
        PortLength = (ports / 8)*8;
        if(ports > 0){PortLength += 1;}
      }
}


void SetMode(int mode){}//  2 mode: <byte/flag> chooses mode Program/Manual/Random
void STOP_ALL(){}//   3 stop              turns all outputs to 0, stops execution of programs
void Start_launch(int pin)//   4 start             starts the program
{}
void SetOnTime(int onTime)//   5 on_time <int>           sets ON_TIME
{
  ON_TIME = onTime;
}

