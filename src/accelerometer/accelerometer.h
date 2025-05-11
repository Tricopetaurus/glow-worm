/**
 * Accelerometer Module
 * This configures tap events around the LIS3DH accelerometer
 * Like all loop modules, we have an init and a loop that should be called externally
 */
#pragma once

namespace accel {

  enum class CMD {
    NONE,
    TOGGLE_AWAKE,
    NEXT,
  };

  /**
   * Initialize the accelerometer module
   * Must be called before loop
   */
  void init();

  /**
   * Periodic loop that advances our tap detection state machine.
   * Returns the most recently detected command, or NONE if none has occurred.
   */
  CMD loop();

  /**
   * Put the RP2040 in to a deep sleep state until an interrupt event occurs.
   * As-configured, this means the MCU will be in a sleep state until a tap
   * event occurs
   */
  void sleep_until_tap();


  /**
   * Unit tests for the accelerometer.
   * Just used to verify sanity, no longer needed
   */
  bool test();
};