SenseAbility Arduino 101 Source Code
====================================

This Arduino sketch is written for the Arduino 101.

It interfaces with a Honeywell SenseAbility 2.0 shield, which includes four sensors:

* Magnetic - SM351LT Nanopower Magnetoresistive Sensor
* Pressure - HSCMNNN001BA3A3 TruStability Silicon Pressure Sensor
* Humidity/Temperature - HIH61331-021-001 HumidIcon Humidity & Temperature Sensor
* Airflow - HAFBLF0200C4AX3 Zephyr Airflow Sensor

The Arduino 101 also operates as a BLE Peripheral device. With three characteristics:

1. Sensor Output - Data from all sensors is output as an 8-byte packet
2. LED Input - Characteristic can be written to 0 or 1 to turn LED off or on.
3. Fan Input - Characteristic can be written to 0 or 1 to turn fan off or on. 

## Upload Instructions

1. Download [Arduino](https://www.arduino.cc/en/Main/Software)
2. Download the [Arduino 101 Board Definitions](https://www.arduino.cc/en/Guide/Arduino101#toc2)
3. Open **SenseAbility101.ino** in the Arduino IDE.
    * You should see BLE.ino and Sensors.ino open in separate tabs.
4. Plug in your Arduino 101 and find its serial port number
5. Select the Serial Port number from the Tools > Serial Port menu.
6. Select "Arduino/Genuino 101" from the Tools > Board menu.
7. Click upload.