#include <CurieBLE.h>

#define SENSEABILITY_BUFFER_SIZE 8

uint8_t senseBuf[SENSEABILITY_BUFFER_SIZE];

BLEService blePlaceholderService("6789aaaa-0000-1000-8000-00805f9b0131");
BLECharCharacteristic blePlaceholderChar1("1234ffff-0000-1000-8000-00805f9b0131",
                             BLERead | BLEWrite);
BLECharCharacteristic blePlaceholderChar2("12350000-0000-1000-8000-00805f9b0131",
                             BLERead | BLEWrite);

BLEService bleSenseService("6789aaaa-0000-1000-8000-00805f9b0132");    // SenseAbility data Characteristic
BLECharacteristic bleSenseChar("1234ffff-0000-1000-8000-00805f9b0132", // standard 128-bit characteristic UUID
                             BLERead | BLENotify, sizeof(senseBuf));   // remote clients will be able to
                                                                       // get notifications if this characteristic changes

void setupBLE(void) {
  BLE.begin();

  BLE.setLocalName("SenseAbility 2.0");
  BLE.setDeviceName("SenseAbility 2.0");
  
  BLE.setAdvertisedServiceUuid(blePlaceholderService.uuid());  // add the service UUID
  BLE.addService(blePlaceholderService); // Add the Senseability
  blePlaceholderService.addCharacteristic(blePlaceholderChar1); // add the Senseability characteristic
  blePlaceholderService.addCharacteristic(blePlaceholderChar2); // add the Senseability characteristic
  
  BLE.setAdvertisedServiceUuid(bleSenseService.uuid());  // add the service UUID
  BLE.addService(bleSenseService); // Add the Senseability
  bleSenseService.addCharacteristic(bleSenseChar); // add the Senseability characteristic

  // Start the BLE service:
  BLE.advertise();
}

bool BLE_Connected(void) {
  BLEDevice central = BLE.central();
  
  if (central) {
    return true;
  }

  return false;
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

  for (int i=0; i<8; i++) {
    Serial.print(String(senseBuf[i], HEX) + " ");
  }
  Serial.println();
}

