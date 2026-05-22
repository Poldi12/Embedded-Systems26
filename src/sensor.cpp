#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include "sensor.h"

static Adafruit_BME280 bme;

void sensorSetup() {
    Wire.setTimeOut(2000);  // 1s timeout so bme.begin() doesn't hang forever
    Wire.begin(21, 22);
    Serial.println("First try to initialize I2C bus\n");
    delay(100);

    if (!bme.begin(0x76)) {
        Serial.println("BME280 not found at 0x76, trying 0x77...");
        if (!bme.begin(0x77)) {
            Serial.println("Could not find BME280 sensor. Check wiring.");
            while (1) delay(10);
        }
    }
    Serial.println("BME280 found. Reading sensor data:");
    Serial.println("-------------------------------");
}

void sensorLoop() {
    Serial.printf("Temperature: %.2f °C\n", bme.readTemperature());
    Serial.printf("Humidity:    %.2f %%\n", bme.readHumidity());
    Serial.printf("Pressure:    %.2f hPa\n", bme.readPressure() / 100.0F);
    Serial.printf("Altitude:    %.2f m\n", bme.readAltitude(1013.25));
    Serial.println("-------------------------------");
    delay(2000);
}
