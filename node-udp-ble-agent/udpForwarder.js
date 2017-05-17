/* jslint node:true */
/* jshint unused:true */

var dgram = require('dgram');
var inherits = require('util').inherits;
var EventEmitter = require('events').EventEmitter;

var client = dgram.createSocket('udp4');

function udpForwarder(clientPort, clientHost, serverPort, serverHost) {
  var self = this;
  EventEmitter.call(this);
  
  self._clientPort = clientPort;
  self._clientHost = clientHost;
  self._serverPort = serverPort;
  self._serverHost = serverHost;
  
  var server = dgram.createSocket('udp4');
  
  this._telemetry = [];
  
  client.on("message", function(msg, rinfo) {
    console.log('the packet came back: ' + msg + '(' + rinfo + ')');
  });
  
  server.bind(this._serverPort, this._serverHost);
  
  server.on('listening', function() {
    var address = server.address();
    console.log('server listening ' + address.address + ':' + address.port);
  });
  
  server.on('message', function (msg, rinfo) {
    console.log('server got: ' + msg + ' from ' + rinfo.address + ':' + rinfo.port);
    var jsonMsg = JSON.parse(msg);
    var jsonPayload = JSON.parse(jsonMsg.payload);
    self.emit('command', jsonMsg.command, jsonPayload);
  });
}

inherits(udpForwarder, EventEmitter);

udpForwarder.prototype.send = function() {
  var msg = convertObjToTelemJSON(this._telemetry);
  client.send(msg, 0, msg.length, this._clientPort, this.clientHost, function(err) {
    if (err !== 0) {
      console.log("Send err: " + err);
    } else {
      console.log("Sent: " + msg);
    }
  });  
  this._telemetry = [];
};

udpForwarder.prototype.add = function(name, value, type) {
  this._telemetry.push({'name': name, 'value': value, 'type': type});
};

function convertObjToTelemJSON(obj) {
  var msgBuf = new Buffer("{");
  obj.forEach(function(entry, index) {
    msgBuf += "\"" + entry.type + "|" + entry.name + "\":\"" + entry.value + "\"";
    if (index < (obj.length - 1)) {
      msgBuf += ',';
    } else {
      msgBuf += '}';
    }
  });
  
  return msgBuf;
}

module.exports = udpForwarder;