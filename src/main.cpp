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
    if(accel::wait_for_tap_with_timeout_ms(25)) {
      leds_on = !leds_on;
    }

    if (leds_on) {
      leds::loop();
    } else {
      leds::off();
    }

  }
#else
    uint8_t tap_reason = accel::wait_for_tap_with_timeout_ms(25);
    if (tap_reason == 0)
      continue;

    leds::set_color({0,0,0});
    sleep_ms(50);
    if (tap_reason & lis::TAP_SRC_X) {
      leds::set_color({255, 0, 0});
    } else if (tap_reason & lis::TAP_SRC_Y) {
      leds::set_color({0, 255, 0});
    } else if (tap_reason & lis::TAP_SRC_Z) {
      leds::set_color({0, 0, 255});
    }
  }

#endif
}
