#include <pico/stdlib.h>
#include <pico/bootrom.h>

#include "usb.h"

static void uart_irq();

void usb::init() {
  stdio_usb_init();
}

void usb::loop() {
  if (stdio_usb_connected()) {
    int ch = stdio_getchar_timeout_us(1);
    if (ch == 'r') {
      rom_reset_usb_boot(0, 0);
    } else if (ch == 'b') {
      #define AIRCR_Register (*((volatile uint32_t*)(PPB_BASE + 0x0ED0C)))
      AIRCR_Register = 0x5FA0004;
    }
  }
}
