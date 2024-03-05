#include "PowerFrame.h"

PowerFrame::PowerFrame(LightMode startMode) { this->currentMode = startMode; }

void PowerFrame::init() {
    this->lightRing.begin();
    this->lightRing.setBrightness(NEOPIXEL_DEFAULT_BRIGHTNESS);
    this->lightRing.show();

    this->selectMode(this->currentMode);
}

void PowerFrame::nextMode() {
    switch (this->currentMode) {
    case STANDARD_BLUE:
        this->currentMode = FLAME_CLOCK;
        break;
    case FLAME_CLOCK:
        this->currentMode = LIGHTS_OFF;
        break;
    default:
        this->currentMode = STANDARD_BLUE;
    }

    this->selectMode(this->currentMode);
}

void PowerFrame::selectMode(LightMode nextMode) {
    this->currentMode = nextMode;

    switch (this->currentMode) {
    default:
        this->initLightsOff();
    }
}

void PowerFrame::update() {
    switch (this->currentMode) {
    case STANDARD_BLUE:
        this->updateStandardBlue();
        break;
    case FLAME_CLOCK:
        this->updateFlameClock();
    default:
        break;
    }
}

void PowerFrame::initLightsOff() {
    this->currentPixel = 0;
    this->countdownTimer.updateWaitTime(NEOPIXEL_SEQUENCE_WAIT_TIME_MS);

    for (uint8_t i = 0; i < NEOPIXEL_RING_LENGTH; i++) {
        this->lightRing.setPixelColor(i, 0);
    }
    this->lightRing.show();
}

void PowerFrame::updateStandardBlue() {
    if (this->currentPixel < NEOPIXEL_RING_LENGTH) {
        if (this->countdownTimer.test()) {
            this->lightRing.setPixelColor(this->currentPixel++, STANDARD_BLUE_COLOR);
            this->lightRing.show();
        }
    }
}

void PowerFrame::updateFlameClock() {}