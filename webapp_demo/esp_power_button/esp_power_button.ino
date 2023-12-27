#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>


#define PWR_OFF_TIME 5000
#define PUSH_TIME 400

#define PWR_PIN 13
#define RST_PIN 12
//this needs to be pin 16 to be input pull down
#define STATUS_PIN 16

//const char* ssid = "Family-2.4";
//const char* password = "m0xh-w1mq-1bpb";

const char* ssid = "testing";
const char* password = "password";

ESP8266WebServer server(WiFi.softAPIP(), 80);
//only if interested in getting esp's voltage
//ADC_MODE(ADC_VCC); 

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

void configureEndpoints() {
  server.on("/pcswitch/pwr_off_force/", [] () {
    do_powerOffForce();
    server.send(200, "text/plain", "PWR_OFF_FORCE, " + getStatusString());
  });

  server.on("/pcswitch/pwr_off/", [] () {
    do_powerOff();
    server.send(200, "text/plain", "PWR_OFF, " + getStatusString());
  });

  server.on("/pcswitch/pwr_on/", [] () {
    do_powerOn();
    server.send(200, "text/plain", "PWR_ON, " + getStatusString());
  });

  server.on("/pcswitch/reset/", [] () {
    do_reset();
    server.send(200, "text/plain", "RESET, " + getStatusString());
  });

  server.on("/pcswitch/status/", [] () {
    server.send(200, "text/plain", "STATUS, " + getStatusString());
  });

  server.onNotFound([] () {
    server.send(404, "text/plain", "Not Found");
  });
}

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  WiFi.mode(WIFI_AP);

  pinMode(STATUS_PIN, INPUT_PULLDOWN_16);
  // max connections is deprecated 
//if(WiFi.softAP(ssid, password, 3, true, 8))
  if(WiFi.softAP(ssid, password,3,false)){
    digitalWrite(LED_BUILTIN, LOW);
    delay(50);
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    failLoop();
  }

  configureEndpoints();
  server.begin();
}

void loop() {
  server.handleClient();
}
