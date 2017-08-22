// Code based on
// https://github.com/adafruit/DHT-sensor-library/blob/master/examples/DHTtester/DHTtester.ino
// https://gist.github.com/igrr/7f7e7973366fc01d6393
// https://github.com/iot-playground/Arduino/blob/master/ESP8266ArduinoIDE/DS18B20_temperature_sensor/DS18B20_temperature_sensor.ino

// esp8266 + dht22 + mqtt

#include "DHT.h"
#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const char* ssid = "";
const char* password = "";

char* topicHum = "dev/node1/humidity";
char* topicTemp = "dev/node1/temperature";

char* server = "";
char* hellotopic = "hello_topic";

#define DHTPIN 5            // what pin we're connected to
#define DHTTYPE DHT22       // DHT 22  (AM2302)
#define REPORT_INTERVAL 1   // in sec

String clientName;
DHT dht(DHTPIN, DHTTYPE, 15);
WiFiClient wifiClient;

PubSubClient client(server, 1883, wifiClient);

float oldH ;
float oldT ;

void setup() {
  Serial.begin(115200);
  Serial.println("DHTxx test!");
  delay(20);

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  clientName += "esp8266-";
  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += macToStr(mac);
  clientName += "-";
  clientName += String(micros() & 0xff, 16);

  Serial.print("Connecting to ");
  Serial.print(server);
  Serial.print(" as ");
  Serial.println(clientName);

  if (client.connect((char*) clientName.c_str())) {
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic is: ");
    Serial.println(topicHum);
    Serial.println(topicTemp);
    
    if (client.publish(hellotopic, "hello from ESP8266")) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }

  dht.begin();
  oldH = -1;
  oldT = -1;
}

void loop() {

  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float hi = dht.computeHeatIndex(f, h);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  
  String payloadHum = "";
  payloadHum += h;
  payloadHum += "";
  String payloadTem = "";
  payloadTem += t;
  payloadTem += "";
  
  if (t != oldT || h != oldH )
  {
    sendPayload(payloadHum, topicHum);
    sendPayload(payloadTem, topicTemp);
    oldT = t;
    oldH = h;
  }

  int cnt = REPORT_INTERVAL;

  while (cnt--)
    delay(1000);
}


void sendPayload(String payload, char* topic) {
  if (!client.connected()) {
    if (client.connect((char*) clientName.c_str())) {
      Serial.println("Connected to MQTT broker again");
      Serial.print("Topic is: ");
      Serial.println(topic);
    }
    else {
      Serial.println("MQTT connect failed");
      Serial.println("Will reset and try again...");
      abort();
    }
  }

  if (client.connected()) {
    Serial.print("Sending payload: ");
    Serial.println(payload);

    if (client.publish(topic, (char*) payload.c_str())) {
      Serial.println("Publish ok");
    }
    else {
      Serial.println("Publish failed");
    }
  }

}


void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}

String macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

