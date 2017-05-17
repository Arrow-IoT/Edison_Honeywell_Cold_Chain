#include <CurieTimerOne.h>

#define SENSOR_UPDATE_RATE 1000000 // 1 second

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
  if (updateSensors) {
    
    uint8_t status = readSensors(humidity, tempH, pressure, tempP, airflow, magnet);
    float tempAvg = (tempP + tempH) / 2.0;

    sendBLE(status, tempAvg, humidity, magnet, pressure, airflow);
    
    updateSensors = false;
    
  }
}

void triggerSensorUpdate(void) {
  updateSensors = true;
}
