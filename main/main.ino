#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <Adafruit_MLX90614.h>

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
  server.send(200, "text/html", "<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=0\"><title>Body Temperature</title><style>body{display:flex;height:100vh;margin:0;background:#3d3d44;font-family:\"Jaldi\",sans-serif;font-size:14px;color:white}#wrapper{margin:auto;display:flex;flex-direction:column;align-items:center}p{-webkit-user-select:none;-moz-user-select:none;-ms-user-select:none;user-select:none}#info{opacity:.2;margin:0;text-align:center}#termometer{width:40px;background:#38383f;height:500px;position:relative;border:9px solid #2a2a2e;border-radius:40px;z-index:1;margin-bottom:50px}#termometer:before,#termometer:after{position:absolute;content:\"\";border-radius:50%}#termometer:before{width:100%;height:40px;bottom:9px;background:#38383f;z-index:-1}#termometer:after{transform:translateX(-50%);width:65px;height:65px;background-color:#3dcadf;bottom:-55px;border:9px solid #2a2a2e;z-index:-3;left:50%}#termometer #graduations{height:59%;top:20%;width:50%}#termometer #graduations,#termometer #graduations:before{position:absolute;border-top:2px solid rgba(0,0,0,0.5);border-bottom:2px solid rgba(0,0,0,0.5)}#termometer #graduations:before{content:\"\";height:34%;width:100%;top:32%}#termometer #temperature{bottom:0;background:linear-gradient(#f17a65,#3dcadf) no-repeat bottom;width:100%;border-radius:20px;background-size:100% 500px;transition:all .2s ease-in-out}#termometer #temperature,#termometer #temperature:before,#termometer #temperature:after{position:absolute}#termometer #temperature:before{content:attr(data-value);background:rgba(0,0,0,0.7);color:white;z-index:2;padding:5px 10px;border-radius:5px;font-size:1em;line-height:1;transform:translateY(50%);left:calc(100%+1em / 1.5);top:calc(-1em+5px - 5px * 2)}#termometer #temperature:after{content:\"\";border-top:.4545454545em solid transparent;border-bottom:.4545454545em solid transparent;border-right:.6666666667em solid rgba(0,0,0,0.7);left:100%;top:calc(-1em / 2.2+5px)}#playground{font-size:1.1em}#playground #range{display:flex}#playground #range input[type=text]{width:2em;background:transparent;border:0;color:inherit;font:inherit;margin:0 5px;padding:0 5px;border-bottom:2px solid transparent;transition:all .2s ease-in-out}#playground #range input[type=text]:focus{border-color:#3dcadf;outline:0}#playground #range input[type=text]:first-child{text-align:right}#playground #unit{width:100%;margin:0;text-align:center}#playground #unit:hover{cursor:pointer}input[type=range]{-webkit-appearance:none;background:transparent;margin:5.5px 0;width:100%}input[type=range]::-moz-focus-outer{border:0}input[type=range]:hover{cursor:pointer}input[type=range]:focus{outline:0}input[type=range]:focus::-webkit-slider-runnable-track{background:#313137;border-color:#313137}input[type=range]:focus::-ms-fill-lower{background:#2a2a2e}input[type=range]:focus::-ms-fill-upper{background:#313137;border-color:#313137}input[type=range]::-webkit-slider-runnable-track{cursor:default;height:10px;transition:all .2s ease;width:100%;cursor:pointer;transition:all .2s ease-in-out;box-shadow:1px 1px 1px transparent,0 0 1px rgba(13,13,13,0);background:#2a2a2e;border:2px solid #2a2a2e;border-radius:5px}input[type=range]::-webkit-slider-thumb{box-shadow:4px 4px 4px transparent,0 0 4px rgba(13,13,13,0);background:#3dcadf;border:0 solid #3d3d44;border-radius:12px;cursor:pointer;height:11px;width:18px;-webkit-appearance:none;margin-top:-2.5px}input[type=range]::-moz-range-track{box-shadow:1px 1px 1px transparent,0 0 1px rgba(13,13,13,0);cursor:default;height:10px;transition:all .2s ease;width:100%;cursor:pointer;transition:all .2s ease-in-out;background:#2a2a2e;border:2px solid #2a2a2e;border-radius:5px;height:5px}input[type=range]::-moz-range-thumb{box-shadow:4px 4px 4px transparent,0 0 4px rgba(13,13,13,0);background:#3dcadf;border:0 solid #3d3d44;border-radius:12px;cursor:pointer;height:7px;width:14px}input[type=range]::-ms-track{cursor:default;height:10px;transition:all .2s ease;width:100%;cursor:pointer;transition:all .2s ease-in-out;background:transparent;border-color:transparent;border-width:5.5px 0;color:transparent}input[type=range]::-ms-fill-lower{box-shadow:1px 1px 1px transparent,0 0 1px rgba(13,13,13,0);background:#222226;border:2px solid #2a2a2e;border-radius:10px}input[type=range]::-ms-fill-upper{box-shadow:1px 1px 1px transparent,0 0 1px rgba(13,13,13,0);background:#2a2a2e;border:2px solid #2a2a2e;border-radius:10px}input[type=range]::-ms-thumb{box-shadow:4px 4px 4px transparent,0 0 4px rgba(13,13,13,0);background:#3dcadf;border:0 solid #3d3d44;border-radius:12px;cursor:pointer;height:7px;width:14px;margin-top:2.5px}input[type=range]:disabled::-webkit-slider-thumb{cursor:not-allowed}input[type=range]:disabled::-moz-range-thumb{cursor:not-allowed}input[type=range]:disabled::-ms-thumb{cursor:not-allowed}input[type=range]:disabled::-webkit-slider-runnable-track{cursor:not-allowed}input[type=range]:disabled::-ms-fill-lower{cursor:not-allowed}input[type=range]:disabled::-ms-fill-upper{cursor:not-allowed}</style></head><body><div id=\"wrapper\"><div id=\"termometer\"><div id=\"temperature\" data-value=\"0°C\"></div><div id=\"graduations\"></div></div><div id=\"playground\"><div id=\"range\"><input id=\"minTemp\" type=\"text\" value=\"35°C\"><input type=\"range\" min=\"35\" max=\"42\" step=\"0.1\" value=\"37.5\" oninput=\"showSetTemperature()\" onchange=\"loadSetTemperature()\"><input id=\"maxTemp\" type=\"text\" value=\"42°C\"></div><p id=\"unit\">设定温度：<span id=\"setTemp\">37.5</span>°C</p></div></div><script>const units={Celcius:\"°C\",Fahrenheit:\"°F\"};const config={minTemp:35,maxTemp:42,unit:\"Celcius\"};const temperature=document.getElementById(\"temperature\");function setTemperature(tempvalue){if(tempvalue<=config.minTemp){temperature.dataset.value=tempvalue+units[config.unit];tempvalue=config.minTemp;temperature.style.height=(tempvalue-config.minTemp)/(config.maxTemp-config.minTemp)*100+\"%\"}else{if(tempvalue>=config.maxTemp){temperature.dataset.value=tempvalue+units[config.unit];tempvalue=config.maxTemp;temperature.style.height=(tempvalue-config.minTemp)/(config.maxTemp-config.minTemp)*100+\"%\"}else{temperature.dataset.value=tempvalue+units[config.unit];temperature.style.height=(tempvalue-config.minTemp)/(config.maxTemp-config.minTemp)*100+\"%\"}}}function updateTemperature(){var xmlhttp;if(window.XMLHttpRequest){xmlhttp=new XMLHttpRequest()}else{xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\")}xmlhttp.onreadystatechange=function(){if(xmlhttp.readyState==4&&xmlhttp.status==200){setTemperature(xmlhttp.responseText)}};xmlhttp.open(\"GET\",\"/status\",true);xmlhttp.send()}function showSetTemperature(){document.querySelector(\"#setTemp\").innerText=document.querySelector(\"#range > input[type=range]:nth-child(2)\").value}function loadSetTemperature(){var data=new FormData();data.append(\"temp\",document.querySelector(\"#range > input[type=range]:nth-child(2)\").value);var xmlhttp;if(window.XMLHttpRequest){xmlhttp=new XMLHttpRequest()}else{xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\")}xmlhttp.open(\"POST\",\"/setTemp\");xmlhttp.send(data)}function getSetTemperature(){var xmlhttp;if(window.XMLHttpRequest){xmlhttp=new XMLHttpRequest()}else{xmlhttp=new ActiveXObject(\"Microsoft.XMLHTTP\")}xmlhttp.onreadystatechange=function(){if(xmlhttp.readyState==4&&xmlhttp.status==200){document.querySelector(\"#range > input[type=range]:nth-child(2)\").value=xmlhttp.responseText;document.querySelector(\"#setTemp\").innerText=xmlhttp.responseText}};xmlhttp.open(\"GET\",\"/getTemp\",true);xmlhttp.send()}window.onload=function(){getSetTemperature();setInterval(\"updateTemperature()\",1000)};</script></body></html>");
}

void handleHistory() {
  server.send(200, "text/html", "");
}

void handleStatus() {
  while(String(mlx.readObjectTempC()+3.5)=="nan");
  server.send(200, "text/plain", String(mlx.readObjectTempC()+3.5));
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
  if (!mlx.begin()) {
    while (1);
  };
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
  if(mlx.readObjectTempC()+3.5 >= setTemperature){
    digitalWrite(D8, HIGH);
  }else if(mlx.readObjectTempC()+3.5 < setTemperature){
    digitalWrite(D8, LOW);
  }
  server.handleClient();
  delay(100);
}
