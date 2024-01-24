#include <pico/time.h>
#include "NeoPixelConnect.h"

class Index {
private:
    int max_val;
    int current_val;
public:
    Index(int max_val) : max_val{max_val}, current_val{0} {};

    void incr() {
        this->current_val += 1;
        this->current_val %= max_val;
    }

    int prev_int() const {
        if (this->current_val == 0) {
            return this->max_val - 1;
        }
        return this->current_val - 1;
    }

    int as_int() const { return this->current_val; }
};

int main() {
    constexpr int LOOP_DELAY_MS = 100;
    constexpr int NUM_LEDS = 300;
    constexpr int LED_COLOR[] = {
            100, 200, 50,
    };

    // Setup
    NeoPixelConnect led_driver{0, NUM_LEDS};
    Index led_index{NUM_LEDS};

    // Run forever
    for (;;) {
        // 0 out the prior color
        led_driver.neoPixelSetValue( led_index.prev_int(),0,0,0, false);
        // Set the new color
        led_driver.neoPixelSetValue(
                led_index.as_int(),
                LED_COLOR[0],
                LED_COLOR[1],
                LED_COLOR[2],
                false
        );
        led_driver.neoPixelShow();

        sleep_ms(LOOP_DELAY_MS);
        led_index.incr();
    }
}
