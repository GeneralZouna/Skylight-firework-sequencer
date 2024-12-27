#include <WiFi.h>
//website related
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>

#define MAX_SEQUENCE_LENGTH 320

#define RXD2 16
#define TXD2 17

const char* ssid  = "Skyligt";
const char* password = "";

AsyncWebServer website(80);

struct SequenceEntry {
  String Name = "";
  char Box;
  int StepDelay;
  char Pin;
};

struct ReciverBox {
  String Name;
  unsigned char id;
  char MAC[6];
};

SequenceEntry Sequence[MAX_SEQUENCE_LENGTH];
ReciverBox ReciverList[20];

int SequenceIndex = 0;
bool SequenceRunning = false;

/*######################################################
*               ESP NOW
########################################################*/
const uint8_t slaveNodes[20][6] = {
    {0xE8,0xDB,0x84,0xE7,0x84,0x42}, //E8:DB:84:E7:84:42
    {0x2C,0xF4,0x32,0x1C,0x2B,0x31},
    {0x84,0x0D,0x8E,0x9C,0xD4,0x2E},
    {0x24,0x62,0xAB,0x0C,0xDC,0xCD},
    {0xff,0xff,0xff,0xff,0xff,0xff},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0}
};

struct Packet {
  char pin =0;
  long on_time=5000;
};

struct SerialPacket{
  uint8_t Box = 0;
  Packet sPacket;
};


/*###########################################################################
*               Website related functions
###############################################################################*/
void resetSequenceArray(){
  for (int i=0; i< MAX_SEQUENCE_LENGTH; i++){
    Sequence[i] = SequenceEntry();
  }
}


String processor(const String& var){
  return String();
}

void GetSequenceFile(){
  Serial.println("Getting Sequence from file");
  SequenceEntry* pSequ;
  pSequ = Sequence;
  size_t lenSeq = sizeof(Sequence);
  File myFile = LittleFS.open("/Sequence.bin",FILE_READ);
  myFile.readBytes((char *)&Sequence, myFile.size());
  myFile.close();

}
void SetSequenceFile(){
  Serial.println("Saving Sequence to file");
  size_t lenSeq = sizeof(Sequence);

  File myFile = LittleFS.open("/Sequence.bin",FILE_WRITE);
  myFile.write((byte *)&Sequence, sizeof(Sequence));
  myFile.close();
}

String JsonSeqBuffer = "";
unsigned long JsonSeqBufferReset = millis();

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);


  /*#################################################################################
  *             WIFI and Website setup
  ##################################################################################*/
  //WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid,password);
  //WiFi.begin(ssid,password);
  Serial.println("Access Point Started");
  Serial.print("IP Address: ");
  //Serial.println(WiFi.localIP());
  Serial.println(WiFi.softAPIP());

  if (!LittleFS.begin()) {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  else{
    Serial.println("LittleFS mounted successfully");
    GetSequenceFile();
  }

//websites
  website.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/ControlPanel.html", String(), false, processor);
  });
  website.on("/ControlPanel.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/ControlPanel.html", String(), false, processor);
  });
  website.on("/Sequence.html", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(LittleFS, "/Sequence.html", String(), false, processor);
  });
  
  /* ####################################################################################
  *                             API
  ###########################################################################*/

  website.on("/api/SequenceIndex", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.println("Index requested: "+ String(SequenceIndex));
    request->send(200, "application/json", "[ " + String(SequenceIndex) + " ]" );
  });

  website.on("/api/SequenceTable", HTTP_GET, [](AsyncWebServerRequest *request){
    String json = "[";
    for(int i=0; i<MAX_SEQUENCE_LENGTH;i++){
      if(Sequence[i].Name != ""){
        String entry = "[\"";
        String N = Sequence[i].Name;
        N.replace('"','\"');
        entry += N + "\"," + String(int(Sequence[i].Box)) + "," + String(float(Sequence[i].StepDelay)/1000) + "," + String(int(Sequence[i].Pin)) + "]";
        json += entry + ",";
      }else{
        json.setCharAt(json.length()-1,']');
        break;
      }
    }
    Serial.println("Sending table" + json);
    request->send(200, "application/json", json );
  });

  website.on("/api/Start", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "application/json", String("[") + String((SequenceRunning) ? "1":"0") + String("]") );
  });

  website.on("/api/SequenceIndex", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL,
  [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    String json = "";
    for (size_t i = 0; i < len; i++) {
      json += (char) data[i];
    }
    JsonDocument doc;
    if(deserializeJson(doc, json)){
        request->send(400);
    }else{
    SequenceIndex = doc[0];
    Serial.println("Sequence is set to" + String(doc[0]));
    request->send(200);
    }
  });
  website.on("/api/Start", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL,
  [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
    String json = "";
    for (size_t i = 0; i < len; i++) {
      json += (char) data[i];
    }
    JsonDocument doc;
    if(deserializeJson(doc, json)){
        request->send(400);
    }else{
    Serial.println("Is running is set to" + String(doc[0]));
    SequenceRunning = (int(doc[0])==1);
    request->send(200);
    }
  });
  website.on("/api/SequenceTable", HTTP_POST, [](AsyncWebServerRequest *request){}, NULL,
  [](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {    
    
    for (size_t i = 0; i < len; i++) {
      JsonSeqBuffer += (char) data[i];
    }
    JsonSeqBufferReset = millis() + 1000;
    JsonDocument doc;
    Serial.println("JSON recived: " + JsonSeqBuffer);
    if(deserializeJson(doc, JsonSeqBuffer)){
        request->send(400);
    }else{
      resetSequenceArray();
      for (size_t i=0; i<doc.size();i++){
        JsonArray SeqJsonArr = doc[i].as<JsonArray>();
        const char* hehe = SeqJsonArr[0]; 
        Sequence[i].Name = String(SeqJsonArr[0]);
        Sequence[i].Box = SeqJsonArr[1].as<unsigned char>();
        Sequence[i].StepDelay = SeqJsonArr[2].as<int>();
        Sequence[i].Pin = SeqJsonArr[3].as<unsigned char>();

        Serial.println("Getting sequence with index " + String(i) + " :");
        Serial.println("Name:" + Sequence[i].Name);
        Serial.println("Box:" + String(Sequence[i].Box));
        Serial.println("Delay:" + String(Sequence[i].StepDelay));
        Serial.println("pin:" + String(Sequence[i].Pin));
      }
    SetSequenceFile();
    request->send(200);
    }
  });
  

  website.serveStatic("/", LittleFS, "/");

  website.begin();

}

