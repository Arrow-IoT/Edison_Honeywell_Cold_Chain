# Edison Smart Home Arrow Connect Demo

Combining Honeywell sensing, Intel technology, and Arrow Connect to track temperature, and other environmental conditions remotely and reliably. The system also supports on-the-fly rule definitions to trigger alerts or autonomously take actions.

## Summary

Utilizing multiple low-power sensing nodes – wirelessly connected via Bluetooth to a central Intel gateway – the Intel/Honeywell Cold Chain Monitor reliably reports environmental sensor data to the cloud through the Arrow Connect platform. The system reports temperature, humidity, pressure, and airflow readings at programmable intervals, which can be monitored through simple API.

![Demo Architecture](https://github.com/Arrow-IoT/Edison_Honeywell_Cold_Chain/raw/master/Documentation/Images/demo-architecture.png)

## Directory

* **Documentation** -- Documentation for all components of the demo
    * **SenseAbility** -- SenseAbility 2.0 and Honeywell sensors documentation
* **node-red** -- Example node-red flow demonstrating SenseAbility dashboard and light control
    * **Language**: JavaScript/Node.JS
    * **IDE**: node-red
* **Selene** -- Example Arrow Connect engine (Selene) configuration files for SenseAbility and UDP devices.
* **SenseAbility101** -- Arduino 101 firmware for integrating with the SenseAbility shield and writing the data out to a BLE characteristic.
    * **Language**: Arduino (C++)
    * **IDE**: [Arduino](https://www.arduino.cc/en/Guide/Arduino101)
* **node-udp-ble-agent** -- Node.JS script setting the Edison up as a UDP server, taking action on certain JSON payloads.
    * **Language**: Node.JS
    * **IDE**: [Intel XDK](https://software.intel.com/en-us/intel-xdk)
    
## Instructions

(Work in progress. Outline below.)

1. Assemble hardware
    a. Arduino 101 w/ SenseAbility Shield
    b. Intel Edison 
2. Flash Edison with latest firmware, and connect it to your WiFi network.
3. Install selene on the Intel Edison.
4. Update and SCP self.properties and udp-forwarder.properties files from **Selene** folder. Also update selene self.config file.
5. Use XDK to flash **node-udp-ble-agent** firwmare onto Edison. Make sure it's set to run on startup.
6. Enter **Arduino** folder and upload code to an Arduino 101.
7. Run selene and node-udp-ble-agent at same time.
8. Download Node.js then install node-red.
9. Import the flow from the **node-red/flow** folder.
10. Edit global variables in the node-red flow.
11. Deploy node red flow and watch data fly in!

Going further: attach an LED and fan to the Arduino 101 to control them from the dashboard.