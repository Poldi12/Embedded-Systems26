// Simple ESP32 blink sketch
// Uses the built-in LED on most ESP32 boards (GPIO 2)

#include <Arduino.h>

void setup()
{
    pinMode(2, OUTPUT);
}

void loop()
{
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);
}
