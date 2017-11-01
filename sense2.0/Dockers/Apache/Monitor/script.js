
// Script JavaScript

var on_led3 = null;				// ToogleLED
var on_led4 = null;				// ToogleLED
var on_led5 = null;				// ToogleLED

/*				  MQTT Topic Struct

				  'dev/node1/led1'
				    |    |     |
                    |    |     |
            device(dev)  |    variable to sub/pub 
            			 | (led1..n,temp1...n,button1...n)
               or        |
            command(cmd) |
            			 |
            	IoT device node (node1...n)

*/

var topic_led1 = 'dev/node1/led1';
var topic_led2 = 'dev/node1/led2';
var topic_led3 = 'dev/node1/led3';
var topic_led4 = 'dev/node1/led4';
var topic_led5 = 'dev/node1/led5';
var topic_button1 = 'dev/node1/led3';
var topic_button2 = 'dev/node1/led4';
var topic_button3 = 'dev/node1/led5';
var topic_temp1 = 'dev/node1/temperature';
var topic_hum1 = 'dev/node1/humidity';

var tempGauge;					// Object
var humGauge;					// Object

var TempValue = new Array();	// Temp Historic [19]
var HumValue = new Array();		// Hum Historic [19]

// Create a Client with a random ID
var client = new Messaging.Client(websocketserver, websocketport,"web_" + parseInt(Math.random() * 100, 10));

// To make a function available after the document is loaded, then init the connection
$(document).ready(function() {
	 
	init();
		
});

// Config. LED Connection
var options = {
		
		timeout: 3,
		
		onSuccess: function () {
				$('#message').html('Connected to ' + websocketserver + ':' + websocketport);
				// Connection succeeded; subscribe to our topic
				// To Do verify this lines above
				client.subscribe(topic, {qos: 0});
				client.subscribe(ledtopic, {qos: 0});
				// $('#topic').val(topic);

				humGauge.setLedColor(steelseries.LedColor.GREEN_LED); 
				//change status LED to GREEN on broker connection
				
				tempGauge.setLedColor(steelseries.LedColor.GREEN_LED); 
				//change status LED to GREEN on broker connection

		},
		
		onFailure: function (message) {
				$('#message').html("Connection failed: " + message.errorMessage);

				humGauge.setLedColor(steelseries.LedColor.RED_LED); 
				//change status LED to RED on broker disconnection

				tempGauge.setLedColor(steelseries.LedColor.RED_LED); 
				//change status LED to RED on broker disconnection

		}
};

// Function to init and config. the gauges
function init() {
		
		/* Connect to MQTT broker */
		client.connect(options);

		tempGauge = new steelseries.Radial('gaugeCanvas', {
				gaugeType: steelseries.GaugeType.TYPE4,
				minValue:-10,
				maxValue:80,
				size: 250,
				frameDesign: steelseries.FrameDesign.STEEL,
				knobStyle: steelseries.KnobStyle.STEEL,
				pointerType: steelseries.PointerType.TYPE6,
				lcdDecimals: 0,
				section: null,
				area: null,
				titleString: 'Temperature',
				unitString: 'C',
				threshold: 0,
				lcdVisible: true,
				lcdDecimals: 2
		});
		
		// gives a blank display 'NaN' until broker has connected
		tempGauge.setValue(''); 
		// set the LED RED until connected
		tempGauge.setLedColor(steelseries.LedColor.RED_LED); 
		
		humGauge = new steelseries.Radial('humGaugeCanvas', {
				gaugeType: steelseries.GaugeType.TYPE4,
				minValue:0,
				maxValue:100,
				size: 250,
				frameDesign: steelseries.FrameDesign.STEEL,
				knobStyle: steelseries.KnobStyle.STEEL,
				pointerType: steelseries.PointerType.TYPE6,
				lcdDecimals: 0,
				section: null,
				area: null,
				titleString: 'Humidity',
				unitString: '%',
				threshold: 0,
				lcdVisible: true,
				lcdDecimals: 2
		});

		// gives a blank display 'NaN' until broker has connected
		humGauge.setValue(''); 
		// set the LED RED until connected
		humGauge.setLedColor(steelseries.LedColor.RED_LED);

}

