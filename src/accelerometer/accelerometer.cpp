#include <pico/platform/cpu_regs.h>
#include <hardware/sync.h>
#include <hardware/structs/scb.h>

#include <pico/sem.h>
#include <pico/stdlib.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>

#include "accelerometer.h"
#include "lis3dh.h"

#define DEBUG_PRINTS (false)

// ================================
//  PRIVATE VARIABLES
// ================================
enum class STATE {
  START,
  TAP_1,
  NOISY_TAP,
  WAIT_1,
  EVENT_SINGLE_TAP,
  EVENT_DOUBLE_TAP
};

static STATE current_state = STATE::START;
static semaphore_t sem;

// ================================
//  PRIVATE FUNCTION DECLARATIONS
// ================================
/**
 * Generic Read of 8 bytes from a given accelerometer register
 */
static uint8_t read_register_u8(uint8_t addr);

/**
 * Generic Write of 8 bytes from a given accelerometer register
 */
static void write_register_u8(uint8_t addr, uint8_t val);

/**
 * Interrupt callback
 * Whenever the GPIO for the accelerometer toggles because of a tap,
 * control will immediately jump to this function.
 */
static void irq_callback(uint gpio, uint32_t events);

/**
 * Clear any registered tap events.
 * Useful for kicking off the state machine from a known state
 */
static void clear_tap();

/**
 * Convenience function for logging.
 * Returns a string representation of our current / next state.
 */
static constexpr const char* state_to_str(STATE s);

// ================================
//  PUBLIC FUNCTION DEFINITIONS
// ================================
void accel::init() {
  // Set up for I2C communication with the accelerometer
  i2c_init(i2c_default, 100*1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

  // Set up our semaphore that'll track when a tap event comes around
  sem_init(&sem, 0, 1);

  // Configure accelerometer for single-tap, assign an interrupt to handle events
  // These are all using default values pulled from lis3dh.h
  write_register_u8(lis::REG_CTRL1, lis::CTRL1_DEFAULT);
  write_register_u8(lis::REG_CTRL3, lis::CTRL3_DEFAULT);
  write_register_u8(lis::REG_TAP_CFG, lis::TAP_CFG_DEFAULT);
  write_register_u8(lis::REG_TAP_THS, lis::TAP_THS_DEFAULT);
  write_register_u8(lis::REG_TAP_DURATION, lis::TAP_DURATION_DEFAULT);
  write_register_u8(lis::REG_TAP_TIME_LATENCY, lis::TAP_TIME_LATENCY_DEFAULT);
  write_register_u8(lis::REG_TAP_TIME_WINDOW, lis::TAP_TIME_WINDOW_DEFAULT);

  // Set up that interrupt for whenever tap events happen
  gpio_init(lis::INT_PIN);
  gpio_set_irq_enabled_with_callback(lis::INT_PIN, GPIO_IRQ_EDGE_RISE, true, &irq_callback);
  clear_tap();
}


// Low power setting
// If we're in the initial state (wait for tap)
// Then we can just chill on a WFI until irq hits
void accel::sleep_until_tap() {
  if (current_state == STATE::START) {
    // Stealing code from rp2_common/pico_sleep/sleep.c, processor_deep_sleep
    scb_hw->scr |= ARM_CPU_PREFIXED(SCR_SLEEPDEEP_BITS);
    __wfi();
  }
}

accel::CMD accel::loop() {
  static uint32_t state_change_time = 0;
  static constexpr uint32_t TIMEOUT_NOISE_MS = 100;
  static constexpr uint32_t TIMEOUT_1_MS = 100;
  static constexpr uint32_t TIMEOUT_2_MS = 200;

  STATE next_state = current_state;
  CMD command = CMD::NONE;
  bool tap_occurred = false;

  uint32_t current_time = to_ms_since_boot(get_absolute_time());

  // Check if a tap has occurred
  if(sem_try_acquire(&sem)) {
    if ((read_register_u8(lis::REG_TAP_SRC) & lis::TAP_SRC_NEG) != 0) {
      tap_occurred = true;
    }
  }

  switch(current_state) {

  case STATE::START:
    // If a tap event occurs, move to TAP_1
    if (tap_occurred) {
      next_state = STATE::TAP_1;
    }
    break;

  case STATE::TAP_1:
    // We need a minimum amount of time to pass as a filter against noise
    if (tap_occurred) {
      next_state = STATE::NOISY_TAP;
    } else if ((current_time - state_change_time) >= TIMEOUT_1_MS) {
      next_state = STATE::WAIT_1;
    }
    break;

  case STATE::NOISY_TAP:
    if (tap_occurred) {
      state_change_time = current_time;
    }
    else if ((current_time - state_change_time) >= TIMEOUT_NOISE_MS) {
      next_state = STATE::START;
    }
    break;

  case STATE::WAIT_1:
    // Minimum threshold met.
    // If a secondary timeout passes, we hit EVENT_SINGLE_TAP
    // If a new tap event happens, we hit EVENT_DOUBLE_TAP
    if (tap_occurred) {
      next_state = STATE::EVENT_DOUBLE_TAP;
    } else if ((current_time - state_change_time) >= TIMEOUT_2_MS) {
      next_state = STATE::EVENT_SINGLE_TAP;
    }
    break;

  case STATE::EVENT_SINGLE_TAP:
    // Event! Set our fire command, then head back to START
    command = CMD::NEXT;
    next_state = STATE::START;
    break;

  case STATE::EVENT_DOUBLE_TAP:
    command = CMD::TOGGLE_AWAKE;
    next_state = STATE::START;
    break;
  }

  if (next_state != current_state) {
#if defined(DEBUG_PRINTS) && (DEBUG_PRINTS)
    stdio_printf(":: %u :: %s -> %s\n",
                 current_time,
                 state_to_str(current_state),
                 state_to_str(next_state));
#endif
    current_state = next_state;
    state_change_time = current_time;
  }

  return command;
}

bool accel::test() {
  // Test #1: Check if WHOAMI has the expected value.
  uint8_t whoami = read_register_u8(lis::REG_WHOAMI);
  if (whoami != lis::EXPECTED_WHOAMI) {
    return false;
  }

  return true;
}

// ================================
//  PRIVATE FUNCTION DEFINITIONS
// ================================
static void irq_callback(uint gpio, uint32_t events) {
  if (gpio != lis::INT_PIN) {
    return;
  }
  if (events & GPIO_IRQ_EDGE_RISE) {
    sem_release(&sem);
  }
}

static uint8_t read_register_u8(uint8_t addr) {
  uint8_t rval;
  i2c_write_blocking(i2c_default, lis::ADDRESS, &addr, 1, true);
  i2c_read_blocking(i2c_default, lis::ADDRESS, &rval, 1, false);
  return rval;
}

static void write_register_u8(uint8_t addr, uint8_t val) {
  uint8_t sendme[] = {addr, val};
  i2c_write_blocking(i2c_default, lis::ADDRESS, sendme, 2, false);
}

static void clear_tap() {
  // We just want to set the permits back to 0 if it's not already there.
  sem_try_acquire(&sem);
}

static constexpr const char* state_to_str(STATE s) {
  switch(s) {
  case STATE::START:
    return "START";
  case STATE::TAP_1:
    return "TAP_1";
  case STATE::NOISY_TAP:
    return "NOISY_TAP";
  case STATE::WAIT_1:
    return "WAIT_1";
  case STATE::EVENT_SINGLE_TAP:
    return "SINGLE_TAP_EVENT";
  case STATE::EVENT_DOUBLE_TAP:
    return "DOUBLE_TAP_EVENT";
  }
  return "???";
}