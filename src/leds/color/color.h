/**
 * Structures to handle colors in HSL and RGB format for convenience.
 * Also handles converting from HSL to RGB.
 */
#pragma once

#include <cstdint>

namespace color {

  typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
  } rgb_t;

  typedef struct {
    float h;
    float s;
    float l;
  } hsl_t;

  /**
   * Returns an rgb_t given an hsl_t
   * Will return {0, 0, 0} if invalid inputs are provided.
   */
  rgb_t hsl_to_rgb(const hsl_t& hsl);

};