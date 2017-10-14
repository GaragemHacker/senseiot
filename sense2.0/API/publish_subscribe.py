import sys
import paho.mqtt.client as mqtt
import sqlite3
import time
import colorama
import random
from colorama import Fore, Back, Style

broker = "echo1001.me"
port = 1883
keppAlive = 60
topic = 'dev/#'
mqtt_send_topic = "cmd/node1/rele1"
mqtt_send_msg =''
mqtt_send_msg_prev =''

# Insere na Tabela de Temperatura
def insertTemperature(message,topic):

    # Acesso ao Banco de Dados
    conn = sqlite3.connect('database.db')
    cursor = conn.cursor()
    cursor.execute("INSERT INTO temperature (temperature, topic) VALUES (?,?)", (message,topic))
    conn.commit()
    conn.close()

# Insere na Tabela de Umidade
def insertHumidity(message,topic):

    # Acesso ao Banco de Dados
    conn = sqlite3.connect('database.db')
    cursor = conn.cursor()
    cursor.execute("INSERT INTO humidity (humidity, topic) VALUES (?,?)", (message,topic))
    conn.commit()
    conn.close()

# Insere na Tabela de Corrente RMS
def insertCurrent(message,topic):

    # Acesso ao Banco de Dados
    conn = sqlite3.connect('database.db')
    cursor = conn.cursor()
    cursor.execute("INSERT INTO currentrms (current, topic) VALUES (?,?)", (message,topic))
    conn.commit()
    conn.close()

# Define on_connect event Handler
def on_connect(client, userdata, flags, rc):

    print("["+ Fore.RED + "STATUS"+ Style.RESET_ALL +"] Conectado ao Broker. Resultado de conexao: "+str(rc))
    client.subscribe(topic)

# Define on_subscribe event Handler
def on_subscribe(mosq, obj, mid, granted_qos):

    print("["+ Fore.RED + "STATUS"+ Style.RESET_ALL +"] Inscrito nos topicos MQTT")

# Define on_message event Handler
def on_message(client, userdata, msg):

    # Convert to String
    message = str(msg.payload) 
    topic = str(msg.topic)

    print("["+ Fore.GREEN + "MSG RECEBIDA"+ Style.RESET_ALL +"] Topico: "+msg.topic+" | Mensagem: "+ message)

    # Topicos MQTT para Cadastro no Banco de Dados
    if msg.topic == 'dev/node1/temperature':

        insertTemperature(message,topic)

    if msg.topic == 'dev/node1/humidity':

        insertHumidity(message,topic)

    if msg.topic == 'dev/node1/current':

        insertCurrent(message,topic)

    if msg.topic == 'dev/node2/current':

        insertCurrent(message,topic)


# Define on_publish event Handler
#def on_publish(client, userdata, mid):
#    print("[MSG ENVIADA] Topico: "+mqtt_topic+" | Mensagem: "+ mqtt_msg)

# Retira da Tabela de Comands o ultimo valor
def get_status(rele_status):
    
    #temp = str(random.uniform(25., 30.0))

    topicNode = 'cmd/node1/rele1'
    
    # Acesso ao Banco de Dados
    conn = sqlite3.connect('database.db')
    cursor = conn.cursor()
    cursor.execute("SELECT comands FROM comands WHERE topic='%s' ORDER BY id DESC LIMIT 1" %(topicNode))
    result = cursor.fetchall()

    for row in result:

        mqtt_send_msg = row[0]
    
    conn.commit()
    conn.close()


    #cursor.execute("INSERT INTO currentrms (current, topic) VALUES (?,?)", (message,topic))
    #query = cursor.execute("SELECT comands FROM comands WHERE topic='%s' ORDER BY id DESC LIMIT 1" %(topicNode))    
    #print("query: "+str(query.fetchone()))
    #print("query: ")
    #.fetchone()
    #return {'currentrms': [i[0] for i in query.cursor.fetchall()]} # Fetches first column that is Employee ID

    return(mqtt_send_msg)
    
def publish_message(topic, message):
    
    print("["+ Fore.BLUE + "MSG ENVIADA"+ Style.RESET_ALL +"] Topico: "+topic+" | Mensagem: "+ message)   
    client.publish(topic, message)

try:

    print("["+ Fore.RED + "STATUS"+ Style.RESET_ALL +"] Inicializando MQTT...")

    # Metodos
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.on_subscribe = on_subscribe

    client.connect(broker, port, keppAlive)
    
    #client.loop_forever()
    client.loop_start()

    # Verifica Alteracao no Banco de Dados a cada 500ms
    while True:
        
        # Recebe o ultimo valor do banco
        mqtt_send_msg = get_status(mqtt_send_msg)

        # Verifica se nao eh igual ao penutimo
        if mqtt_send_msg != mqtt_send_msg_prev:

            # Publica no MQTT o valor do banco
            publish_message(mqtt_send_topic,mqtt_send_msg)
            # Armazena para o proximo loop
            mqtt_send_msg_prev = mqtt_send_msg

        # Aguarda 500ms
        time.sleep(0.5)

except KeyboardInterrupt:
    print "\nScript finalizado."
    sys.exit(0)
