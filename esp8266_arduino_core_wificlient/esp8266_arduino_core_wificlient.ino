/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <ESP8266WiFi.h>

const char* ssid     = "POGO24";
const char* password = "abcd5678";

const char* host = "www.adafruit.com";

unsigned long timestamp;

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  delay(5000);
  ++value;
  timestamp=millis();
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {  
    Serial.print(value);
    Serial.print(",");
    Serial.print(0);
    Serial.print(",");
    Serial.println(millis()-timestamp);
    return;
  }
  
  // We now create a URI for the request
  String url = "/testwifi/index.html";
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(100); //original was 10ms but too often that was not enough to get a response
  
  // Read all the lines of the reply from server and print them to Serial
  int numOfChars = 0;
  while(client.available()){
    String line = client.readStringUntil('\r');
    numOfChars+=line.length();
  }

  Serial.print(value);
  Serial.print(",");
  Serial.print(numOfChars>0);
  Serial.print(",");
  Serial.println(millis()-timestamp);
}

