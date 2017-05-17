# Edison Smart Home Arrow Connect Demo

Overview

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