#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_MLX90614.h>
#include <FS.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#ifndef APSSID
#define APSSID "BT"
#define APPSK  ""
#endif

double setTemperature = 38.5;

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

void handleRoot() {
  File file = SPIFFS.open("/index.html","r");
  server.streamFile(file, "text/html");
  file.close();
}

void handleHistory() {
  File file = SPIFFS.open("/history.html","r");
  server.streamFile(file, "text/html");
  file.close();
}

void handleStatus() {
//  while(String(mlx.readObjectTempC()+3.5)=="nan");
//  server.send(200, "text/plain", String(mlx.readObjectTempC()+3.5));
  server.send(200, "text/plain", String(36.5));
}

void handleGetTemp() {
  server.send(200, "text/plain", String(setTemperature));
}

void handleSetTemp() {
  if (server.method() != HTTP_POST) {
    server.send(405, "text/plain", "Method Not Allowed");
  } else {
    for (uint8_t i = 0; i < server.args(); i++) {
      if (server.argName(i)=="temp"){
        setTemperature = server.arg(i).toFloat();
      } else if (server.argName(i)=="save"){
        
      }
    }
    server.send(200, "text/plain", String(setTemperature));
  }
}

void setup() {
  SPIFFS.begin();
//  if (!mlx.begin()) {
//    while (1);
//  };
  delay(1000);
  pinMode(D8, OUTPUT);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();

  server.on("/", handleRoot);
  server.on("/history", handleHistory);
  server.on("/status", handleStatus);
  server.on("/getTemp", handleGetTemp);
  server.on("/setTemp", handleSetTemp);
  server.begin();
}

void loop() {
//  if(mlx.readObjectTempC()+3.5 >= setTemperature){
//    digitalWrite(D8, HIGH);
//  }else if(mlx.readObjectTempC()+3.5 < setTemperature){
//    digitalWrite(D8, LOW);
//  }
  server.handleClient();
  delay(100);
}
