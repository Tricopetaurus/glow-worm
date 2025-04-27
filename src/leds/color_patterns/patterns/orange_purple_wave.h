#pragma once

#include <array>
#include <cstdint>
#include "pattern.h"


template <std::size_t NUM_LEDS>
class OrangePurpleWave : public ColorPattern<400, NUM_LEDS> {
private:
  static constexpr int LED_SEP = 15;
  static constexpr std::size_t NUM_COLORS = 400;
public:
  OrangePurpleWave() {
    color::hsl_t purple = {240, 1, 0.3};
    color::hsl_t orange = {18, 1, 0.3};

    float h_step =
        static_cast<float>(orange.h + 360 - purple.h) / NUM_COLORS * 2;
    float s_step = static_cast<float>(orange.s - purple.s) / NUM_COLORS * 2;
    float l_step = static_cast<float>(orange.l - purple.l) / NUM_COLORS * 2;

    for (std::size_t i = 0; i < NUM_COLORS / 2; i++) {
      auto this_rgb =
          color::hsl_to_rgb({purple.h + (i * h_step), purple.s + (i * s_step),
                             purple.l + (i * l_step)});
      this->_color_array[i] = this_rgb;
      this->_color_array[NUM_COLORS - i - 1] = this_rgb;
    }

  }

  virtual void get_frame(std::array<color::rgb_t, NUM_LEDS>& frame ,std::size_t frame_idx) {
    frame_idx %= NUM_COLORS;
    for (std::size_t j = 0; j < NUM_LEDS; j++) {
      std::size_t index = (frame_idx + (LED_SEP * j)) % NUM_COLORS;
      const auto &this_pixel = this->_color_array[index];
      frame[j] = this_pixel;
    }
  }
};