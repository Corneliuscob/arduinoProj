#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ArduinoOTA.h>
#include <ESP8266WebServer.h>

const char* ssid = "Family-2.4";
const char* password = "moxh-w1mq-1bpb";

ESP8266WebServer server(80);

const char* www_username = "toto";
const char* www_password = "password";
int pbutton = 2;
int ledpin = 3;
void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if(WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("WiFi Connect Failed! Rebooting...");
    delay(1000);
    ESP.restart();
  }
  ArduinoOTA.begin();

  server.on("/", [](){
    if(!server.authenticate(www_username, www_password))
      return server.requestAuthentication();
    server.send(200, "text/plain", "Login OK");
  });
  server.begin();

  Serial.print("Open http://");
  Serial.print(WiFi.localIP());
  Serial.println("/ in your browser to see it working");
  pinMode(ledpin,OUTPUT);
  pinMode(pbutton,INPUT);
}

void loop() {
  
  ArduinoOTA.handle();
  server.handleClient();
  int buttonState  = digitalRead(pbutton);
  if(buttonState == HIGH){
    digitalWrite(ledpin,HIGH);
  }
  else{
    digitalWrite(ledpin,LOW);
  }
  
}
