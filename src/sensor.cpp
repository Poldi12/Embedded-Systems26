#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_SSD1306.h>
#include "sensor.h"

#include <WiFi.h>
#include <HTTPClient.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR     0x3C

static Adafruit_BME280 bme;
static Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Wifi params
const char* ssid = "Galaxy S20 FE 5G A68D"; // your_wifi_ssid
const char* password = "tnan5726";
const char* serverName = "http://192.168.95.134:5000/data"; // your_server_endpoint

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

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("SSD1306 not found at 0x3C. Check wiring.");
        while (1) delay(10);
    }
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("BME280 ready");
    display.display();

    //setup fan
    pinMode(18, OUTPUT);
    digitalWrite(18, LOW); // Start with fan off

    //Wifi setup
    WiFi.mode(WIFI_STA);

  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");

  // Wait until connected
  while (WiFi.status() != WL_CONNECTED)
  {

    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected!");

  Serial.print("ESP32 IP Address: ");
  Serial.println(WiFi.localIP());
}

void sensorLoop() {
    float temp     = bme.readTemperature();
    float hum      = bme.readHumidity();
    float pres     = bme.readPressure() / 100.0F;
    float alt      = bme.readAltitude(1013.25);

    Serial.printf("Temperature: %.2f °C\n", temp);
    Serial.printf("Humidity:    %.2f %%\n",  hum);
    Serial.printf("Pressure:    %.2f hPa\n", pres);
    Serial.printf("Altitude:    %.2f m\n",   alt);
    Serial.println("-------------------------------");

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.printf("Temp:  %.1f C\n",   temp);
    display.printf("Hum:   %.1f %%\n",  hum);
    display.printf("Pres:  %.1f hPa\n", pres);
    display.printf("Alt:   %.1f m\n",   alt);
    display.display();

    //fan logic
    if (temp > 29.0) {
        digitalWrite(18, HIGH); // Turn on fan
    } else {
        digitalWrite(18, LOW);  // Turn off fan
    }

    //Wifi logic
    //###########
    // Check WiFi connection
    if (WiFi.status() == WL_CONNECTED)
    {

        HTTPClient http;

        // Flask server address
        http.begin(serverName);

        // JSON format
        http.addHeader("Content-Type", "application/json");

        // ====================================
        // DUMMY SENSOR VALUES
        // ====================================

        /*
        static float temperature = 25.0;
        static float humidity = 50.0;
        static float pressure = 1000.0;

        // Smooth random changes
        temperature += random(-20, 21) / 10.0;
        humidity += random(-30, 31) / 10.0;
        pressure += random(-10, 11) / 10.0;

        // Keep values realistic
        temperature = constrain(temperature, 20.0, 35.0);
        humidity = constrain(humidity, 40.0, 80.0);
        pressure = constrain(pressure, 980.0, 1030.0);

        */
        // ====================================
        // CREATE JSON
        // ====================================

        String json = "{";
        json += "\"temperature\":" + String(bme.readTemperature(), 1) + ",";
        json += "\"humidity\":" + String(bme.readHumidity(), 1) + ",";
        json += "\"pressure\":" + String(bme.readPressure() / 100.0F, 1);
        json += "}";

        // Print to Serial Monitor
        Serial.println("Sending:");
        Serial.println(json);

        // ====================================
        // SEND TO FLASK
        // ====================================

        int httpResponseCode = http.POST(json);

        Serial.print("HTTP Response Code: ");
        Serial.println(httpResponseCode);

        http.end();
    }
    else
    {

        Serial.println("WiFi NOT connected");
    }

    delay(1000);
}
