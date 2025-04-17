#include <array>

#include <pico.h>
#include <pico/time.h>

#include "NeoPixelConnect.h"
#include "color.h"

std::array<color::rgb_t, 400> color_map;

void build_map() {
  // color::hsl_t purple = {269, 1, 0.4};
  color::hsl_t purple = {240, 1, 0.3};
  color::hsl_t orange = {18, 1, 0.3};

  color::rgb_t start = color::hsl_to_rgb(purple);
  color::rgb_t end = color::hsl_to_rgb(orange);

  float r_step = static_cast<float>(end.r - start.r) / color_map.size();
  float g_step = static_cast<float>(end.g - start.g) / color_map.size();
  float b_step = static_cast<float>(end.b - start.b) / color_map.size();

  float h_step = static_cast<float>(orange.h + 360 - purple.h) / color_map.size() * 2;
  float s_step = static_cast<float>(orange.s - purple.s) / color_map.size() * 2;
  float l_step = static_cast<float>(orange.l - purple.l) / color_map.size() * 2;

  for (auto& clr : color_map) {
    clr = {.r = 40, .g=40, .b=10};
  }

  for (size_t i = 0; i < color_map.size()/2; i++) {
#if 0
    color_map[i] = {
        static_cast<uint8_t> (start.r + (i*r_step)),
        static_cast<uint8_t> (start.g + (i*g_step)),
        static_cast<uint8_t> (start.b + (i*b_step)),
    };
#else
    auto this_rgb =
        color::hsl_to_rgb({purple.h + (i * h_step), purple.s + (i * s_step),
                           purple.l + (i * l_step)});
    color_map[i] = this_rgb;
    color_map[color_map.size() - i - 1] = this_rgb;
#endif
  }
}

int main() {
  constexpr int LED_PIN = 21;
  // constexpr int LED_PIN = 4;
  constexpr int LED_POWER_EN = 23;
  constexpr int NUM_LEDS = 8;

  // Setup
  gpio_init(LED_PIN);
  gpio_init(LED_POWER_EN);
  gpio_set_dir(LED_PIN, true);
  gpio_set_dir(LED_POWER_EN, true);
  gpio_put(LED_POWER_EN, true);

  build_map();

  NeoPixelConnect led_driver{LED_PIN, NUM_LEDS};
  sleep_ms(50);
  const int sep = 15;
  // Run forever
    for (;;) {
#if 1
      for (size_t i = 0; i < color_map.size(); i++) {
        for (size_t j = 0; j < NUM_LEDS; j++) {
          size_t index = (i + (sep*j)) % color_map.size();
          const auto& this_pixel = color_map[index];
          led_driver.neoPixelSetValue(j, this_pixel.r, this_pixel.g, this_pixel.b);
        }
        led_driver.neoPixelShow();
        sleep_ms(25);
      }
#else
      for (const auto &c : color_map) {
        led_driver.neoPixelFill(c.r, c.g, c.b);
        sleep_ms(25);
      }
      sleep_ms(500);
      for (auto it = color_map.rbegin(); it != color_map.rend(); ++it) {
        led_driver.neoPixelFill(it->r, it->g, it->b);
        sleep_ms(25);
      }
      sleep_ms(500);
#endif
    }
}
