#include <Arduino.h>
#include "blinky.h"
#include "sensor.h"

void setup() {
    Serial.begin(9600); //115200 modern default, works with blinky

    // -- comment in one block --

    //blinkySetup();
    sensorSetup();
}

void loop() {
    // -- comment in one block --

    // blinkyLoop();
    sensorLoop();
}
