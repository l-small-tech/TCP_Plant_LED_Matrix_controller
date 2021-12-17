#include <ESP8266WiFi.h>

#include "secrets.h"

int port = 8888;  //Port number
WiFiServer server(port);

int count=0;
//=======================================================================
//                    Power on setup
//=======================================================================
void setup() {
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
//=======================================================================
//                    Loop
//=======================================================================

void loop() {
  WiFiClient client = server.available();
  
  if (client) {
    if(client.connected()) {
      Serial.println("Client Connected");
    }
    
    while(client.connected()){      
      while(client.available()>0){
        // read data from the connected client
        Serial.write(client.read()); 
      }
    }
    client.stop();
    Serial.println("Client disconnected");    
  }
}