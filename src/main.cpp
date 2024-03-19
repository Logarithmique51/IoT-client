#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network and MQTT broker
const char* ssid = "Freebox44 2.4GHZ";
const char* password = "noel2022";
const char* mqtt_server = "192.168.1.83";
const int mqtt_port = 1883; // Default MQTT port
const char* mqtt_topic = "your_topic";

WiFiClient espClient;
PubSubClient client(espClient);
//   WiFi.begin("Freebox44 2.4GHZ", "noel2022");

void setup_wifi() {
  delay(10);
  // Connect to Wi-Fi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  // Publish a message every 5 seconds
  client.publish(mqtt_topic, "Hello from ESP8266");
  delay(5000);
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

 
