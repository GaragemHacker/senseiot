--[[
	Exemplo basico de como usar o ESP8266 + NodeMCU + Lua + MQTT.
	Escrito por Diego W. Antunes <devlware@gmail.com> em 25/11/2015.
--]]

-- Variaveis usadas no script
ver = 0.2
ssid = "id_roteador"
ssid_passwd = "senha_rede_wifi"
pir_pin = 3
m_id = "algum_id"
m_server = "m11.cloudmqtt.com"
m_user = "user_cloudmqtt"
m_pass = "pass_cloud_mqtt"
m_port = 12912
mqtt_con = false
DEBUG = true
pir_state = {c = 0, o = 0}
state = 1

-- Cria instancia do MQTT.
m = mqtt.Client(m_id, 60, m_user, m_pass)

-- Callback de conexao.
m:on("connect", function(m) 
	mqtt_con=true
	if DEBUG then print ("\n\n", m_id, " conecto no HOST ", m_server, " na porta ", m_port, "\n\n") end
end)

-- Callback de desconexao.
m:on("offline", function(m)
	if DEBUG then
		print("\n\nDesconectado do broker.") print("Memoria: ", node.heap())
	end
	mqtt_con=false
	tmr.alarm(1, 10000, 0, function() m:connect(m_server, m_port, 0) end)
end)

-- Registra mensagem de testamento.
m:lwt("/lwt", "Putz, perdi a conexao!", 0, 0)

-- Quanto pir for para 1, envia mensagem para broker.
function send_alert()
	if mqtt_con then
		m:publish("alert/pir", "1", 0, 0, function(m) if DEBUG then print("alerta intruso enviado") end end)
	end
end

-- Rotina para monitorar a porta digital.
function pir_check()
	pir_state.c = gpio.read(pir_pin)
	if DEBUG then print(gpio.read(pir_pin)) end
	if (1 == pir_state.c) and (0 == pir_state.o) then
		send_alert()
	end
	pir_state.o = pir_state.c
end

-- Seta pino como entrada e imprime valor inicial.
-- Tambem inicializa timer de verificacao alarme.
function pir_init()
	gpio.mode(pir_pin,gpio.INPUT)
	tmr.alarm(0, 1000, 1, pir_check)
	if DEBUG then print("pir_init() ok") end
end

-- Para execucao timer.
function pir_stop()
	tmr.stop(0)
end

-- Inicializa WIFI e tenta conectar na rede.
function wi_conn()
	wifi.setmode(wifi.STATION)
	wifi.sta.config(ssid, ssid_passwd)
	ip,netmask = wifi.sta.getip()

	if nil ~= ip then
		if DEBUG then print(ip, netmask, wifi.sta.getmac()) end
		wifi.sta.autoconnect(1)
		state=state+1
	else
		if DEBUG then print("Tentando conectar...") end
	end
end

-- Inicializa WIFI e registrar MQTT.
function machine()
	if 1 == state then
		wi_conn()
	elseif 2 == state then
		if false == mqtt_con then
			m:connect(m_server, m_port, 0)
		else
			state=state+1
		end
	elseif 3 == state then
		-- Inicializa monitoramento.
		pir_init()
		pir_check()
		tmr.stop(1)
	end
end

-- Rotina que controla o inicio da aplicacao.
tmr.alarm(1, 5000, 1, machine)
