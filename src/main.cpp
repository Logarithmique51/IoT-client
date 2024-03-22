#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Lifecycle.h>
#include <Flare.h>
#include <Mqtt.h>

const char* ssid = "Freebox44 2.4GHZ";
const char* password = "noel2022";

WiFiEventHandler gotIpEventHandler, disconnectedEventHandler;


void handleWiFiModeChange(const WiFiEventStationModeGotIP & evt){
  Flare::listen();
};

void handleDisconnected(const WiFiEventStationModeDisconnected & evt){
  Flare::stop();
};

void setup_wifi() {
  WiFi.persistent(true);
  WiFi.begin(ssid, password);
  gotIpEventHandler = WiFi.onStationModeGotIP(&handleWiFiModeChange);
  disconnectedEventHandler = WiFi.onStationModeDisconnected(&handleDisconnected);
}


void blink(){
  digitalWrite(D8,LOW);
  delay(200);
  digitalWrite(D8,HIGH);
  delay(200);
  digitalWrite(D8,LOW);
}
 
void readPacket(const char* packet,IPAddress senderIp){

  if(strcmp(packet, "admin") == 0) {
    Mqtt::listen(senderIp);
   }else{
    Serial.println("UNKNOW");
  }
}

void setup_flare(){
  Flare::onIncommingPacket(readPacket);
}

void setup_mqtt(){
  Mqtt::on("limitless/logic",[](byte* payload){
    Serial.println("reçu");
  });
}

void setup() {
  pinMode(D8,OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  setup_flare();
  setup_mqtt();
}


void loop() {
  Flare::loop();
  Mqtt::loop();
}
 
 
