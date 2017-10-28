

# API para Acesso dos dados do Banco de Dados
-------------------

Funcionalidades
---------------

* **create_table.py** Cria as Tabelas do SQLite

* **database.db** Database em SQLite que persiste os dados trocados entre o MQTT e a API 

* **subscriber.py** Descontinuado (~~Subscreve nos Tópicos MQTT e salva os dados no Banco de Dados~~)

* **publish_subscribe.py** Subscreve nos Tópicos MQTT e salva os dados no Banco de Dados, além de Publicar os dados novos no banco de dados que são recebidos pela API

* **rest_flask.py** Descontinuado (~~Coleta do banco os dados e repassa via método GET, Informações: Temperatura~~)

* **rest_flask2.py** Descontinuado (~~Coleta do banco os dados e repassa via método GET, Informações: Temperatura e Corrente~~)

* **restful_flask.py** API - Coleta do banco os dados e repassa via método GET, além de receber os dados vai método POST. Informações: Temperatura, Corrente, Comando (Separados via nós).


