#include <WiFiEsp.h> // Biblioteca
#include <WiFiEspClient.h>
#include <WiFiEspUdp.h>
#include "SoftwareSerial.h"
#include <PubSubClient.h>

#define LED 13

SoftwareSerial Serial1(2,3);

char server[] = "mqtt.tago.io";
char ssid[] = "FIESC_IOT";            // network SSID (nome)
char pass[] = "7FuhM4@Io9";        // network senha
int status = WL_IDLE_STATUS;
char payload_char[] = "";

// função Callback definida
void callback(char* topic, byte* payload, unsigned int length);

WiFiEspClient espClient;
PubSubClient client(server, 1883, callback, espClient);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  WiFi.init(&Serial1);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while(true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");

  client.setServer(server, 1883);
  client.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length){
  payload[length] = '\0';
  String message = (char*)payload;
  Serial.println(message);
  if(message == "on") {
    digitalWrite(LED, HIGH);
    Serial.println("Led Ligado");
  }else{
    digitalWrite(LED, LOW);
    Serial.println("Led Desligado");
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  delay(100); //otherwise callback not work
}

void reconnect() {
  while(!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("Device_xxx","Default","xxx-xxx-xxx-xxx-xxx")) {
      Serial.println("connected");
      client.publish("esp32-publish","hello world");
      client.subscribe("test");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println("Try again later");
      delay(5000);
    }
}
}
