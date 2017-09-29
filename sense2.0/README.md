
Senseiot 2.0
============
Nova Versão de Sense IoT


[![Logo](https://raw.githubusercontent.com/GaragemHacker/senseiot/master/sense1.0/images/sense_iot2_128x128.png)](https://github.com/Garagem-Hacker/senseiot/).

Sense IoT é um projeto para pesquisa e desenvolvimento de uma rede de sensores sem fio.


Hardware
--------

* **ht05**: Sensor de Umidade e Temperatura;
* **sct**: Sensor de Corrente Elétrica;
* **esp8266**: Nó IoT;
* **ASQ/Ctrl-Board**: Aquisição de corrente e Comandos elétricos.


Software
--------
* **BrokerMQTT**: Servidor IoT MQTT
* **Flask**: Microframework para aplicação Web
* **Docker**: Docker Containers
* **Crypto**: Rotinas para Criptografar/Decriptografar dados

Estrutura MQTT
--------------
Regra de comunicação dos tópicos MQTT para os dispositivos

/*		   MQTT Topic Struct

 	    '/dev/node1/current2'
		|    |     |
		|    |     |
  	device(dev)  |    variable to sub/pub 
  		or   | (led1..n,temp1...n,button1...n)
 		     |
	command(cmd) |
 		     |
	IoT device node (node1...n)

*/

 Em resumo:
* Classe dispositivos(dev) ou comandos (cmd)
* Nó IoT em Questão (node1,..n)
* Variavel de Controle/Comandos (led1, button2, temp1, ...)


Dockers Containers
------------------
* **dMosquitto**: Mosquitto MQTT
* **dFlask**: Servico Backend
...


Referências 
-----------

 * [ESP8266](https://github.com/Garagem-Hacker/senseiot/wiki) - Utilização do ESP8266;
 * [ESP8266-&-MQTT](https://github.com/edgarreis/ESP8266-MQTT-Arduino) - Utilização do MQTT no ESP8266;
 * [MQTT-Python-Web](https://douglaszuqueto.com/artigos/integrando-a-aplicacao-web-com-banco-de-dados) - Integrando a aplicação MQTT à web com banco de dados;
 
