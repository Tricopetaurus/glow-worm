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

  rgb_t hsl_to_rgb(const hsl_t& hsl);

};