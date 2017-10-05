import sys
import paho.mqtt.client as mqtt
import sqlite3

broker = "echo1001.me"
port = 1883
keppAlive = 60
topic = 'dev/#'

#Tabela de Temperatura
def insertTemperature(message,topic):
    conn = sqlite3.connect('database.db')
    cursor = conn.cursor()

    cursor.execute("INSERT INTO temperature (temperature, topic) VALUES (?,?)", (message,topic))

    conn.commit()
    conn.close()

# Tabela de Umidade
def insertHumidity(message,topic):
    conn = sqlite3.connect('database.db')
    cursor = conn.cursor()

    cursor.execute("INSERT INTO humidity (humidity, topic) VALUES (?,?)", (message,topic))

    conn.commit()
    conn.close()

#Tabela de Corrente RMS
def insertCurrent(message,topic):
	conn = sqlite3.connect('database.db')
	cursor = conn.cursor()

	cursor.execute("INSERT INTO currentrms (current, topic) VALUES (?,?)", (message,topic))

	conn.commit()
	conn.close()

def on_connect(client, userdata, flags, rc):
    print("[STATUS] Conectado ao Broker. Resultado de conexao: "+str(rc))

    client.subscribe(topic)

def on_message(client, userdata, msg):

    message = str(msg.payload) # converte a mensagem recebida
    topic = str(msg.topic)

    print("[MSG RECEBIDA] Topico: "+msg.topic+" | Mensagem: "+ message)

    if msg.topic == 'dev/node1/temperature':

        insertTemperature(message,topic) # invoca o metodo de cadastro passando por parametro a temperatura recebida

    if msg.topic == 'dev/node1/humidity':

		insertHumidity(message,topic)

    if msg.topic == 'dev/node1/current':

		insertCurrent(message,topic)

    if msg.topic == 'dev/node2/current':

		insertCurrent(message,topic)

try:
    print("[STATUS] Inicializando MQTT...")

    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message

    client.connect(broker, port, keppAlive)
    client.loop_forever()

except KeyboardInterrupt:
    print "\nScript finalizado."
    sys.exit(0)
