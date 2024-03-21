#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const char* ssid = "Freebox44 2.4GHZ";
const char* password = "noel2022";

const char* mqtt_username = "LIMITLESSLOGIC";
const char* mqtt_password = "AZLIMIT51100";

const char* mqtt_server = "192.168.1.29";
const int mqtt_port = 1883;
const char* mqtt_topic = "limitlesslogic/ping";

unsigned long LastMeasureTime = 0;
unsigned long Interval = 5000; //Normally set from config stored in EEPROM


WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("ATTEMPT CONNECT");
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    std::string id = std::to_string(system_get_chip_id());
    std::string topic = "maison/lampe/" + id ;
    if (client.connect(id.c_str(),mqtt_username,mqtt_password)) {
      client.subscribe(topic.c_str()); // Subscribe to the topic
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }

}

void blink(){
  digitalWrite(LED_BUILTIN,LOW);
  Serial.println("BLINK LOW");
  delay(200);
  digitalWrite(LED_BUILTIN,HIGH);
  Serial.println("BLINK HIGH");
  delay(200);
  digitalWrite(LED_BUILTIN,LOW);
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



void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
  blink();
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  unsigned long CurrentTime = millis();
  
  if ((CurrentTime - LastMeasureTime) >= Interval)
  {
    pingBroker(); 
    LastMeasureTime = CurrentTime;
  }
  // delay(700);
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

 
