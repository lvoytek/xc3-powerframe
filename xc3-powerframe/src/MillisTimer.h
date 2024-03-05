/**
 * MillisTimer
 * Keeps track of time ellapsed between timer calls
 */

#ifndef MILLISTIMER_H
#define MILLISTIMER_H

#include <Arduino.h>

class MillisTimer {
  protected:
    // The last logged time value
    unsigned long timer;

    // The period of time to wait before a timer reset success
    unsigned long wait;

  public:
    /**
     * @brief Create a countup timer to check elapsed time.
     */
    MillisTimer() {
        this->timer = 0;
        this->wait = -1;
    }

    /**
     * @brief Create a countdown timer that can reset after the wait interval.
     */
    MillisTimer(unsigned long waitTime) {
        this->timer = 0;
        this->wait = waitTime;
    }

    /**
     * @brief Determine if the timer is up based on the current time and the necessary wait time.
     * @return True if the wait time has passed
     */
    boolean test() {
        unsigned long now = millis();

        if (now > this->timer + this->wait || now < this->timer) {
            this->timer = now;
            return true;
        }

        return false;
    }

    /**
     * @brief Get the time ellapsed since last reset.
     *
     * @return The time ellapsed in milliseconds
     */
    unsigned long ellapsed() {
        unsigned long now = millis();

        if (now < this->timer)
            return 4294967295 - this->timer + now;

        return now - this->timer;
    }

    /**
     * @brief Reset the countup timer.
     */
    void reset() { this->timer = millis(); }

    /**
     * @brief Update the wait time for countdown.
     */
    void updateWaitTime(unsigned long waitTime) {
        this->timer = 0;
        this->wait = waitTime;
    }
};

#endif