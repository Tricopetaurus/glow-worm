#include <pico/time.h>

#include "accelerometer.h"
#include "usb.h"
#include "leds.h"

int main() {
  // Initialize our three sub-loops
  // Also, let's keep track of our "leds_on" state here.
  bool leds_on = false;
  accel::init();
  usb::init();
  leds::init();

  // This for loop is FORever.
  // We'll just keep doing this loop until the device loses power or restarts
  for(;;) {

    // First, process any USB events if they've happened.
    // This is really just for debug-access to reboot or reprogram more easily.
    // In fact, if no usb device is plugged in this should do nothing.
    usb::loop();

    // Next, check if any accelerometer tap events have occurred.
    // `auto` here lets me avoid typing out accel::CMD
    auto tap_event = accel::loop();

    // If the toggle command was sent (double-tap), toggle the leds_on boolean
    if (tap_event == accel::CMD::TOGGLE_AWAKE) {
      leds_on = !leds_on;
    }

    // The rest of the processing and what we do is determined by whether or not
    // the leds should be on.
    if (leds_on) {
      // If a single-tap happens, show the next LED pattern.
      // We only do this if the LEDs are already on.
      if (tap_event == accel::CMD::NEXT) {
        leds::set_pattern_next();
      }
      // Calling leds::loop() updates the current animation
      leds::loop();
    } else {
      // If the LEDs are supposed to be off, shut them down.
      // Also, just hit deep sleep until the accelerometer detects a new command
      leds::off();
      accel::sleep_until_tap();
    }
    // This sleep is somewhat arbitrary.
    // We don't need to do anything any faster than this, so take some time off.
    sleep_ms(1);
  }
}
