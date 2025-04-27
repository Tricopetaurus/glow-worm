#include <array>
#include <hardware/gpio.h>
#include <optional>
#include <pico/stdlib.h>

#include "NeoPixelConnect.h"
#include "color.h"
#include "leds.h"
#include  "patterns.h"

static void blink(color::rgb_t clr, int cnt);

static constexpr int LED_PIN = 21;
static constexpr int LED_POWER_EN = 23;
static constexpr int NUM_LEDS = 8;

static std::optional<NeoPixelConnect> led_driver;
static ColorPatterns<NUM_LEDS> patterns;

void leds::init() {
  gpio_init(LED_PIN);
  gpio_init(LED_POWER_EN);
  gpio_set_dir(LED_PIN, true);
  gpio_set_dir(LED_POWER_EN, true);
  gpio_put(LED_POWER_EN, false);

  led_driver.emplace(LED_PIN, NUM_LEDS);
}

void leds::set_color(color::rgb_t clr) {
  gpio_put(LED_POWER_EN, true);
  led_driver->neoPixelFill(clr.r, clr.g, clr.b, true);
}

void leds::loop() {
  gpio_put(LED_POWER_EN, true);
  static size_t idx = 0;
  static std::array<color::rgb_t, NUM_LEDS> frame = {};

  patterns.get_frame(frame, idx);

  for (int i = 0; i < frame.size(); i++) {
    const auto& pixel = frame[i];
    led_driver->neoPixelSetValue(i, pixel.r, pixel.g, pixel.b);
  }

  led_driver->neoPixelShow();

  if (idx < SIZE_MAX) {
    idx += 1;
  } else {
    idx = 0;
  }
}

void leds::off() {
  gpio_put(LED_POWER_EN, false);
  led_driver->neoPixelFill(0, 0, 0, true);
}

void leds::set_pattern_next() {
  patterns.next();
}

void leds::blink_bad() {
  blink(color::hsl_to_rgb({5, 1, 0.5}), 2);
}

void leds::blink_good() {
  blink(color::hsl_to_rgb({125, 1, 0.5}), 2);
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

