/* spec jslint and jshint lines for desired JavaScript linting */
/* see http://www.jslint.com/help.html and http://jshint.com/docs */
/* jslint node:true */
/* jshint unused:true */

var inherits = require('util').inherits;
var EventEmitter = require('events').EventEmitter;
var noble = require('noble');

var outputServiceUUID = '6789aaaa00001000800000805f9b0131';
var ledCharUUID = '1234ffff00001000800000805f9b0131';
var fanCharUUID = '1235000000001000800000805f9b0131';

var senseAbilityServiceUUID = '6789aaaa00001000800000805f9b0132';
var senseAbilityCharUUID = '1234ffff00001000800000805f9b0132';

var senseAbilityCharacteristic = null;
var ledCharacteristic = null;
var fanCharacteristic = null;

function senseability(senseAddr) {
  var self = this;
  self.address = senseAddr;
  EventEmitter.call(this);
  
  noble.on('stateChange', function(state) {
    if (state === 'poweredOn') {
      noble.startScanning();
    } else {
      noble.stopScanning();
    }
  });

  noble.on('discover', function(peripheral) {
    if (peripheral.address === senseAddr.toLowerCase()) {
      noble.stopScanning();
      
      peripheral.on('disconnect', function(err) {
        self.emit('disconnect', err);
      });
      
      peripheral.connect(function(err) {
        if (err !== null) {
          console.log("Connect error: " + err);
        }
        
        peripheral.discoverServices([], function(err, services) {
          services.forEach(function(service) {
            if (service.uuid === senseAbilityServiceUUID) {
              service.discoverCharacteristics([senseAbilityCharUUID], function(err, characteristics) {
                characteristics.forEach(function(characteristic) {
                  senseAbilityCharacteristic = characteristic;

                  self.connected();                
                });
              });
            }
            else if (service.uuid === outputServiceUUID) {
              service.discoverCharacteristics([], function(err, characteristics) {
                characteristics.forEach(function(characteristic) {
                  if (characteristic.uuid === ledCharUUID) {
                    ledCharacteristic = characteristic;
                  } else if (characteristic.uuid === fanCharUUID) {
                    fanCharacteristic = characteristic;
                  }         
                });
              });
            }
          });
        });
      });
    }
  });
}

inherits(senseability, EventEmitter);

senseability.prototype.scan = function() {
  noble.startScanning();  
};

senseability.prototype.connected = function() {
  var self = this;
  console.log("Connected to senseability (" + self.address + ")");
                
  senseAbilityCharacteristic.subscribe(function(err) {
    if (err !== null) {
      console.log("Error subscribing: " + err);
    }
  });
                
  senseAbilityCharacteristic.on('data', function(data) {
    var sensorData = [];
    
    sensorData.status =  data[0];
    sensorData.tempC = data[1];
    sensorData.tempF = Math.round(data[1] * 9 / 5 + 32);
    sensorData.humidity = data[2];
    sensorData.magnet = data[3];
    sensorData.pressure = ((data[5] << 8) | data[4]) / 100;
    sensorData.airflow = (data[7] << 8) | data[6];
    
    self.emit('update', sensorData);
  });
};

senseability.prototype.setLED = function(value) {
  var msg = new Buffer(1);
  msg.writeUInt8(value, 0);
  ledCharacteristic.write(msg);
};

senseability.prototype.setFan = function(value) {
  var msg = new Buffer(1);
  msg.writeUInt8(value, 0);
  fanCharacteristic.write(msg);
};

module.exports = senseability;