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
  digitalWrite(D8,HIGH);
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
    digitalWrite(D8,LOW);
   }else{
    Serial.println("UNKNOW");
  }
}

void setup_flare(){
  Flare::onIncommingPacket(readPacket);
}

void setup_mqtt(){

  std::string topic_base = "maison/lampe/";
  std::string chip_id = std::to_string(system_get_chip_id());
  topic_base.append(chip_id);
  char* topic_base_buffer = new char[topic_base.length() + 1];
  strcpy(topic_base_buffer,topic_base.c_str());
  Mqtt::on(topic_base_buffer,[](byte* payload){
    blink();
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
  Lifecycle::loop();
}
 
 
