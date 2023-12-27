/*
 * Copyright (c) 2015, Majenko Technologies
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * * Neither the name of Majenko Technologies nor the names of its
 *   contributors may be used to endorse or promote products derived from
 *   this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#define PWR_OFF_TIME 5000
#define PUSH_TIME 500

//#define PWR_PIN 13 // d7 opn nodemcu
#define PWR_PIN D7 // d7 opn nodemcu
#define RST_PIN 12 //d6 on nodemcu
//this needs to be pin 16 to be input pull down d0 on the nodemcu
#define STATUS_PIN D0


const char *ssid = "Family-2.4";
const char *password = "m0xh-w1mq-1bpb";

ESP8266WebServer server ( 80 );



const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
  <head>
    <meta http-equiv='refresh' content='60'/>
    <title>ESP8266 Demo</title>
    <style>
      body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }
    </style>
  </head>
  <body>
    <h1>Hello from ESP8266!</h1>
    <h2> <a href="/pwr_off_force/">Power Off (f)<a/> </h2>
    <h2> <a href="/pwr_off/">Power Off</a> </h2>
    <h2> <a href="/pwr_on/">Power On</a> </h2>
    <h2> <a href="/reset/">Reset</a> </h2>
    <h2> <a href="/status/">Status</a> </h2>
    <p>Uptime: %02d:%02d:%02d</p>
    <img src="/test.svg" />
    <h3>API Endpoints</h3>
    <ul>
      <li>/pwr_off_force/</li>
      <li>/pwr_off/</li>
      <li>/pwr_on/</li>
      <li>/reset/</li>
      <li>/status/</li>
    </ul>
  </body>
</html>
)=====";


void handleRoot() {
 
//   char temp[100];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;
  String s = MAIN_page;
//   snprintf ( temp, 1000,"", hr, min % 60, sec % 60  );
  server.send ( 200, "text/html", s );
 
}

void handleNotFound() {
  
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";

  for ( uint8_t i = 0; i < server.args(); i++ ) {
    message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
  }

  server.send ( 404, "text/plain", message );
  
}

void setup ( void ) {
  
  pinMode (PWR_PIN, INPUT);
  pinMode (RST_PIN, INPUT);
  pinMode(STATUS_PIN, INPUT_PULLUP);
  
//  digitalWrite(STATUS_PIN,LOW);
  Serial.begin ( 115200 );
  WiFi.begin ( ssid, password );
  Serial.println ( "" );

  // Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println ( "" );
  Serial.print ( "Connected to " );
  Serial.println ( ssid );
  Serial.print ( "IP address: " );
  Serial.println ( WiFi.localIP() );

  if ( MDNS.begin ( "esp8266" ) ) {
    Serial.println ( "MDNS responder started" );
  }

  server.on ( "/", handleRoot );
  server.on ( "/test.svg", drawGraph );
    configureEndpoints();
  server.onNotFound ( handleNotFound );
  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop ( void ) {
  server.handleClient();
}

void configureEndpoints() {
  server.on("/pwr_off_force/", [] () {
    do_powerOffForce();
    
    server.sendHeader("Location", "/",true);
//    server.send(200, "text/plain", "PWR_OFF_FORCE, " + getStatusString());
    server.send(302, "text/plane",""); 
  });

  server.on("/pwr_off/", [] () {
    do_powerOff();
    server.send(200, "text/plain", "PWR_OFF, " + getStatusString());
  });

  server.on("/pwr_on/", [] () {
    do_powerOn();
    server.send(200, "text/plain", "PWR_ON, " + getStatusString());
  });

  server.on("/reset/", [] () {
    do_reset();
    server.send(200, "text/plain", "RESET, " + getStatusString());
  });

  server.on("/status/", [] () {
    server.send(200, "text/plain", "STATUS, " + getStatusString());
  });

  server.onNotFound([] () {
    server.send(404, "text/plain", "Not Found");
  });
}

void drawGraph() {
  String out = "";
  char temp[100];
  out += "<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\" width=\"400\" height=\"150\">\n";
  out += "<rect width=\"400\" height=\"150\" fill=\"rgb(250, 230, 210)\" stroke-width=\"1\" stroke=\"rgb(0, 0, 0)\" />\n";
  out += "<g stroke=\"black\">\n";
  int y = rand() % 130;
  for (int x = 10; x < 390; x+= 10) {
    int y2 = rand() % 130;
    sprintf(temp, "<line x1=\"%d\" y1=\"%d\" x2=\"%d\" y2=\"%d\" stroke-width=\"1\" />\n", x, 140 - y, x + 10, 140 - y2);
    out += temp;
    y = y2;
  }
  out += "</g>\n</svg>\n";

  server.send ( 200, "image/svg+xml", out);
}

void failLoop() {
  while(true) {
    digitalWrite(LED_BUILTIN, LOW);
    delay(100);                       
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
  }
}


void togglePin(int pin, int ms) {
  digitalWrite(pin, LOW);
  pinMode(pin, OUTPUT);
  delay(ms);
  pinMode(pin, INPUT);
}
bool isPoweredOn() {
  return digitalRead(STATUS_PIN) == HIGH;
}
String getStatusString() {
  return String(isPoweredOn() ? "ON" : "OFF") + ", " + 
         WiFi.softAPIP().toString(); 
// add to string if want esp voltage
//         ESP.getVcc()/1024.00f + "V";
}


void do_powerOffForce() {
  digitalWrite(LED_BUILTIN, LOW);
  togglePin(PWR_PIN, PWR_OFF_TIME);
  digitalWrite(LED_BUILTIN, HIGH);
}


void do_powerOn() {
  if(!isPoweredOn()) {
    digitalWrite(LED_BUILTIN, LOW);
    togglePin(PWR_PIN, PUSH_TIME);
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    server.send(409, "text/plain", "ERR, " + getStatusString());
  }
}


void do_powerOff() {
  if(isPoweredOn()) {
    digitalWrite(LED_BUILTIN, LOW);
    togglePin(PWR_PIN, PUSH_TIME);
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    server.send(409, "text/plain", "ERR, " + getStatusString());
  }
}
void do_reset() {
  if(isPoweredOn()) {
    digitalWrite(LED_BUILTIN, LOW);
    togglePin(RST_PIN, PUSH_TIME);
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    server.send(409, "text/plain", "ERR, " + getStatusString());
  }
}
