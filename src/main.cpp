#include <WiFi.h>
#include <HTTPClient.h>

const char *ssid = "Galaxy S20 FE 5G A68D";
const char *password = "tnan5726";
const char *serverName = "http://192.168.95.134:5000/data";

void setup()
{

  Serial.begin(115200);

  // Start WiFi
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

void loop()
{

  Serial.println("================================");

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

    // ====================================
    // CREATE JSON
    // ====================================

    String json = "{";
    json += "\"temperature\":" + String(temperature, 1) + ",";
    json += "\"humidity\":" + String(humidity, 1) + ",";
    json += "\"pressure\":" + String(pressure, 1);
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