# Fever Patient Monitoring System: Telegram API & MLX90614 Infrared Sensor
The impact of the COVID-19 pandemic has led to limited interaction and contact among humans. One of the main symptoms experienced by patients infected for the first time is high fever. The Fever Patient Monitoring System is a fever patient monitoring system that uses the ESP32 microcontroller for data processing and communication. The ESP32 will be connected to a temperature sensor used to detect the patient's body temperature. Additionally, the ESP32 will be connected to a web server that serves to notify the patient's condition when the fever is high. The sensors used include the MLX90614 temperature sensor and an IR obstacle sensor for object detection.

## Goals
- Implementing the use of infrared signals to measure temperature through the MLX90614 sensor.
- Creating a patient monitoring system that can contact a supervisor when the patient's condition is not good.

## Concept 
The measurement is carried out by bringing the object (in this experiment, it is the hand) close to the IR Obstacle sensor at a certain distance until the sensor can detect the presence of the object. If the sensor detects the object, the device will display the temperature of that object. If the object has a temperature above the normal limit, it will be categorized as high temperature, and the LED will light up in red. However, if the object has a temperature below the normal limit, it will be categorized as a normal temperature. If the patient's temperature is high, the system will send a message via WiFi and Telegram to the patient's supervisor to check on the patient's condition.

## System Design

## Measurements and Results

## Conclution
