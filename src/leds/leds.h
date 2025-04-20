#pragma once

#include "color.h"

namespace leds {

  void init();
  void loop();
  void off();
  void blink_good();
  void blink_bad();
  void set_color(color::rgb_t clr);
};