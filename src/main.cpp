#include <array>

#include <pico.h>
#include <pico/time.h>

#include "accelerometer.h"
#include "lis3dh.h"
#include "usb.h"
#include "leds.h"

int main() {
  bool leds_on = false;
  accel::init();
  usb::init();
  leds::init();

  for(;;) {
    usb::loop();

#if 1
    switch(accel::wait_for_tap_with_timeout_ms(25)) {
    case accel::CMD::NONE:
      break;
    case accel::CMD::TOGGLE_AWAKE:
      leds_on = !leds_on;
      break;
    case accel::CMD::NEXT:
      leds::set_pattern_next();
      break;
    }

    if (leds_on) {
      leds::loop();
    } else {
      leds::off();
    }

  }
#else
    auto tap_reason = accel::wait_for_tap_with_timeout_ms(25);
    if (tap_reason == accel::CMD::NONE)
      continue;

    leds::set_color({0,0,0});
    sleep_ms(50);
    if (tap_reason == accel::CMD::TOGGLE_AWAKE) {
      leds::set_color({255, 0, 0});
    } else if (tap_reason == accel::CMD::NEXT) {
      leds::set_color({0, 255, 0});
    }
  }

#endif
}
