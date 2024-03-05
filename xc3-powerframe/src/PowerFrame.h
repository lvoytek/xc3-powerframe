#ifndef POWERFRAME_H
#define POWERFRAME_H

#include <Adafruit_NeoPixel.h>
#include <stdint.h>
#include "MillisTimer.h"

#define NEOPIXEL_DATA_PIN 3
#define NEOPIXEL_RING_LENGTH 45
#define NEOPIXEL_DEFAULT_BRIGHTNESS 20
#define NEOPIXEL_SEQUENCE_WAIT_TIME_MS 25

#define STANDARD_BLUE_COLOR 0x18EFFF

/**
 * @brief The available light display modes
 * Mode Descriptions:
 *
 * STANDARD_BLUE:
 * Quickly light up each pixel in sequence with the standard power frame blue color, then keep all pixels fully lit.
 *
 * FLAME_CLOCK:
 * Quickly light up each pixel in sequence with the flame clock red color up to 3/4 full. Then flicker the last few
 * pixels and reduce number lit over time.
 *
 * LIGHTS_OFF:
 * Turn off all lights and maintain this state.
 */
typedef enum { STANDARD_BLUE = 0, FLAME_CLOCK, LIGHTS_OFF } LightMode;

class PowerFrame {
  private:
    // Various state variables needed for keeping track of things in each light mode
    uint8_t currentPixel;
    MillisTimer countdownTimer = MillisTimer(NEOPIXEL_SEQUENCE_WAIT_TIME_MS);
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
    void updateFlameClock();

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