
// Script JavaScript
// Config Broker Connection
var websocketserver = "104.236.109.245";		//'45.62.225.233';		//'1.1.1.3'		//'iot.eclipse.org'; //'broker.hivemq.com'; //'192.168.25.4'; //'192.168.110.82';
var websocketport   =  9001;	//8000;	//9001; //80; 
var topic = 'dev/node1/#';

var ledtopic = 'clients/arduino/led';	// Web PUBlishes to ledtopic + "/cmd"
