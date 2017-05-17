#include <Wire.h>

#define I2C_ADDRESS_PRESSURE 0x38
#define I2C_ADDRESS_HUMIDITY 0x27
#define I2C_ADDRESS_AIRFLOW  0x49

#define PRESSURE_OUTPUT_MIN 1638.4f  // (2<<13)*0.1 (10% of 2^14)
#define PRESSURE_OUTPUT_MAX 14745.6f // (2<<13)*0.9 (90% of 2^14)
#define PRESSURE_MIN 0.0f
#define PRESSURE_MAX 1.0f

#define FLOW_RATE_0 8192.0f

#define SM351LT_PIN 13

void setupSensors() {
  pinMode(SM351LT_PIN, INPUT);
  Wire.begin();
}

uint8_t readSensors(float & humidity, float & tempH, float & pressure, float & tempP, float & airflow, uint8_t & magnet) {
  uint8_t status;

  status =  readHumidity(humidity, tempH) == 0 ? 1<<0 : 0;
  status |= readPressure(pressure, tempP) == 0 ? 1<<1 : 0;
  status |= readAirflow(airflow) == 0 ? 1<<2 : 0;
  status |= readMagnet(magnet) == 0 ? 1<<3 : 0;
  
  return status;
}

uint8_t readHumidity(float & humidity, float & temperature) {
  unsigned char raw_data[4];

  // Send a Measurement request (MR)
  Wire.beginTransmission(I2C_ADDRESS_HUMIDITY);
  Wire.write((byte)0); 
  Wire.endTransmission(true); // Send a stop bit

  delay(50); // Wait >36.5ms (typical) for measurement to complete
  
  // Send a data fetch (DF)
  Wire.requestFrom(I2C_ADDRESS_HUMIDITY, 4);
  for (int i=0; i<4; i++) {
    raw_data[i] = Wire.read();
  }
  
  uint8_t status = (raw_data[0] & 0xc0); // Mask out 2 msb's
  status >>= 6; // Shift right 6 bits
  uint16_t raw_humidity = ((raw_data[0] & 0x3F) << 8) | raw_data[1];
  int16_t raw_temperature = ((raw_data[2] << 8) | (raw_data[3] & 0xFC)) >> 2;
  
  humidity = (((float) raw_humidity) / 16383.0) * 100.0;
  temperature = (((float) raw_temperature) / 16383.0) * 165.0 - 40.0;
  
  return status;
}

uint8_t readPressure(float & pressure, float & temperature) {
  unsigned char raw_data[4];
  Wire.requestFrom(I2C_ADDRESS_PRESSURE, 4);
  for (int i=0; i<4; i++) {
    raw_data[i] = Wire.read();
  }
  
  uint8_t status = (raw_data[0] & 0xc0); // Mask out 2 msb's
  status >>= 6; // Shift right 6 bits
  uint16_t raw_pressure = ((raw_data[0] & 0x3F) << 8) | raw_data[1];
  int16_t raw_temperature = ((raw_data[2] << 8) | (raw_data[3] & 0xE0)) >> 5;
  
  pressure = (((float)raw_pressure - PRESSURE_OUTPUT_MIN)*(PRESSURE_MAX - PRESSURE_MIN)) / 
                       (PRESSURE_OUTPUT_MAX - PRESSURE_OUTPUT_MIN) - PRESSURE_MIN;
  temperature = (((float)raw_temperature / 2047.0) * 200.0) - 50.0;
  
  return status;
}

uint8_t readAirflow(float & airflow) {
  unsigned char raw_data[2];
  
  Wire.requestFrom(I2C_ADDRESS_AIRFLOW, 2);
  for (int i=0; i<2; i++) {
    raw_data[i] = Wire.read();
  }

  uint8_t status = (raw_data[0] & 0xc0); // Mask out 2 msb's
  status >>= 6; // Shift right 6 bits
  // !!! handle: if status != 0, store serial number

  int16_t raw_airflow = (((raw_data[0] & 0x3F) << 8) | raw_data[1]); // - FLOW_RATE_0;
  airflow = (((float)raw_airflow - 1638.0) * (200.0) / (13107.0)) - 100.0;
  
  return status;
}

uint8_t readMagnet(uint8_t & magnet) {
  magnet = digitalRead(SM351LT_PIN);
  return 0;
}

