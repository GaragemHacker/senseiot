pir alert -> ESP8266 + NodeMCU + Lua + MQTT = FUN ;)
=========

Introdução

Lua é uma linguagem um pouco familiar então resolvi testá-la com o ESP8266.
Para isso segui os passos do Augusto Campos e instalei o NodeMCU na minha
placa conforme o link http://br-arduino.org/2015/08/nodemcu-esp8266.html.
O processo é rápido e simples, necessita de alguns passos mas nada demais.
A nota que fica dessa primeira parte, é importante selecionar a opcao MQTT
na ferramenta http://frightanic.com/nodemcu-custom-build/ quando for escolher
os modulos a serem compilados.

Objetivo

Testar a integração da placa ESP8266 com NodeMCU e um broker MQTT. Neste
exemplo vou simular que o pino 2 da placa esta conectado a um sensor PIR,
poderia ser qualquer outro sensor digital como por exemplo um reed switch.

Para este exemplo usei os seguintes itens:

SW e HW
- placa ESP8266
- fonte alimentação 3.3V
- jumpers
- cabo serial ou conversor usb/serial
- conta no cloudmqtt.com
- teraterm ou coolterm ou outra ferramenta para acesso a serial
- luatool, disponivel em https://github.com/4refr0nt/luatool
- Python + Serial

Funcionamento

1) Execute os passos conforme o link do BR-Arduino para colocar o NodeMCU na
sua placa ESP. Atenção para a configuração dos pinos de IO pois são necesários
para atualizar o firmware da sua placa. Use os jumpers para mudar o estado dos
pinos. Depois de colocar o NodeMCU para rodar configure o pinos para:

RX e TX conectados no seu conversor, pode ser um modulo FTDI para ligar no PC.
CH_PH setao para HIGH
GPIO_0 setado para HIGH
GPIO_2 conectados no GND
RST conectado no GND

O pino GPIO_2 será usado para simular o comportamento de um sensor de presença,
por isso é legal deixar um jumper conectado nesse pino para poder mudar de HIGH
para LOW e para HIGH.

2) Crie uma conta no serviço https://www.cloudmqtt.com, é rápido e gratuito
para conexões de teste. Veja os planos que eles comercializam em 
https://www.cloudmqtt.com/plans.html Escolhi para este teste a opção Cute Cat.

3) Baixe o código exemplo pir_alert.lua e altere as variáveis de configuração
abaixo. Link para o código https://github.com/devlware/senseiot/blob/master/pir_alert/pir_alert.lua

Configure o ID e senha para seu roteador.
ssid = "id_roteador"
ssid_passwd = "senha_rede_wifi"

Indique um id para identificação da sua placa, pode ser qualquer texto.
m_id = "algum_id"

Preencha o usuário e senha com os dados obtidos na conta do cloudmqtt.
m_user = "user_cloudmqtt"
m_pass = "pass_cloud_mqtt"

Salve o arquivo, mantenha o nome como pir_alert.lua.

4) Pronto, agora você precisa enviar esse arquivo lua para a sua placa. Ele será
executado toda vez que a placa for alimentada ou reiniciada.
Use a ferramenta luatool baixada anteriormente para enviar o arquivo para sua
placa. O comando no Mac/Linux deve ser:
Substitua tty.usbserial pelo id do dispositivo serial usado.

./luatool.py --port /dev/tty.usbserial --src pir_alert.lua --dest init.lua --verbose

5) Agora remova a energia da placa, use seu programa preferido para abrir a porta
serial onde a ESP esta conectada. Ligue a alimentação da placa, você deve ver algumas
informações sendo exibidas no console Lua.

6) Aguarde alguns segundos, o ip, máscara e Mac da sua placa devem ser impressos
no console.

7) Depois uma mensagem deve informar que a conexão com o broker MQTT foi concluida.

8) Faça o login na sua conta no cloudmqtt e acesse Websocket UI.
https://api.cloudmqtt.com/sso/cloudmqtt/websocket Aqui você poderá ver o tópico
sendo publicado quando o pino 2 da sua placa mudar de LOW para HIGH.

9) Pronto, agora você pode testar seu setup. Mude o status do pino 2 (GPIO_2) de
LOW para HIGH. A mensagem "alerta intruso enviado" deve ser exibida na linha de
comando do Lua e uma entrada com topico alert/pir e mensagem 1 deve ser exibida
na tela de controle do cloudmqtt (Websocket UI).


TODO
- Monitorar pino do pir por um com interrupção.
- Botar a placa para dormir e acordar com a interrupção para economizar energia.
- Medir consumo da placa usando baterias.
