#include <espnow.h>
#include <ESP8266WiFi.h>

#define LATCH_pin 12 //LATCH pin on shift register
#define DATA_pin 14  //DATA_in pin on shift register
#define CLK_pin 13   //CLOCK pin on shift register
#define OUTPUT_ENABLE_pin 5  // Output Enable pin on both 74HC595s (74HC595 Pin 13). Must be pulled LOW to enable the outputs

#define MAX_OUTPUT 16

char16_t output_buffer = 0;
long long pin_timeout[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

struct recived_message{
  char pin;
  long on_time;
};

recived_message Data;

void UpdateOutput(){

   digitalWrite(LATCH_pin, LOW);
   for(int i = MAX_OUTPUT-1; i >= 0; i--){
      digitalWrite(CLK_pin,LOW);
      digitalWrite(DATA_pin, (millis() < pin_timeout[i]));
      digitalWrite(CLK_pin,HIGH);
      
   }
   digitalWrite(LATCH_pin, HIGH);
   
}


void OnDataRecv(unsigned char* mac, unsigned char* incomingData, unsigned char len) {
  memcpy(&Data, incomingData, sizeof(Data));
  Serial.print("Recived message from MAC: " );
  Serial.print(mac[0],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.println(mac[5],HEX);
  Serial.println("\tPin: " + String((int) Data.pin));
  Serial.println("\tOn time: " + String(Data.on_time));
}


void setup() {
  pinMode(LATCH_pin, OUTPUT);
  pinMode(DATA_pin, OUTPUT);
  pinMode(CLK_pin, OUTPUT);
  pinMode(OUTPUT_ENABLE_pin, OUTPUT);

  digitalWrite(OUTPUT_ENABLE_pin,HIGH);

  digitalWrite(LATCH_pin, LOW);
  shiftOut(DATA_pin,CLK_pin,MSBFIRST,0);
  shiftOut(DATA_pin,CLK_pin,MSBFIRST,0);
  digitalWrite(LATCH_pin, HIGH);
  digitalWrite(LATCH_pin, LOW);
  digitalWrite(OUTPUT_ENABLE_pin,LOW);


  Serial.begin(115200);
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  Serial.print("Current MAC: ");
  Serial.println(WiFi.macAddress());
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

  Serial.println("Finished Setup");
}

void loop() {
  UpdateOutput();

  if(Data.pin > 0){
    Serial.println("New data pin " + String(Data.pin));
    if(Data.on_time>10000) {Data.on_time=10000;}
    pin_timeout[Data.pin-1] = millis() + Data.on_time;
    Data.pin =0;
    Data.on_time =0;
  }

}