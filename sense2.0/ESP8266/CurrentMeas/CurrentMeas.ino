
#include <PubSubClient.h>           // MQTT Library
#include <ESP8266WiFi.h>            // WiFi Library
#include "EmonLib.h"             	  // Emon Library

#define REPORT_INTERVAL 1   		    // in sec

// Wifi SSID e Password
const char* ssid = "";
const char* password = "";

// Tópicos MQTT
char* hellotopic = "hello_topic"; 
char* topic = "/dev/node1/current";
char* server = "";
int port = 1883;

// MQTT Client
String clientName;
WiFiClient wifiClient;
PubSubClient client(server, port, wifiClient);

// Medição
double Irms;
double Vrms = 127.0;
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

  // Sensor de Corrente (input pin, calibration)
	emon1.current(A0, 19);      	

	delay(20);

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
          
  }
  else {
    Serial.println("MQTT connect failed");
    Serial.println("Will reset and try again...");
    abort();
  }
}


// Principal
void loop(){

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

