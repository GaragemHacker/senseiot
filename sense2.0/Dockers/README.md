
# CONTAINERS
-------------

[![Logo](https://raw.githubusercontent.com/edgarreis/senseiot/master/sense2.0/Dockers/docker.png)](https://github.com/Garagem-Hacker/senseiot/).


* **API** container;
* **Apache** Web;
* **Mosquitto** MQTT;
* **MSQL** Server;


* **API** container;
Criação uma API RestFUL em Python;

[REF - building-a-basic-restful-api-in-python](https://www.codementor.io/sagaragarwal94/building-a-basic-restful-api-in-python-58k02xsiq);

[REF - como-criar-uma-api-restful-agil-e-escalavel-com-python](http://www.programadorlivre.com/como-criar-uma-api-restful-agil-e-escalavel-com-python-parte-1/);

* **APACHE** container;

Criação do Servidor com utilização da pasta do host no container:

docker run -p 80:80 -p 443:443 -v /var/www/html/ -:/var/www/html/ -d eboraas/apache



* **MOSQUITTO** container;

Cria um container Mosquitto completo para configuração posterior

docker run -ti -p 1883:1883 -p 9001:9001 toke/mosquitto
docker exec -it dMosquitto /bin/bash

* **MSQL** Server;

Cria um container MySQL para Acesso Externo dos dados


docker run --name mysql-container -p 3306:3306 -v /home/zulu/datadir:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=mysql-server MYSQL_ROOT_HOST=% -d mysql/mysql-server:latest


------------------

* **Arvore dos Containers**


[![Logo](https://raw.githubusercontent.com/edgarreis/senseiot/master/sense2.0/Dockers/Tree-04-09-17.png)](https://github.com/Garagem-Hacker/senseiot/).
