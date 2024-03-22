#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Lifecycle.h>
#include <Flare.h>

const char* ssid = "Freebox44 2.4GHZ";
const char* password = "noel2022";

const char* mqtt_username = "LIMITLESSLOGIC";
const char* mqtt_password = "AZLIMIT51100";

const char* mqtt_server = "192.168.1.29";
const int mqtt_port = 1883;
const char* mqtt_topic = "limitlesslogic/ping";

WiFiClient espClient;
PubSubClient client(espClient);

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

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    std::string id = std::to_string(system_get_chip_id());
    std::string topic = "maison/lampe/" + id ;
    if (client.connect(id.c_str(),mqtt_username,mqtt_password)) {
      client.subscribe(topic.c_str()); // Subscribe to the topic
    } else {
      Serial.print(client.state());
      delay(5000);
    }
  }
}

void blink(){
  digitalWrite(D8,LOW);
  Serial.println("BLINK LOW");
  delay(200);
  digitalWrite(D8,HIGH);
  Serial.println("BLINK HIGH");
  delay(200);
  digitalWrite(D8,LOW);
  Serial.println("BLINK LOW");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message reçu sur le topic: ");
  Serial.println(topic);
  Serial.print("Payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  blink();
}

void pingBroker(){
  
  char output[128];
  JsonDocument descriptor;
  descriptor["mac"] = WiFi.macAddress();
  descriptor["ip"] = WiFi.localIP();
  descriptor["strenght"] = WiFi.RSSI();
  descriptor["type"] = "SHUTTER"; 
  descriptor["id"] = system_get_chip_id();
  serializeJson(descriptor, output);

  client.publish(mqtt_topic,output);

}

void test(){
  Serial.println("CALL EVERY 2 SEC");
}

void readPacket(const char* packet,IPAddress senderIp){
  if(strcmp(packet, "admin") == 0) {
    Serial.println("BRIDGE");
    Serial.println("From: "+senderIp.toString());
  }else{
    Serial.println("UNKNOW");
  }
}

void setup_flare(){
  Flare::onIncommingPacket(readPacket);
}

void setup() {
  pinMode(D8,OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  setup_flare();
  
  // client.setServer(mqtt_server, mqtt_port);
  // client.setCallback(callback);
  // blink();
  // Lifecycle::add(2000,&pingBroker);
  // WiFiEventHandler ok;


}



void loop() {
  // if (!client.connected()) {
  //   reconnect();
  // }
  // client.loop();
  // Lifecycle::loop();
  Flare::loop();
 
}

// #include <ESP8266WiFi.h>
// #include <WiFiUdp.h>
// #include <ArduinoJson.h>


// const char* ssid     = "<YOURSSID>";
// const char* password = "<YOURPASSWORD>";
// JsonDocument LimitlessId;



// void setup() 
// {
//   Serial.begin(9600);

//   Serial.println();
//   Serial.println();
//   Serial.print("Connecting to ");
//   Serial.println(ssid);

//   WiFi.mode(WIFI_STA);
//   WiFi.begin("Freebox44 2.4GHZ", "noel2022");

//   while (WiFi.status() != WL_CONNECTED) 
//   {
//     delay(500);
//     Serial.print(".");
//   }

//   Serial.println("");
//   Serial.println("WiFi connected");
//   Serial.println("IP address: ");
//   Serial.println(WiFi.localIP());
// }

// void loop()
// {
//   Serial.println("Setting Up To Receive");
//   WiFiUDP udp;
//   IPAddress address(239, 255, 255, 3);
//   udp.beginMulticast(WiFi.localIP(), address, 2013);

//   while (true)
//   {
//     int packetSize = udp.parsePacket();
//     if (packetSize > 0)
//     {
//       Serial.println(packetSize);
//       char buffer[packetSize];
//       udp.read(buffer, packetSize);
//       buffer[packetSize] = '\0';
//       Serial.println(buffer);

//       udp.beginPacket(udp.remoteIP(), udp.remotePort());
//       char output[128];

//       LimitlessId["mac"] = WiFi.macAddress();
//       LimitlessId["ip"] = WiFi.localIP();
//       LimitlessId["strenght"] = WiFi.RSSI();
//       LimitlessId["type"] = "SHUTTER"; 

//       serializeJson(LimitlessId, output);
//       Serial.println(output);
//       udp.write(output);
//       udp.endPacket();
//     }
//   }
// }

 
