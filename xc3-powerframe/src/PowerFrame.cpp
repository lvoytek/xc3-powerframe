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
        this->currentMode = FLAME_CLOCK_KEVES;
        break;
    case FLAME_CLOCK_KEVES:
        this->currentMode = FLAME_CLOCK_AGNUS;
        break;
    case FLAME_CLOCK_AGNUS:
        this->currentMode = SPIN_SEQUENCE;
        break;
    case SPIN_SEQUENCE:
        this->currentMode = LIGHTS_OFF;
        break;
    default:
        this->currentMode = STANDARD_BLUE;
    }

    this->selectMode(this->currentMode);
}

void PowerFrame::selectMode(LightMode nextMode) {
    this->currentMode = nextMode;
    this->initLightsOff();

    switch (this->currentMode) {
    case LIGHTS_OFF:
        this->currentState = FROZEN;
        break;
    default:
        this->currentState = LOADING;
    }
}

void PowerFrame::update() {
    switch (this->currentMode) {
    case STANDARD_BLUE:
        this->updateStandardBlue();
        break;
    case FLAME_CLOCK_KEVES:
        this->updateFlameClock(FLAME_CLOCK_KEVES_BASE_COLOR, FLAME_CLOCK_KEVES_EDGE_COLOR);
        break;
    case FLAME_CLOCK_AGNUS:
        this->updateFlameClock(FLAME_CLOCK_AGNUS_BASE_COLOR, FLAME_CLOCK_AGNUS_EDGE_COLOR);
        break;
    case SPIN_SEQUENCE:
        this->updateSpinSequence();
        break;
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
    if (this->currentState == LOADING) {
        if (this->updateSequenceLoad(NEOPIXEL_RING_LENGTH, STANDARD_BLUE_COLOR)) {
            this->currentState = FROZEN;
        }
    }
}

void PowerFrame::updateFlameClock(uint32_t baseColor, uint32_t edgeColor) {
    if (this->currentState == LOADING) {
        if (this->updateSequenceLoad(FLAME_CLOCK_START_LENGTH, edgeColor)) {
            this->currentState = FROZEN;
        }
    }
}

void PowerFrame::updateSpinSequence() {
    if (this->currentState == LOADING) {
        if (this->updateSequenceLoad(NEOPIXEL_RING_LENGTH, STANDARD_BLUE_COLOR)) {
            this->currentState = RUNNING;

            for (uint8_t i = 0; i < SPIN_SEQUENCE_SPINNER_LENGTH; i++) {
                this->lightRing.setPixelColor(i, FLAME_CLOCK_KEVES_BASE_COLOR);
            }

            this->currentPixel = 0;
            this->lightRing.show();
        }
    } else if (this->currentState == RUNNING) {
        if (this->countdownTimer.test()) {
            this->lightRing.setPixelColor((this->currentPixel - 1) % NEOPIXEL_RING_LENGTH, STANDARD_BLUE_COLOR);
            this->lightRing.setPixelColor((this->currentPixel + SPIN_SEQUENCE_SPINNER_LENGTH) % NEOPIXEL_RING_LENGTH,
                                          FLAME_CLOCK_KEVES_BASE_COLOR);
            this->currentPixel = (this->currentPixel + 1) % NEOPIXEL_RING_LENGTH;
            this->lightRing.show();
        }
    }
}

bool PowerFrame::updateSequenceLoad(uint8_t maxPixels, uint32_t color) {
    if (this->currentPixel < maxPixels) {
        if (this->countdownTimer.test()) {
            this->lightRing.setPixelColor(this->currentPixel++, color);
            this->lightRing.show();
        }

        return false;
    }

    return true;
}