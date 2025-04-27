#pragma once

#include <cstdint>
#include "color.h"
#include "patterns/orange_purple_wave.h"
#include "patterns/rainbow_wave.h"
#include "patterns/pattern.h"

template <std::size_t NUM_LEDS>
class ColorPatterns {
private:
  int current_pattern_idx;
public:
  static constexpr std::size_t NUM_PATTERNS = 2;
  OrangePurpleWave<NUM_LEDS> orange_purple;
  RainbowWave<NUM_LEDS> rainbow_wave;
  ColorPatterns() : current_pattern_idx(0) {};

  void get_frame(std::array<color::rgb_t, NUM_LEDS>& frame,std::size_t frame_idx) {
    switch(current_pattern_idx) {
      case 0:
        return orange_purple.get_frame(frame, frame_idx);
      case 1:
        return rainbow_wave.get_frame(frame, frame_idx);
    }
  }

  void next() {
    this->current_pattern_idx += 1;
    this->current_pattern_idx %= NUM_PATTERNS;
  }
};