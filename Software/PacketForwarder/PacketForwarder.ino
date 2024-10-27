#include <WiFi.h>
//website related
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include <ArduinoJson.h>
//comand of other units related
#include <esp_now.h>

#define RXD2 16
#define TXD2 17

const uint8_t slaveNodes[20][6] = {
    {0xE8,0xDB,0x84,0xE7,0x84,0x42}, //E8:DB:84:E7:84:42
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
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0},
    {0,0,0,0,0,0}
};

struct Packet {
  char pin =0;
  long on_time=1000;
};

struct SerialPacket{
  uint8_t Box = 0;
  Packet sPacket;
};

esp_now_peer_info_t peerInfo;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  char macStr[18];
  Serial.print("Packet from: ");
  // Copies the sender mac address to a string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print(macStr);
  Serial.print(" send status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  for(int i=0; i<20;i++){
    if (slaveNodes[i][0] == slaveNodes[i][1] && slaveNodes[i][2] == slaveNodes[i][1] && slaveNodes[i][2] == slaveNodes[i][3] && slaveNodes[i][3] == slaveNodes[i][4] && slaveNodes[i][4] == slaveNodes[i][5] && slaveNodes[i][5] == 0){break;}
    peerInfo.channel = 0;  
    peerInfo.encrypt = false;
    memcpy(peerInfo.peer_addr, slaveNodes[i], 6);
    if (esp_now_add_peer(&peerInfo) != ESP_OK){
      Serial.println("Failed to add peer " + String(i));
      return;
    }
  }

}

void loop() {
  while (Serial2.available()){
    Serial.println("Getting packet to send");
    Packet DataPacket;
    SerialPacket sSerialPacket;
    Serial2.readBytes((uint8_t *) &sSerialPacket,sizeof(sSerialPacket));
    DataPacket = sSerialPacket.sPacket;
    Serial.println("DataPacket: " + String(DataPacket.on_time) + " --- " + String(int(DataPacket.pin)));
    esp_err_t result = esp_now_send(slaveNodes[sSerialPacket.Box - 1], (uint8_t *) &DataPacket, sizeof(DataPacket));
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
}
  }

}
