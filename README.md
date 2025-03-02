# ESP32 IoT Project with Blynk and MQTT

## Overview
This project implements an IoT system using an ESP32 microcontroller, which connects to WiFi, reads data from multiple sensors (DHT22, ADXL345, LM35), and sends the data to Blynk and an MQTT broker. Additionally, it provides remote control of an LED via Blynk.

## Features
- **WiFi Connectivity**: Connects to a WiFi network to transmit sensor data.
- **Blynk Integration**: Displays sensor data and controls devices remotely.
- **MQTT Communication**: Publishes sensor readings to an MQTT broker.
- **Sensor Support**:
  - **DHT22**: Measures temperature and humidity.
  - **ADXL345**: Captures acceleration and vibration data.
  - **LM35**: Measures temperature and controls an RGB LED based on readings.
- **RGB LED Control**: Adjusts LED color based on temperature thresholds.
- **Remote LED Control**: Enables users to toggle an LED via the Blynk app.

## Hardware Requirements
- ESP32 Microcontroller
- DHT22 Temperature & Humidity Sensor
- ADXL345 Accelerometer Sensor
- LM35 Temperature Sensor
- RGB LED (Red, Green, Blue)
- WiFi Network

## Software Requirements
- Arduino IDE
- ESP32 Board Package
- Blynk Library
- PubSubClient Library
- DHT Sensor Library
- Adafruit Sensor Library
- Adafruit ADXL345 Library

## Installation & Setup
1. **Install Arduino IDE**: Download and install from [Arduino Official Website](https://www.arduino.cc/en/software).
2. **Add ESP32 Board**:
   - Open Arduino IDE and go to `File > Preferences`.
   - In the "Additional Board Manager URLs" field, enter: `json`.
   - Go to `Tools > Board > Board Manager`, search for **ESP32**, and install it.
3. **Install Required Libraries**:
   - Open Arduino IDE and go to `Sketch > Include Library > Manage Libraries`.
   - Search for and install:
     - **Blynk**
     - **PubSubClient**
     - **DHT sensor library**
     - **Adafruit Sensor**
     - **Adafruit ADXL345**
4. **Set Up Blynk**:
   - Download the **Blynk** app.
   - Create a new project and get the **BLYNK_AUTH_TOKEN**.
   - Configure virtual pins for temperature, humidity, vibration, and LED control.
5. **Configure MQTT**:
   - Create an account on an MQTT broker such as **EMQX**.
   - Obtain your MQTT server address, port, username, and password.
   
## Usage
1. **Upload Code to ESP32**:
   - Connect the ESP32 to your PC.
   - Open the Arduino IDE, select the correct board and port.
   - Replace WiFi, Blynk, and MQTT credentials in the code.
   - Upload the code to ESP32.
2. **Run the System**:
   - The ESP32 will connect to WiFi.
   - It will start reading sensor values and sending them to Blynk and MQTT.
   - You can monitor the data on Blynk or subscribe to MQTT topics.
3. **Control the LED**:
   - Use the Blynk app to toggle the LED on/off.
   - The RGB LED changes color based on temperature readings.

## MQTT Topics
| Sensor | Topic |
|--------|-------|
| Temperature | `iot/temp` |
| Humidity | `iot/hum` |
| Vibration X | `iot/vibX` |
| Vibration Y | `iot/vibY` |
| Vibration Z | `iot/vibZ` |
| LM35 Temperature | `iot/lm35_temp` |
| LED Control | `iot/led_control` |

## Troubleshooting
- **WiFi Not Connecting**:
  - Check SSID and password.
  - Ensure the router supports ESP32 connections.
- **Blynk Not Working**:
  - Verify the correct **BLYNK_AUTH_TOKEN**.
  - Check internet connectivity.
- **MQTT Not Connecting**:
  - Ensure the broker address, port, username, and password are correct.
  - Check for firewall issues blocking MQTT ports.
- **Sensors Not Responding**:
  - Check wiring connections.
  - Ensure proper library installation.

## Future Enhancements
- Implement a web-based dashboard for real-time monitoring.
- Add more sensors for environmental monitoring.
- Use machine learning for anomaly detection in sensor data.

## License
This project is open-source and licensed under the **MIT License**.

---
**Developed by:** *Majd Ghazal*

