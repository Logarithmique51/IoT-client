#include "Flare.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Lifecycle.h>

IPAddress Flare::multicast_ip = IPAddress(239, 255, 255, 3);
unsigned int Flare::multicast_port = 1998;
WiFiUDP Flare::server;
std::function<void(const char *,IPAddress)> Flare::incomingPacketHandler = nullptr;

void Flare::onIncommingPacket(std::function<void(const char*,IPAddress)> handler)
{
    incomingPacketHandler = handler;
}

void Flare::loop()
{
    int packetSize = server.parsePacket();
    if (packetSize > 0)
    {
        char buffer[packetSize];
        server.read(buffer, packetSize);
        buffer[packetSize] = '\0';
        if (incomingPacketHandler)
        {
            incomingPacketHandler(buffer,server.remoteIP());
        }
    }
}

void Flare::listen()
{
    server.beginMulticast(WiFi.localIP(), multicast_ip, multicast_port);
    Serial.println("Server multicast listen to:"+WiFi.localIP().toString());
    digitalWrite(D8,HIGH);
    // Lifecycle::add(700,)
}

void Flare::stop()
{
    server.stop();
}