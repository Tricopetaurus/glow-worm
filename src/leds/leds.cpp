#include <array>
#include <hardware/gpio.h>
#include <optional>
#include <pico/stdlib.h>

#include "NeoPixelConnect.h"
#include "color.h"
#include "leds.h"
#include "patterns.h"


// ================================
//  PRIVATE VARIABLES and CONSTANTS
// ================================
static constexpr int LED_PIN = 21;
static constexpr int LED_POWER_EN = 23;
static constexpr int NUM_LEDS = 7; // TODO: I think it's actually 7.

// DEV NOTE:
// led_driver's constructor has a side effect of turning the LEDs on.
// I don't want that to happen yet, but I also want the object to be
// "globally" accessible by any other function in this file.
// So, the trick is to defer construction until the init() function is called.
// A way I like to do this is with std::optional.
// This is a container that reserves ahead of time the space needed to hold
// the object. The object (led_driver, of type NeoPixelConnect) won't be
// constructed until the .emplace function is called.
static std::optional<NeoPixelConnect> led_driver;
static ColorPatterns<NUM_LEDS> patterns;

// ================================
//  PUBLIC FUNCTION DEFINITIONS
// ================================
void leds::init() {
  // Initialize and set our GPIO to be outputs
  gpio_init(LED_PIN);
  gpio_init(LED_POWER_EN);
  gpio_set_dir(LED_PIN, true);
  gpio_set_dir(LED_POWER_EN, true);
  // Also, start with the power to the LEDs turned off.
  gpio_put(LED_POWER_EN, false);

  // Finally, perform our deferred construction of the led driver.
  // Before this line, led_driver isn't in a valid state for things to call it.
  led_driver.emplace(LED_PIN, NUM_LEDS);
}

void leds::loop() {
  // This is a buffer to store the current frame that we'll display on the LEDs
  static std::array<color::rgb_t, NUM_LEDS> frame = {};
  // This was arbitrarily chosen to make patterns look nice.
  // A smaller value will make the animations move faster.
  static constexpr uint32_t MS_PER_FRAME = 25;
  // Keep track of when the last frame occurred so we
  // only push updates at a MS_PER_FRAME rate.
  static uint32_t last_frame_ms = 0;
  // Index for the frame in the current pattern.
  // Imagine a reel of tape. This index points to the frame on the reel
  // that we want to display right now.
  static size_t frame_idx = 0;

  // Turn on the LEDs
  gpio_put(LED_POWER_EN, true);

  // Figure out the current time so we know if we should update the frame index
  uint32_t current_ms =  to_ms_since_boot(get_absolute_time());
  if (current_ms - last_frame_ms > MS_PER_FRAME) {
    last_frame_ms = current_ms;

    // Get frame number "frame_index" and store it in our buffer, "frame"
    patterns.get_frame(frame, frame_idx);

    // Now, iterate over the frame buffer and put each pixel's
    // color value on the led strip
    for (int i = 0; i < frame.size(); i++) {
      const auto &pixel = frame[i];
      led_driver->neoPixelSetValue(i, pixel.r, pixel.g, pixel.b);
    }

    // Finally, show the new colors on the led strip
    led_driver->neoPixelShow();

    // Increment our frame index for the next loop
    // Note: get_frame() takes care of performing modulo math to wrap this
    // value down to the valid number of frames. So, we can just keep
    // incrementing and rolling over forever.
    frame_idx += 1;
  }
}

void leds::off() {
  gpio_put(LED_POWER_EN, false);
  led_driver->neoPixelFill(0, 0, 0, true);
}

void leds::set_pattern_next() {
  patterns.next();
}
