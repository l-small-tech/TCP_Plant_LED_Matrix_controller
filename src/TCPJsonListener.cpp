#ifndef TCPJSONLISTENER
#define TCPJSONLISTENER
#endif
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

#include "RGBWStrip.cpp"
#include "secrets.h"

#define BUFF_SIZE 1024
#define PORT 8888

class TCPJsonListener {
private:
    WiFiServer* server;
    RGBWStrip* strip = new RGBWStrip();
    char* msgBuff = new char[BUFF_SIZE];

public:
    TCPJsonListener(int port) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(SSID, PASSWORD);

        // Wait for connection
        Serial.println("Connecting to Wifi");
        while (WiFi.status() != WL_CONNECTED) {   
            delay(1000);
            Serial.println(".");

            Serial.println(WiFi.localIP());

            server->begin();
        }

        server = new WiFiServer(port);
    }

    void loop() {
        WiFiClient client = server->available();
          if (client) {
            if(client.connected()) {
                Serial.println("Client Connected");
            }
            
            while(client.connected()){

                int i = 0;
                while(client.available()>0){
                    // read data from the connected client
                    msgBuff[i++] = client.read(); 
                }
            }
            Serial.println("Client disconnected, MSG:");
            Serial.print(msgBuff);
            Serial.println();

            StaticJsonDocument<1024> doc;
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

                strip->fillRGBW(r, g, b, w);
            }

            client.stop();
        }
    }
};