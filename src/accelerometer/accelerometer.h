#pragma once

#include <stdint.h>

namespace accel {
  void init();
  void clear_tap();
  uint8_t wait_for_tap();
  uint8_t wait_for_tap_with_timeout_ms(unsigned int timeout_ms);
  bool test();
};