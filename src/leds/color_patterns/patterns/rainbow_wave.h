#pragma once

#include <array>
#include <cstdint>
#include "pattern.h"

template <std::size_t NUM_LEDS>
class RainbowWave : public ColorPattern<360*10, NUM_LEDS> {
private:
  static constexpr int LED_SEP = 360 / NUM_LEDS;
  static constexpr std::size_t NUM_COLORS = 360;
public:
  RainbowWave() {
    // This one is so trivial we could do it live with get_frame...
    static float h_step = 360.0 / NUM_COLORS;
    for (int i = 0; i < NUM_COLORS; i++) {
      this->_color_array[i] = color::hsl_to_rgb({h_step*i, 1, 0.5});
    }
  }

  virtual void get_frame(std::array<color::rgb_t, NUM_LEDS>& frame ,std::size_t frame_idx) {
    frame_idx *= 5;
    frame_idx %= NUM_COLORS;
    for (std::size_t j = 0; j < NUM_LEDS; j++) {
      std::size_t index = (frame_idx + (j * LED_SEP)) % NUM_COLORS;
      const auto &this_pixel = this->_color_array[index];
      frame[j] = this_pixel;
    }
  }
};