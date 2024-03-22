#include <Mqtt.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Lifecycle.h>

WiFiClient Mqtt::wifiClient;
PubSubClient Mqtt::client;
std::vector<TopicHandler> Mqtt::topicListHandler;

void Mqtt::ping(){
  
  char output[128];
  JsonDocument descriptor;
  descriptor["mac"] = WiFi.macAddress();
  descriptor["ip"] = WiFi.localIP();
  descriptor["strenght"] = WiFi.RSSI();
  descriptor["type"] = "SHUTTER"; 
  descriptor["id"] = system_get_chip_id();
  serializeJson(descriptor, output);
  client.publish("limitlesslogic/ping",output);
 }

void Mqtt::on(const char* topic,std::function<void(byte* payload)> callback){
    topicListHandler.push_back({topic,callback});
}

void Mqtt::callBackHandler(char* topic, byte* payload, unsigned int length){
    for(TopicHandler &topicHandler : topicListHandler){
        if(strcmp(topic,topicHandler.topic) == 0){
            topicHandler.callback(payload);        
        }
    }
}

void Mqtt::listen(IPAddress ipBroker){
    client.setClient(wifiClient);
    client.setServer(ipBroker,1883);
    client.setCallback(callBackHandler);
    std::string id = std::to_string(system_get_chip_id());

    const bool isSuccess = client.connect(id.c_str(),"LIMITLESSLOGIC","AZLIMIT51100");
    if(isSuccess){
        Lifecycle::add(2000,ping);
        for(TopicHandler &topicHandler : topicListHandler){
            const bool isSuccessSubscribe = client.subscribe(topicHandler.topic);
            Serial.print(topicHandler.topic);
            Serial.print(" => ");
            Serial.println(isSuccessSubscribe);
        }
    }

}

void Mqtt::loop(){
    if(client.connected()){
        client.loop();
    }
    if(client.state() == MQTT_CONNECTION_LOST){
        Serial.println("BROKEN");
    }
}

