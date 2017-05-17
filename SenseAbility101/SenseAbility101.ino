#include <CurieTimerOne.h>

#define SENSOR_UPDATE_RATE 15000000 // 15 seconds
bool updateSensors = false;

float humidity = 0;
float pressure = 0;
float tempP = 0;
float tempH = 0;
float airflow = 0;
uint8_t magnet = 255;

void setup() {
  Serial.begin(115200);
  setupSensors();
  setupBLE();
  CurieTimerOne.start(SENSOR_UPDATE_RATE, &triggerSensorUpdate);
}

void loop() {
  //if (BLE_Connected()) {
    if (updateSensors) {
      
      uint8_t status = readSensors(humidity, tempH, pressure, tempP, airflow, magnet);
      float tempAvg = (tempP + tempH) / 2.0;
      Serial.println("Pressure: " + String(pressure, 3) + " psi");
      Serial.println("Humidity: " + String(humidity, 3) + " %");
      Serial.println("Temperature (average): " + String(tempAvg) + " deg C");
      Serial.println("Airflow: " + String(airflow, 3) + "%");
      Serial.println("Magnet: " + String(magnet));
  
      sendBLE(status, tempAvg, humidity, magnet, pressure, airflow);
      Serial.println("Connection Status: " + String(BLE_Connected()));
      Serial.println();
      
      updateSensors = false;
      
    }
  //}
}

void triggerSensorUpdate(void) {
  updateSensors = true;
}
