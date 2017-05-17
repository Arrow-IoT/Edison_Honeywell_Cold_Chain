/* spec jslint and jshint lines for desired JavaScript linting */
/* see http://www.jslint.com/help.html and http://jshint.com/docs */
/* jslint node:true */
/* jshint unused:true */

"use strict" ;

var senseability = require('./ble-senseability');
var udpForwarder = require('./udpForwarder');

var fridge = new senseability("98:4F:EE:0D:0F:64");
var udp = new udpForwarder(41000, '127.0.0.1', 42000, '127.0.0.1');

fridge.on('update', function(data) {
  udp.add('status', data.status, 'i');
  udp.add('temperature', data.tempF, 'f');
  udp.add('humidity', data.humidity, 'f');
  udp.add('magnet', data.magnet, 'b');
  udp.add('pressure', data.pressure, 'f');
  udp.add('airflow', data.airflow, 'f');
  udp.send();
});

fridge.on('disconnect', function() {
  console.log("disconnected");
  fridge.scan();
});

udp.on('command', function(command, payload) {
  console.log("Command: " + command);
  console.log("Payload: " + payload.value);
  switch (command) {
    case 'led':
      fridge.setLED(payload.value);
      break;
    case 'fan':
      fridge.setFan(payload.value);
      break;
  }
});

udp.on("err", function(err) {
  console.log("emitter error: " + err);
});

udp.on("uncaughtException", function(err) {
  console.log("emitter uncaught exception: " + err);
});