// Function to change the LED gauge to red if the connection with broker is lost and update massage
client.onConnectionLost = function (responseObject) {
		$('#message').val("connection lost: " + responseObject.errorMessage);
		tempGauge.setLedColor(steelseries.LedColor.RED_LED); 
		//change status LED to RED on broker disconnection
		humGauge.setLedColor(steelseries.LedColor.RED_LED); 
		//change status LED to RED on broker disconnection
};

// Function to MQTT Message Arrived 
client.onMessageArrived = function (msg) {
	
	$('#message').val(msg.payloadString);

	console.log(JSON.stringify(msg.destinationName));
	console.log(JSON.stringify(msg.payloadString));
	
	topic_parts = msg.destinationName.split('/');
	
	// LED1 STATUS
	if (msg.destinationName == topic_led1) {
		
		var status_led1 = parseInt(msg.payloadString);
		console.log("GOTCHA " + status_led1);
		
		if (status_led1) {
			$("#status_off_led1").hide();
			$("#status_on_led1").show();
		} else {
			$("#status_off_led1").show();
			$("#status_on_led1").hide();
		}
		
		return;
	}
	
	// LED2 STATUS
	if (msg.destinationName == topic_led2) {
		
		var status_led2 = parseInt(msg.payloadString);
		console.log("GOTCHA " + status_led2);
		
		if (status_led2) {
			$("#status_off_led2").hide();
			$("#status_on_led2").show();
		} else {
			$("#status_off_led2").show();
			$("#status_on_led2").hide();
		}
		
		return;
	}
	
	// LED3 STATUS
	if (msg.destinationName == topic_led3) {
		
		var status_led3 = parseInt(msg.payloadString);
		console.log("GOTCHA " + status_led3);
		
		if (status_led3) {
			
			// TO DO - instead signal on_led change to send a msg on new topic to feedback
			
			on_led3 = true;
			$("#status_off_led3").hide();
			$("#status_on_led3").show();
		} else {
			on_led3 = false;
			$("#status_off_led3").show();
			$("#status_on_led3").hide();
		}

		return;
	}

	// LED4 STATUS
	if (msg.destinationName == topic_led4) {
		
		var status_led4 = parseInt(msg.payloadString);
		console.log("GOTCHA " + status_led4);
		
		if (status_led4) {
			on_led4 = true;
			$("#status_off_led4").hide();
			$("#status_on_led4").show();
		} else {
			on_led4 = false;
			$("#status_off_led4").show();
			$("#status_on_led4").hide();
		}
		
		return;
	}

	// LED5 STATUS
	if (msg.destinationName == topic_led5) {
		
		var status_led5 = parseInt(msg.payloadString);
		console.log("GOTCHA " + status_led5);
		
		if (status_led5) {
			on_led5 = true;
			$("#status_off_led5").hide();
			$("#status_on_led5").show();
		} else {
			on_led5 = false;
			$("#status_off_led5").show();
			$("#status_on_led5").hide();
		}
		
		return;
	}
	
	sensor_addr = topic_parts[2];
	//sensor_addr = "Room Light";
	
	// TEMPERATURE
	//if (topic_parts[topic_parts.length - 1] == 'temperature') {
	if (msg.destinationName == topic_temp1) {	
	1
		// Label
		$('#TempValueLabel').text(msg.payloadString + ' C');
		$('#TempValueLabel').removeClass('').addClass('label-default');

		TempValue.push(parseInt(msg.payloadString));
		
		if (TempValue.length >= 20) {
			TempValue.shift();
		}

		// Chart
		$('#TempValueChart').sparkline(TempValue, {
			type: 'line',
			width: '180',
			height: '40',
			verticalAxisVisibility: 'visible',
			horizontalAxisVisibility: 'visible',
			formatLabel: 'Temperature',
		});
		
		// // Chart
		// $('#TempValueChart').igSparkline(TempValue, {
		// 	displayType:'line',
		// 	width: '180',
		//  	height: '40',
		// });


		// Gauge
		tempGauge.setValue(msg.payloadString);
		
		// Sensor Addr
		if (msg.payloadString < '50') {
			$('#label_temp').text(sensor_addr + ' OK');
			$('#label_temp').removeClass('label-danger').addClass('label-success');
		} else {
			$('#label_temp').text('high ' + sensor_addr);
			$('#label_temp').removeClass('label-success').addClass('label-danger');
		}
	}
	
	// HUMIDITY
	//if (topic_parts[topic_parts.length - 1] == 'humidity') {
	if (msg.destinationName == topic_hum1) {

		// Label
		$('#HumValueLabel').text(msg.payloadString + '%');
		$('#HumValueLabel').removeClass('').addClass('label-default');

		HumValue.push(parseInt(msg.payloadString));
		
		if (HumValue.length >= 20) {
			HumValue.shift();
		}

		// Chart
		$('#HumValueChart').sparkline(HumValue, {
			type: 'line',
			width: '180',
			height: '40'
		});
		
		// Gauge
		humGauge.setValue(msg.payloadString);
		
		// Sensor Addr
		if (msg.payloadString < '50') {
			$('#label_hum').text('low  ' + sensor_addr);
			$('#label_hum').removeClass('label-success').addClass('label-danger');
		} else {
			$('#label_hum').text(sensor_addr + ' OK');
			$('#label_hum').removeClass('label-danger').addClass('label-success');
		}
	}
};


