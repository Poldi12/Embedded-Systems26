#include <Arduino.h>
#include "blinky.h"

#define LED_PIN 2

void blinkySetup() {
    pinMode(LED_PIN, OUTPUT);
    Serial.println("Blinky ready");
}

void blinkyLoop() {
    Serial.println("LED ON");
    digitalWrite(LED_PIN, HIGH);
    delay(500);

    Serial.println("LED OFF");
    digitalWrite(LED_PIN, LOW);
    delay(500);
}
