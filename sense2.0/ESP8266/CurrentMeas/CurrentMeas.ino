
#include <PubSubClient.h>
#include <ESP8266WiFi.h>
#include "EmonLib.h"
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>


#define REPORT_INTERVAL 1           // in sec

// Wifi SSID e Password
const char* ssid = "";     
const char* password = "";  

// Tópicos MQTT
char* hellotopic = "hello_topic"; 
char* topic = "dev/node1/current";
char* server = "";
int port = 1883;

// MQTT Client
String clientName;
WiFiClient wifiClient;
PubSubClient client(server, port, wifiClient);

// Medição
double Irms;
double Vrms = 127.00;
int cnt;

EnergyMonitor emon1;


// Converte mac to string
String macToStr(const uint8_t* mac){
  
    String result;

  for (int i = 0; i < 6; ++i) {

      result += String(mac[i], 16);

      if (i < 5)
          result += ':';
    }
    
    return result;
}

// Configurações
void setup(){


  //Comunicação Serial
  Serial.begin(115200);
  Serial.println("Booting");

  Serial.println();
  Serial.print("Connecting to ")  ;
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Aguardando Conexão
  while (WiFi.status() != WL_CONNECTED) {
  
      delay(500);
      Serial.print(".");
  }

  // Conectado
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Cliente
  clientName += "esp8266-";
  uint8_t mac[6];
  WiFi.macAddress(mac);
  clientName += macToStr(mac);
  clientName += "-";
  clientName += String(micros() & 0xff, 16);

  // Conexão com o Servidor MQTT
  Serial.print("Connecting to ");
  Serial.print(server);
  Serial.print(" as ");
  Serial.println(clientName);

  if (client.connect((char*) clientName.c_str())) {
    
    Serial.println("Connected to MQTT broker");
    Serial.print("Topic is: ");
    Serial.println(topic);
          
  }else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }

  // Configuração do EMONCS
  Serial.print("Configuring EMONCS");
  
  // Sensor de Corrente (input pin, calibration)
  emon1.current(A0, 19);        

  delay(20);


  // Configuração do OTA  
  // Port defaults to 8266
  ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("Node1-Current");

  // No authentication by default
  ArduinoOTA.setPassword((const char *)"esp8266");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());




  
}


// Principal
void loop(){
    
    // Verificação OTA update
    ArduinoOTA.handle();

    // Medição de Corrente (Calculo Irms)
    Irms = emon1.calcIrms(1480)-0.03;
    
    // Conversão double to String
    String payload = String(Irms,DEC);

    // Envio do Pacote MQTT
    sendPayload(payload, topic);
    
    cnt = REPORT_INTERVAL;

    // Temporização 1s
    while (cnt--)
      delay(1000);
}


// Envia MQTT Payload
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


// Manipula Mensagem Recebidas
void callback(char* topic, byte* payload, unsigned int length) {

  
}



