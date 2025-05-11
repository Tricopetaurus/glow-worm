#include "color.h"
#include <cstdint>
#include <cmath>

// This equation was pulled just about directly from Wikipedia,
// Accessed from here around April 2025
// https://en.wikipedia.org/wiki/HSL_and_HSV
color::rgb_t color::hsl_to_rgb(const color::hsl_t& hsl) {
  rgb_t rgb = {0, 0, 0};
  float h = hsl.h;
  float s = hsl.s;
  float l = hsl.l;

  // Range check, return right away if we're not in bounds
  if ( h < 0.0
       || s < 0 || s > 1
       || l < 0 || l > 1) {
    return rgb;
  }
  h = fmodf(h, 360.0);

  // fn = L - amax(-1, min(k-3, 9-k, 1))
  //  k = ( n + H/30) mod 12
  //  a = Sl min (L, 1 - L)
  // (R, G, B) = ( f(0), f(8), f(4) )

  // k is actually k(n), so we need one per color
  typedef struct {
    uint8_t* color_ptr;
    float n;
  } pair_t;

  pair_t rgb_n_map[] = {
      {&rgb.r, 0},
      {&rgb.g, 8},
      {&rgb.b, 4},
  };

  for(auto& p : rgb_n_map) {
    float a = s * std::fmin(l, 1-l);
    float k = fmodf(p.n + h/30, 12);
    float fn = l - a * fmaxf(-1, std::fminf(std::fminf(k-3, 9-k), 1));
    // fn should now be in range [0, 1], scale to a u8.
    *p.color_ptr = static_cast<uint8_t> (fn * UINT8_MAX);
  }

  return rgb;
}
