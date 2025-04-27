#pragma once

#include <array>
#include <cstdint>
#include "color.h"

template <std::size_t NUM_COLORS, std::size_t NUM_LEDS>
class ColorPattern {
protected:
  std::array<color::rgb_t, NUM_COLORS> _color_array;
public:
  ColorPattern() {
    this->_color_array.fill({0,0,0});
  }
  virtual void get_frame(std::array<color::rgb_t, NUM_LEDS>& frame ,std::size_t frame_idx) = 0;
};
