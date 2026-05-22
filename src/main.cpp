// Simple ESP32 blink sketch
// Uses the built-in LED on most ESP32 boards (GPIO 2)

#include <Arduino.h>

void setup()
{
    Serial.begin(115200);
    pinMode(2, OUTPUT);
    Serial.println("Setup complete");
}

void loop()
{
    Serial.println("LED ON");
    digitalWrite(2, HIGH);
    delay(500);

    Serial.println("LED OFF");
    digitalWrite(2, LOW);
    delay(500);
}
