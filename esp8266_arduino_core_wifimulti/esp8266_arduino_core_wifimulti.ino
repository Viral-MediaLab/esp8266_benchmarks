/**
 * BasicHTTPClient.ino
 *
 *  Created on: 24.05.2015
 *
 */

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
unsigned int i = 0;

void setup() {
    i++;
    USE_SERIAL.begin(115200);
   // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(1000);
    }

    WiFiMulti.addAP("POGO24", "abcd5678");

}

void loop() {
    // wait for WiFi connection
    if((WiFiMulti.run() == WL_CONNECTED)) {

        HTTPClient http;
        unsigned long timestamp = millis();
        int success = 0;
        
//        USE_SERIAL.print("[HTTP] begin...\n");
        // configure traged server and url
        http.begin("www.adafruit.com", 80, "/testwifi/index.html"); //HTTP

//        USE_SERIAL.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
        if(httpCode) {
            // HTTP header has been send and Server response header has been handled
//            USE_SERIAL.printf("[HTTP] GET... code: %d\n", httpCode);

            // file found at server
            if(httpCode == 200) {
                String payload = http.getString();
//                USE_SERIAL.println(payload);
                success = payload.indexOf("CC3000")!=-1;
            }
        } else {
//            USE_SERIAL.print("[HTTP] GET... failed, no connection or no HTTP server\n");
        }
        USE_SERIAL.print(++i);  
        USE_SERIAL.print(",");
        USE_SERIAL.print(success);  
        USE_SERIAL.print(",");
        USE_SERIAL.println(millis()-timestamp);
    }

    delay(1000);
}

