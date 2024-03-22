#ifndef FLARE_H
#define FLARE_H
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

class Flare {
    public: 
        static void listen();
        static void loop();
        static void stop();
        static void onIncommingPacket(std::function<void(const char*,IPAddress)>);
        static std::function<void(const char*,IPAddress)> incomingPacketHandler;

    private: 
        static WiFiUDP server;
        static IPAddress multicast_ip;
        static unsigned int multicast_port;
    };

#endif
