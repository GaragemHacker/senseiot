#apt-get install python-mysqldb
import MySQLdb
import random
import time
import datetime
import paho.mqtt.client as mqtt

broker = "echo1001.me"
port = 1883
keppAlive = 60
topic = 'dev/#'

def insertTemperature(message):

	conn = MySQLdb.connect("echo1001.me","root","mysql-server","mydata")
	c = conn.cursor()

	id = random.randrange(1000, 10000)
	temperature = random.randrange(20, 30)
	now = datetime.datetime.now()

	c.execute("INSERT INTO table_test (id, temperature, timestamp) VALUES (%s,%s,%s)",
		(id, message, now))

	conn.commit()

	#c.execute("SELECT * FROM table_test")

	#rows = c.fetchall()

	#for eachRow in rows:
	#	print eachRow

	#c.close()


def on_connect(client, userdata, flags, rc):

	print("[STATUS] Conectado ao Broker. Resultado de conexao: "+str(rc))
	client.subscribe(topic)

def on_message(client, userdata, msg):

    	message = str(msg.payload) # converte a mensagem recebida
    	print("[MSG RECEBIDA] Topico: "+msg.topic+" | Mensagem: "+ message)

    	if msg.topic == 'dev/node1/temperature':

        	insertTemperature(message) # invoca o metodo de cadastro passando por parametro a temperatura recebida

	#if msg.topic == 'dev/node1/humidity':

        	#insertHumidity(message)

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

