#include <Arduino.h>
#include "blinky.h"
#include "sensor.h"

void setup() {
    Serial.begin(115200);

    // -- comment in one block --

    //blinkySetup();
    sensorSetup();
}

void loop() {
    // -- comment in one block --

    // blinkyLoop();
    sensorLoop();
}
