#include <array>
#include <hardware/gpio.h>
#include <optional>
#include <pico/stdlib.h>

#include "NeoPixelConnect.h"
#include "color.h"
#include "leds.h"

static std::array<color::rgb_t, 400> color_map;
static void build_map();
static void blink(color::rgb_t clr, int cnt);

static constexpr int LED_PIN = 21;
static constexpr int LED_POWER_EN = 23;
static constexpr int NUM_LEDS = 8;
static constexpr int LED_SEP = 15;
static std::optional<NeoPixelConnect> led_driver;

void leds::init() {
  gpio_init(LED_PIN);
  gpio_init(LED_POWER_EN);
  gpio_set_dir(LED_PIN, true);
  gpio_set_dir(LED_POWER_EN, true);
  gpio_put(LED_POWER_EN, false);

  build_map();

  led_driver.emplace(LED_PIN, NUM_LEDS);
}

void leds::set_color(color::rgb_t clr) {
  gpio_put(LED_POWER_EN, true);
  led_driver->neoPixelFill(clr.r, clr.g, clr.b, true);
}

void leds::loop() {
  gpio_put(LED_POWER_EN, true);

  static size_t idx = 0;
  for (size_t j = 0; j < NUM_LEDS; j++) {
    size_t index = (idx + (LED_SEP * j)) % color_map.size();
    const auto &this_pixel = color_map[index];
    led_driver->neoPixelSetValue(j, this_pixel.r, this_pixel.g, this_pixel.b);
  }
  led_driver->neoPixelShow();

  idx = (idx + 1) % color_map.size();
}

void leds::off() {
  gpio_put(LED_POWER_EN, false);
  led_driver->neoPixelFill(0, 0, 0, true);
}

void leds::blink_bad() {
  blink(color::hsl_to_rgb({5, 1, 0.5}), 2);
}

void leds::blink_good() {
  blink(color::hsl_to_rgb({125, 1, 0.5}), 2);
}

static void build_map() {
  // color::hsl_t purple = {269, 1, 0.4};
  color::hsl_t purple = {240, 1, 0.3};
  color::hsl_t orange = {18, 1, 0.3};

  color::rgb_t start = color::hsl_to_rgb(purple);
  color::rgb_t end = color::hsl_to_rgb(orange);

  float r_step = static_cast<float>(end.r - start.r) / color_map.size();
  float g_step = static_cast<float>(end.g - start.g) / color_map.size();
  float b_step = static_cast<float>(end.b - start.b) / color_map.size();

  float h_step =
      static_cast<float>(orange.h + 360 - purple.h) / color_map.size() * 2;
  float s_step = static_cast<float>(orange.s - purple.s) / color_map.size() * 2;
  float l_step = static_cast<float>(orange.l - purple.l) / color_map.size() * 2;

  for (auto &clr : color_map) {
    clr = {.r = 40, .g = 40, .b = 10};
  }

  for (size_t i = 0; i < color_map.size() / 2; i++) {
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

static void blink(color::rgb_t clr, int cnt) {
  for(int i = 0; i < cnt*2; i++) {
    if (i%2 == 0) {
      led_driver->neoPixelFill(clr.r, clr.g, clr.b, true);
      sleep_ms(400);
    } else {
      led_driver->neoPixelFill(0, 0, 0, true);
      sleep_ms(250);
    }
  }

}

