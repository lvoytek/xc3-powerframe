#ifndef POWERFRAME_H
#define POWERFRAME_H

#include "MillisTimer.h"
#include <Adafruit_NeoPixel.h>
#include <stdint.h>

#define NEOPIXEL_DATA_PIN 3
#define NEOPIXEL_RING_LENGTH 45
#define NEOPIXEL_DEFAULT_BRIGHTNESS 20
#define NEOPIXEL_SEQUENCE_WAIT_TIME_MS 25

#define STANDARD_BLUE_COLOR 0x18EFFF
#define FLAME_CLOCK_KEVES_BASE_COLOR 0xE1FDF9
#define FLAME_CLOCK_KEVES_EDGE_COLOR 0x57E0FD
#define FLAME_CLOCK_AGNUS_BASE_COLOR 0xFEFEE0
#define FLAME_CLOCK_AGNUS_EDGE_COLOR 0x93ED2F

#define FLAME_CLOCK_START_LENGTH NEOPIXEL_RING_LENGTH * 3 / 4

#define SPIN_SEQUENCE_SPINNER_LENGTH 5

/**
 * @brief The available light display modes
 * Mode Descriptions:
 *
 * STANDARD_BLUE:
 * Quickly light up each pixel in sequence with the standard power frame blue color, then keep all pixels fully lit.
 *
 * FLAME_CLOCK_KEVES/FLAME_CLOCK_AGNUS:
 * Quickly light up each pixel in sequence with the flame clock color up to 3/4 full. Then flicker the last few
 * pixels and reduce number lit over time.
 *
 * SPIN_SEQUENCE
 * Rotate a group of lighter pixels around the ring infinitely.
 *
 * LIGHTS_OFF:
 * Turn off all lights and maintain this state.
 */
typedef enum { STANDARD_BLUE = 0, FLAME_CLOCK_KEVES, FLAME_CLOCK_AGNUS, SPIN_SEQUENCE, LIGHTS_OFF } LightMode;

typedef enum { FROZEN = 0, LOADING, RUNNING, INCREASING, DECREASING } PowerFrameState;

class PowerFrame {
  private:
    // Various state variables needed for keeping track of things in each light mode
    uint8_t currentPixel = 0;
    MillisTimer countdownTimer = MillisTimer(NEOPIXEL_SEQUENCE_WAIT_TIME_MS);
    PowerFrameState currentState = FROZEN;

    /**
     * @brief Update state of lighting up pixels in sequence over time.
     *
     * @param maxPixels The maximum number of pixels to light up
     * @param color The color to use for the pixels
     * @return true The maximum number of pixels just finished turning on
     */
    bool updateSequenceLoad(uint8_t maxPixels, uint32_t color);

  protected:
    /**
     * The NeoPixel strip driver used to manage the lights on the ring
     * Assuming standard NeoPixel hardware with 800 KHz bitstream wired for GRB
     */
    Adafruit_NeoPixel lightRing = Adafruit_NeoPixel(NEOPIXEL_RING_LENGTH, NEOPIXEL_DATA_PIN, NEO_GRB + NEO_KHZ800);

    // The current light display mode
    LightMode currentMode;

    /**
     * @brief Turn off the light ring.
     * @note This is used as the initial setting for all modes that start in the off state.
     */
    void initLightsOff();

    /**
     * @brief Initialize the standard blue mode.
     */
    void updateStandardBlue();

    /**
     * @brief Update the state and look of the flame clock.
     */
    void updateFlameClock(uint32_t baseColor, uint32_t edgeColor);

    /**
     * @brief Update the state of the spin sequence.
     */
    void updateSpinSequence();

  public:
    /**
     * @brief Construct a new Power Frame object with an initial mode provided.
     *
     * @param startMode The light mode to use after running init
     */
    PowerFrame(LightMode startMode);

    /**
     * @brief Construct a new Power Frame object with solid blue as it's initial mode.
     */
    PowerFrame() : PowerFrame(STANDARD_BLUE){};

    /**
     * @brief Initialize the neopixel ring and display the starting light mode.
     */
    void init();

    /**
     * @brief Swap to the next light mode and initialize.
     */
    void nextMode();

    /**
     * @brief Select a new light mode and initialize it.
     *
     * @param nextMode The next light mode to run
     */
    void selectMode(LightMode nextMode);

    /**
     * @brief Update the lights based on their mode.
     * @note Run this function via loop() in Arduino.
     */
    void update();
};

#endif