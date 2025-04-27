#pragma once

namespace accel {

  enum class CMD {
    NONE,
    TOGGLE_AWAKE,
    NEXT,
  };

  void init();
  void clear_tap();
  CMD wait_for_tap_with_timeout_ms(unsigned int timeout_ms);
  bool test();

};