/*

 It connects to an MQTT server then:
  - on 0 switches off relay
  - on 1 switches on relay
  - on 2 switches the state of the relay

  - sends 0 on off relay
  - sends 1 on on relay

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 The current state is stored in EEPROM and restored on bootup

*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Bounce2.h>
#include <EEPROM.h>
#include <Stream.h>
#include "string.h"
#define ESP8266_LED1 16
#define ESP8266_LED2 4    

const char* ssid          = ""
const char* password      = ""
const char* mqtt_server   = ""
int port_server           = 1883;
const char* topicButton2  = "dev/node1/led3";
const char* topicButton3  = "dev/node1/led4";
//const char* outTopic    = "clients/cmd";
const char* outTopic      = "clients2";

char ButtonChar[5];
char PayloadChar[5];
char clearChar[5];

String AnalogValue;
String TempValue;
String HumValue;
String Button;
String PayloadString;

WiFiClient espClient;
PubSubClient client(espClient);

long lastMsg = 0;
char msg[50];
int value = 0;
long now = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
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

void callback(char* topic, byte* payload, unsigned int length) {

  // Clear PayloadChar
  //PayloadChar = ";
  memcpy(PayloadChar, clearChar, 5);
  
  // Convert payload to String
  PayloadString = String((char*)payload);
  // Convert String to CharArray
  PayloadString.toCharArray(PayloadChar, 5);

  // LED 1
  if (strcmp(topic,topicButton2) == 0) {
      //Serial.print("1");

      Serial.println(PayloadChar); 

      if ( strcmp(PayloadChar,"0led") == 0){

        Serial.println(PayloadChar); 
        digitalWrite(ESP8266_LED1, LOW);
         
      }else if ( strcmp(PayloadChar,"1led") == 0){

        Serial.println(PayloadChar); 
        digitalWrite(ESP8266_LED1, HIGH);
        
      }
  }

  // LED 2
  if (strcmp(topic,topicButton3) == 0) {
      //Serial.print("1");

      Serial.println(PayloadChar); 

      if ( strcmp(PayloadChar,"0led") == 0){

        Serial.println(PayloadChar); 
        digitalWrite(ESP8266_LED2, LOW);
         
      }else if ( strcmp(PayloadChar,"1led") == 0){

        Serial.println(PayloadChar); 
        digitalWrite(ESP8266_LED2, HIGH);
        
      }
  }

  yield();
  
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client2")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(outTopic, "Node2 Connected");
      // ... and resubscribe
      client.subscribe(topicButton2);
      client.subscribe(topicButton3);
      
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
     }
   }
 }


//void establishContact() {
//  while (Serial.available() <= 0) {
//    delay(300);
//  }
//}

void setup() {

  //Config Pin
  pinMode(ESP8266_LED1, OUTPUT);
  pinMode(ESP8266_LED2, OUTPUT);
  
  Serial.begin(115200);
  //establishContact();  // send a byte to establish contact until receiver responds
  setup_wifi();        // Connect to wifi 
  client.setServer(mqtt_server, port_server);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
    yield();
  }
  
  client.loop();

}


