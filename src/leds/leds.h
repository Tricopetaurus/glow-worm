/**
 * LEDs Module
 *
 * This module handles control over the LEDs
 * This includes turning them on, off, and changing their pattern.
 * Patterns are all pre-compiled and defined under the patterns module.
 */
#pragma once

#include "color.h"

namespace leds {

  /**
   * Initialize the LED module
   */
  void init();

  /**
   * Periodic loop for the LED module
   * Should be called regularly
   * This will turn on the LEDs and animate the current pattern
   * if enough time has passed for a new frame.
   */
  void loop();

  /**
   * Turn off the LEDs.
   * This will keep the current pattern and frame where they are
   */
  void off();

  /**
   * Set the LED pattern to the next available pattern.
   */
  void set_pattern_next();
};