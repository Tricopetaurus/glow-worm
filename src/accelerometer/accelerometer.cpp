#include <pico/sem.h>
#include <hardware/i2c.h>
#include <hardware/gpio.h>
#include <hardware/irq.h>

#include "accelerometer.h"
#include "lis3dh.h"

static void irq_callback(uint gpio, uint32_t events);
static uint8_t read_register_u8(uint8_t addr);
static void write_register_u8(uint8_t addr, uint8_t val);
static uint16_t write_register_u16(uint8_t adsdr);
static void write_register_u16(uint8_t addr, uint16_t val);

static semaphore_t sem;

void accel::clear_tap() {
  // We just want to set the permits back to 0 if it's not already there.
  sem_try_acquire(&sem);
}

void accel::init() {
  i2c_init(i2c_default, 100*1000);
  gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);

  // Set up our semaphore that'll track when a tap event comes around
  sem_init(&sem, 0, 1);

  // Configure accelerometer for double-tap, assign an interrupt to handle
  write_register_u8(lis::REG_CTRL1, lis::CTRL1_DEFAULT);
  write_register_u8(lis::REG_CTRL3, lis::CTRL3_DEFAULT);
  write_register_u8(lis::REG_TAP_CFG, lis::TAP_CFG_DEFAULT);
  write_register_u8(lis::REG_TAP_THS, lis::TAP_THS_DEFAULT);
  write_register_u8(lis::REG_TAP_DURATION, lis::TAP_DURATION_DEFAULT);
  write_register_u8(lis::REG_TAP_TIME_LATENCY, lis::TAP_TIME_LATENCY_DEFAULT);
  write_register_u8(lis::REG_TAP_TIME_WINDOW, lis::TAP_TIME_WINDOW_DEFAULT);

  gpio_init(lis::INT_PIN);
  gpio_set_irq_enabled_with_callback(lis::INT_PIN, GPIO_IRQ_EDGE_RISE, true, &irq_callback);
}


uint8_t accel::wait_for_tap() {
  sem_acquire_blocking(&sem);
  return read_register_u8(lis::REG_TAP_SRC);
;
}

uint8_t accel::wait_for_tap_with_timeout_ms(unsigned int timeout_ms) {
  if (sem_acquire_timeout_ms(&sem, timeout_ms)) {
    return read_register_u8(lis::REG_TAP_SRC);
  } else {
    return 0;
  }
}

bool accel::test() {
  // Test #1: Check if WHOAMI has the expected value.
  uint8_t whoami = read_register_u8(lis::REG_WHOAMI);
  if (whoami != lis::EXPECTED_WHOAMI) {
    return false;
  }

  // Test #2: Configure for single tap, then wait up to 1s for a tap.
  {
    write_register_u8(lis::REG_CTRL1, lis::CTRL1_DEFAULT);
    write_register_u8(lis::REG_CTRL2, lis::CTRL2_DEFAULT);
    write_register_u8(lis::REG_CTRL3, lis::CTRL3_DEFAULT);
    write_register_u8(lis::REG_CTRL4, lis::CTRL4_DEFAULT);
    write_register_u8(lis::REG_TAP_CFG, lis::TAP_CFG_DEFAULT);
    write_register_u8(lis::REG_TAP_DURATION, lis::TAP_DURATION_DEFAULT);
    write_register_u8(lis::REG_TAP_THS, lis::TAP_THS_DEFAULT);
    accel::clear_tap();
    bool click_detected = accel::wait_for_tap_with_timeout_ms(1000);
    if (!click_detected) {
      return false;
    }
  }

  return true;
}

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

static uint16_t write_register_u16(uint8_t adsdr) {

}

static void write_register_u16(uint8_t addr, uint16_t val) {

}