unsigned long NextStepTimer;
void loop() {
  if (JsonSeqBufferReset < millis() and JsonSeqBuffer.length() > 1 ){
    JsonSeqBuffer = "";
  }

  if (SequenceRunning){
    if(millis() >= NextStepTimer && Sequence[SequenceIndex].Pin>0){
      
      SerialPacket sSerialPacket;

      NextStepTimer +=  Sequence[SequenceIndex].StepDelay;
      sSerialPacket.Box = Sequence[SequenceIndex].Box;
      sSerialPacket.sPacket.on_time = 1000;
      sSerialPacket.sPacket.pin = Sequence[SequenceIndex].Pin;
      
      Serial2.write((uint8_t *) &sSerialPacket,sizeof(sSerialPacket));

      SequenceIndex ++;
      Serial.println("Seqence_index = " + String(SequenceIndex));
    }

    if(SequenceIndex > MAX_SEQUENCE_LENGTH){
      SequenceRunning = false;
    }
  }
  else{
    NextStepTimer = millis();
  }




  if (Serial.available()){
    String SerBuff = "";
    char CharBuff[2];
    enum {NONE,SEQ,PASSWORD} cmd = NONE;
    while(Serial.readBytes(CharBuff,1)){
      SerBuff += String(CharBuff[0]);
      if (SerBuff == "sequence:"){
        SerBuff = "";
        cmd = SEQ;
      }
    }
    if (cmd = SEQ){
      JsonDocument doc;
      Serial.println("JSON recived: " + SerBuff);
      if(deserializeJson(doc, SerBuff)){
        Serial.println("ERROR: problem parsing JSON");
      }else{
        resetSequenceArray();
        for (size_t i=0; i<doc.size();i++){
        JsonArray SeqJsonArr = doc[i].as<JsonArray>();
        const char* hehe = SeqJsonArr[0]; 
        Sequence[i].Name = String(SeqJsonArr[0]);
        Sequence[i].Box = SeqJsonArr[1].as<unsigned char>();
        Sequence[i].StepDelay = SeqJsonArr[2].as<int>();
        Sequence[i].Pin = SeqJsonArr[3].as<unsigned char>();

        Serial.println("Getting sequence with index " + String(i) + " :");
        Serial.println("Name:" + Sequence[i].Name);
        Serial.println("Box:" + String(Sequence[i].Box));
        Serial.println("Delay:" + String(Sequence[i].StepDelay));
        Serial.println("pin:" + String(Sequence[i].Pin));
      }
        SetSequenceFile();
      }
    }
  }

}
