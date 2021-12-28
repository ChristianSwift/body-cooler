#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Arduino_JSON.h>
#include <Adafruit_MLX90614.h>
#include <FS.h>
#include "operateFile.h"

//Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#ifndef APSSID
#define APSSID "BT"
#define APPSK  ""
#endif

double setTemperature = 38.5;
int count = 0;

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

void handleHistoryData() {
  File file = SPIFFS.open("/record","r");
  server.streamFile(file, "text/json");
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
      }
    }
    server.send(200, "text/plain", String(setTemperature));
  }
}

void setup() {
  Serial.begin(115200);
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
  server.on("/historydata", handleHistoryData);
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
  if(count == 50){
    Serial.println("Record begin");
    JSONVar recordList = JSON.parse(fileRead("/record"));
    Serial.println(recordList);
    if(recordList.length()>=10){
      for(int i=0;i<recordList.length();i++){
        if(i<recordList.length()-1){
          recordList[i]=(double)recordList[i+1];
        }else{
          recordList[i]=37.5;
//          recordList[i]=mlx.readObjectTempC()+3.5;
        }
      }
    }else{
      recordList[recordList.length()]=36.5;
//      recordList[recordList.length()]=mlx.readObjectTempC()+3.5;
    }
    fileWrite("/record",JSON.stringify(recordList)+"\n");
    count = 0;
  }
  server.handleClient();
  delay(100);
  count++;
}
