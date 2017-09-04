
# CONTAINERS
-------------
* **Mosquitto** MQTT;
* **Apache** Web;

* MOSQUITTO container;

Cria conteiner Mosquitto completo para configuração posterior

docker run -ti -p 1883:1883 -p 9001:9001 toke/mosquitto
docker exec -it dMosquitto /bin/bash

* APACHE conteiner;

Criação do Servidor com utilização da pasta do host no container:

docker run -p 80:80 -p 443:443 -v /var/www/html/ -:/var/www/html/ -d eboraas/apache


------------------

* **Arvore dos Containers**


[![Logo](https://raw.githubusercontent.com/edgarreis/senseiot/master/sense2.0/Dockers/Tree-04-09-17.png)](https://github.com/Garagem-Hacker/senseiot/).
