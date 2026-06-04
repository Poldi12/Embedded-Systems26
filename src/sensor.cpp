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
const char* ssid = "Galaxy S20 FE 5G A68D"; // wifi ssid
const char* password = "tnan5726";
const char* serverName = "http://192.168.95.134:5000/data"; // server endpoint

void sensorSetup() {
    Wire.setTimeOut(2000);  // 1s timeout so bme.begin() doesn't hang forever
    Wire.begin(21, 22);
    Serial.println("First try to initialize I2C bus\n");
    delay(100);

    if (!bme.begin(0x76)) {
        Serial.println("BME280 not found at 0x76, trying 0x77...");
        if (!bme.begin(0x77)) {
            Serial.println("Could not find BME280 sensor.");
            while (1) delay(10);
        }
    }
    Serial.println("BME280 found. Reading sensor data:");
    Serial.println("-------------------------------");

    if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
        Serial.println("SSD1306 not found at 0x3C.");
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

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("Connecting to WiFi...");

    display.clearDisplay();
    display.setCursor(0, 0);
    display.println("Connecting WiFi...");
    display.display();

    int wifiTimeout = 4;
    while (WiFi.status() != WL_CONNECTED && wifiTimeout-- > 0) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi Connected!");
        Serial.print("ESP32 IP: ");
        Serial.println(WiFi.localIP());
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("WiFi Connected!");
        display.println(WiFi.localIP().toString());
        display.display();
        delay(5000);
    } else {
        Serial.println("WiFi not available, continuing without.");
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("No WiFi, continuing");
        display.display();
        delay(3000);
    }
}

void sensorLoop() {
    static int loopCount = 0;
    static String serverStatus = "--";
    loopCount++;

    float temp     = bme.readTemperature();
    float hum      = bme.readHumidity();
    float pres     = bme.readPressure() / 100.0F;
    float alt      = bme.readAltitude(1013.25);

    Serial.printf("Temperature: %.2f °C\n", temp);
    Serial.printf("Humidity:    %.2f %%\n",  hum);
    Serial.printf("Pressure:    %.2f hPa\n", pres);
    Serial.printf("Altitude:    %.2f m\n",   alt);
    Serial.println("-------------------------------");

    if (loopCount % 10 == 0) {
        if (WiFi.status() == WL_CONNECTED) {
            HTTPClient http;
            http.begin(serverName);
            http.setTimeout(2000);
            http.addHeader("Content-Type", "application/json");

            String json = "{";
            json += "\"temperature\":" + String(temp, 1) + ",";
            json += "\"humidity\":" + String(hum, 1) + ",";
            json += "\"altitude\":" + String(alt, 1) + ",";
            json += "\"pressure\":" + String(pres, 1);
            json += "}";

            Serial.println("Sending: " + json);
            int httpResponseCode = http.POST(json);
            Serial.printf("HTTP Response Code: %d\n", httpResponseCode);
            http.end();

            serverStatus = (httpResponseCode > 0) ? "OK" : "FAIL";
        } else {
            Serial.println("WiFi NOT connected");
            serverStatus = "Not connected";
        }
    }

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.printf("Temp:  %.1f C\n",   temp);
    display.printf("Hum:   %.1f %%\n",  hum);
    display.printf("Pres:  %.1f hPa\n", pres);
    display.printf("Alt:   %.1f m\n",   alt);
    display.printf("Srv:   %s\n", serverStatus.c_str());
    display.display();

    if (temp > 29.0) {
        digitalWrite(18, HIGH);
    } else if (temp < 28.0) {
        digitalWrite(18, LOW);
    }

    delay(1000);
}