// NOTE: USE FUNCTION PROTOTYPE TO TOGGLE THE LEDS
// Provides the button logic that toggles our display LED on and off

// function toggle_led(on_led, topic){
		
// 		this.on_led = on_led;
// 		this.topic = topic;


// 		if (on_led){

// 			message = new Messaging.Message("0");
// 			message.destinationName = topic;
// 			client.send(message);
// 		}
// 		else {
	
// 			message = new Messaging.Message("1");
// 			message.destinationName = topic;
// 			client.send(message);	
// 		}
// 	}

//var toggle_led3 = new toggle_led('on_led3','clients/arduino/led3');
// var toggle_led3 = new toggle_led();
// var toggle_led4 = new toggle_led('on_led4','clients/arduino/led4');
// var toggle_led5 = new toggle_led('on_led5','clients/arduino/led5');

// toggle_led3('on_led3','"clients/arduino/led3"');

function toggle_button1(){
	
		if (on_led3){

			message = new Messaging.Message("0");
			message.destinationName = topic_button1;
			client.send(message);
		}
		else {
	
			message = new Messaging.Message("1");
			message.destinationName = topic_button1;
			client.send(message);	
		}
	}

function toggle_button2(){
	
		if (on_led4){

			message = new Messaging.Message("0");
			message.destinationName = topic_button2;
			client.send(message);	
		}				
		else {
	
			message = new Messaging.Message("1");
			message.destinationName = topic_button2;
			client.send(message);
		}
	}

function toggle_button3(){
	
		if (on_led5){

			message = new Messaging.Message("0");
			message.destinationName = topic_button3;
			client.send(message);	
		}
		else {
	
			message = new Messaging.Message("1");
			message.destinationName = topic_button3;
			client.send(message);				
		}
}

// // Simulate the send data
// var interval = window.setInterval(function() {

// 	// Random Value
// 	var rand_temp = parseInt(Math.random() * 100, 10);
// 	var rand_hum  = parseInt(Math.random() * 100, 10);
// 	var t = rand_temp.toString();
// 	var h = rand_hum.toString();

// 	// Send Temperature Topic
// 	message = new Messaging.Message(t);
// 	message.destinationName = topic_temp1;
// 	client.send(message);
	
// 	// Send Humidity Topic
// 	message = new Messaging.Message(h);
// 	message.destinationName = topic_hum1;
// 	client.send(message);
// }, 4000); // 4000 ms	


