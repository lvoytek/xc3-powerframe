#include "PowerFrame.h"

PowerFrame powerFrame = PowerFrame();

void setup() {
  powerFrame.init();
}

void loop() {
    powerFrame.update();
    delay(1);
}