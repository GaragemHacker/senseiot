-- Escrito por Diego W. Antunes <devlware@gmail.com>
-- Em 23/11/2015

-- Exemplo de utilizacao de script Lua com NodeMCU,
-- protocolo MQTT e IO.

-- Variaveis usadas no script
ssid = "id_roteador"
ssid_passwd = "senha_rede_wifi"
pir_pin=3
m_id="algum_id"
m_server="m11.cloudmqtt.com"
m_user="user_cloud_mqtt"
m_pass="senha_cloud_mqtt"
m_port=12912
mqtt_con=false
pir_state = {c = 0, o = 0}

-- Configura e faz a conexao com o roteador.
function wi_conn()
	wifi.setmode(wifi.STATION)
	wifi.sta.config(ssid, ssid_passwd)
	ip,netmask = wifi.sta.getip()
	print(ip, netmask, wifi.sta.getmac())
end

-- Quanto pir for para 1, envia mensagem para broker.
function send_alert()
	if mqtt_con then
		m:publish("alert/pir", "1", 0, 0, function(m) print("alerta intruso enviado") end)
	end
end

-- Rotina para monitorar a porta digital
function pir_check()
	pir_state.c = gpio.read(pir_pin)
	print(gpio.read(pir_pin))
	if (1 == pir_state.c) and (0 == pir_state.o) then
		send_alert()
	end
	pir_state.o = pir_state.c
end

-- Seta pino como entrada e imprime valor inicial.
-- Tambem inicializa timer de verificacao alarme.
function pir_init()
	gpio.mode(pir_pin,gpio.INPUT)
	print(gpio.read(pir_pin))
	tmr.alarm(0, 1000, 1, pir_check)
end

-- Para execucao timer.
function pir_stop()
	tmr.stop(0)
end

-- Cria instancia do MQTT.
m = mqtt.Client(m_id, 60, m_user, m_pass)

-- Registra mensagem de testamento.
m:lwt("/lwt", "Putz, perdi a conexao!", 0, 0)

-- Callback de conexao.
m:on("connect", function(m) 
	mqtt_con=true
	print ("\n\n", m_id, " connected to MQTT host ", m_server, " on port ", m_port, "\n\n")
	-- Subscribe to the topic where the ESP8266 will get commands from
	m:subscribe("/mcu/cmd/#", 0, function(m) print("Subscribed to CMD Topic") end)
end)

-- Callback de desconexao.
m:on("offline", function(m)
	print ("\n\nDisconnected from broker")
	print("Heap: ", node.heap())
	mqtt_con=false
	tmr.alarm(1, 10000, 0, function() m:connect(m_server, m_port, 0) end)
end)

m:connect(m_server, m_port, 0)
