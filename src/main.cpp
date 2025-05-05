#include <pico/time.h>

#include "accelerometer.h"
#include "usb.h"
#include "leds.h"

int main() {
  bool leds_on = false;
  accel::init();
  usb::init();
  leds::init();

  for(;;) {
    usb::loop();
    auto tap_event = accel::loop();

    if (tap_event == accel::CMD::TOGGLE_AWAKE) {
      leds_on = !leds_on;
    }

    if (leds_on) {
      if (tap_event == accel::CMD::NEXT) {
        leds::set_pattern_next();
      }
      leds::loop();
    } else {
      leds::off();
      accel::sleep_until_tap();
    }
    sleep_ms(1);
  }
}
