#include <CurieBLE.h>

#define SENSEABILITY_BUFFER_SIZE 8

#define LED_PIN 9
#define FAN_PIN 10
#define CONNECTED_LED_PIN 2

uint8_t senseBuf[SENSEABILITY_BUFFER_SIZE];

BLEService bleOutputService("6789aaaa-0000-1000-8000-00805f9b0131");
BLECharCharacteristic bleOutputLEDChar("1234ffff-0000-1000-8000-00805f9b0131",
                             BLERead | BLEWrite);
BLECharCharacteristic bleOutputFanChar("12350000-0000-1000-8000-00805f9b0131",
                             BLERead | BLEWrite);

BLEService bleSenseService("6789aaaa-0000-1000-8000-00805f9b0132");    // SenseAbility data Characteristic
BLECharacteristic bleSenseChar("1234ffff-0000-1000-8000-00805f9b0132", // standard 128-bit characteristic UUID
                             BLERead | BLENotify, sizeof(senseBuf));   // remote clients will be able to
                                                                       // get notifications if this characteristic changes
void ledWritten(BLEDevice central, BLECharacteristic characteristic);
void fanWritten(BLEDevice central, BLECharacteristic characteristic);
void bleDeviceConnectHandler(BLEDevice central);
void bleDeviceDisconnectHandler(BLEDevice central);

void setupBLE(void) {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, LOW);
  pinMode(CONNECTED_LED_PIN, OUTPUT);
  digitalWrite(CONNECTED_LED_PIN, LOW);
  
  BLE.begin();

  BLE.setLocalName("SenseAbility 2.0");
  BLE.setDeviceName("SenseAbility 2.0");
  
  BLE.setAdvertisedServiceUuid(bleOutputService.uuid());  // add the LED service UUID
  BLE.addService(bleOutputService); // Add the LED service
  bleOutputService.addCharacteristic(bleOutputLEDChar); // add the LED1 characteristic
  bleOutputService.addCharacteristic(bleOutputFanChar); // add the LED2 characteristic
  
  bleOutputLEDChar.setEventHandler(BLEWritten, ledWritten);
  bleOutputFanChar.setEventHandler(BLEWritten, fanWritten);
  const unsigned char zeroVal = 0;
  bleOutputLEDChar.setValue(0);
  bleOutputFanChar.setValue(0);
  
  BLE.setAdvertisedServiceUuid(bleSenseService.uuid());  // add the service UUID
  BLE.addService(bleSenseService); // Add the Senseability
  bleSenseService.addCharacteristic(bleSenseChar); // add the Senseability characteristic
  
  BLE.setEventHandler(BLEConnected, bleDeviceConnectHandler);
  BLE.setEventHandler(BLEDisconnected, bleDeviceDisconnectHandler);

  // Start the BLE service:
  BLE.advertise();
}

void sendBLE(uint8_t status, uint8_t temperature, uint8_t humidity, uint8_t magnet, float pressure, float airflow) {

  uint16_t pressureInt = pressure * 100.0;
  int16_t airflowInt = airflow;
  // Construct Senseability BLE data buffer
  senseBuf[0] = status;
  senseBuf[1] = temperature;
  senseBuf[2] = humidity;
  senseBuf[3] = magnet;
  senseBuf[4] = pressureInt & 0x00FF;
  senseBuf[5] = (pressureInt & 0xFF00) >> 8;
  senseBuf[6] = airflowInt & 0x00FF;
  senseBuf[7] = (airflowInt & 0xFF00) >> 8;
  bleSenseChar.setValue((unsigned char *)&(senseBuf[0]), sizeof(senseBuf));
}

void bleDeviceConnectHandler(BLEDevice central) {
  // central connected event handler
  Serial.print(F("Connected event, central: "));
  Serial.println(central.address());
  digitalWrite(CONNECTED_LED_PIN, HIGH);
}

void bleDeviceDisconnectHandler(BLEDevice central) {
  // central disconnected event handler
  Serial.print(F("Disconnected event, central: "));
  Serial.println(central.address());
  digitalWrite(CONNECTED_LED_PIN, LOW);
}

void ledWritten(BLEDevice central, BLECharacteristic characteristic) {
  int val = bleOutputLEDChar.value();
  Serial.println("Value: " + String(val));
  if (val == 1) {
    Serial.println(F("LED on"));
    digitalWrite(LED_PIN, HIGH);
  } else {
    Serial.println(F("LED off"));
    digitalWrite(LED_PIN, LOW);
  }
}

void fanWritten(BLEDevice central, BLECharacteristic characteristic) {
  int val = bleOutputFanChar.value();
  Serial.println("Value: " + String(val, HEX));
  if (val) {
    Serial.println(F("Fan on"));
    digitalWrite(FAN_PIN, HIGH);
  } else {
    Serial.println(F("Fan off"));
    digitalWrite(FAN_PIN, LOW);
  }

}

