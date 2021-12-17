// https://arduino-esp8266.readthedocs.io/en/latest/esp8266wifi/client-class.html
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>

#include "secrets.h"
#include "RGBWStrip.cpp"

#define BUFF_SIZE 1024

int port = 8888;  //Port number
WiFiServer server(port);
RGBWStrip strip;


int count=0;
//=======================================================================
//                    Power on setup
//=======================================================================
void setup() {
  delay(100);
  Serial.begin(115200);
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(SSID, PASSWORD); //Connect to wifi
 
  // Wait for connection  
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {   
    delay(1000);
    Serial.println(".");
  }

  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {
  // strip.fillRGBW(0, 0, 0, 255);
  char msgBuff[BUFF_SIZE];
  for (int i = 0; i < BUFF_SIZE; i++) {
    msgBuff[i] = '\0';
  }

  WiFiClient client = server.available();
  
  if (client) {
    if(client.connected()) {
      Serial.println("Client Connected");
    }
    
    int i = 0;
    while(client.connected()){

      
      while(client.available()>0){
        // read data from the connected client
        msgBuff[i++] = client.read(); 
      }
    }
    Serial.println("Client disconnected, MSG:");
    Serial.print(msgBuff);
    Serial.println();

    StaticJsonDocument<BUFF_SIZE> doc;
    DeserializationError error = deserializeJson(doc, msgBuff);

    if (error) {

      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.f_str());
      return;
    }

    if (doc.containsKey("fill")) {

      int r = doc["fill"][0];
      int g = doc["fill"][1];
      int b = doc["fill"][2];
      int w = doc["fill"][3];

      Serial.printf("Filling strip with %d, %d, %d, %d\n", r, g, b, w);

      strip.fillRGBW(r, g, b, w);
    } else if (doc.containsKey("brightness")) {

      int brightness = doc["brightness"];
      Serial.printf("Setting brightness to %d\n", brightness);
      strip.setBrightness(brightness);
    } else {
      Serial.println("Unknown command");
    }

    client.stop();
  }
}