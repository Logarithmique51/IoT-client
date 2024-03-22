#ifndef MQTT_H
#define MQTT_H
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

struct TopicHandler {
    char* topic;
    std::function<void(byte*)> callback;
};


class Mqtt {
    public: 
        static void listen(IPAddress);
        static void on(char*,std::function<void(byte *)>);
        static std::vector<TopicHandler> topicListHandler;
        static void loop();
        static void ping();

     private: 
        static WiFiClient wifiClient;
        static PubSubClient client;
        static void callBackHandler(char *, byte *, unsigned int);
 
    };

#endif


