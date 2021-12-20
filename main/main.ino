#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_MLX90614.h>

Adafruit_MLX90614 mlx = Adafruit_MLX90614();

#ifndef APSSID
#define APSSID "BT"
#define APPSK  ""
#endif

const char *ssid = APSSID;
const char *password = APPSK;

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "Hello");
}

void handleStatus() {
  while(String(mlx.readObjectTempC()+3.5)=="nan");
  server.send(200, "text/plain", String(mlx.readObjectTempC()+3.5));
}

void setup() {
  if (!mlx.begin()) {
    while (1);
  };
  delay(1000);
  pinMode(D8, OUTPUT);
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();

  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.begin();
}

void loop() {
  server.handleClient();
}
