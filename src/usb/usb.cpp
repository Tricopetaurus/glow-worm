#include <cctype>

#include <pico/stdlib.h>
#include <pico/bootrom.h>

#include "usb.h"

static const char* USAGE_STR =
    "=========================" "\n"
    " GLOW-WORM Firmware r1.0 " "\n"
    "=========================" "\n"
    " Commands:               " "\n"
    "   P: Program board with " "\n"
    "      a new .UF2 file    " "\n"
    "   R: Reboot the board   " "\n"
;

static const char* PROMPT_STR = "> ";

static void put_str(const char* str);
static void print_usage();
static void put_prompt();

void usb::init() {
  stdio_usb_init();
}

void usb::loop() {
  static bool should_prompt = true;
  if (stdio_usb_connected()) {
    if (should_prompt) {
      print_usage();
      put_prompt();
      should_prompt = false;
    }
    int ch = stdio_getchar_timeout_us(1);
    if (ch && ch != PICO_ERROR_TIMEOUT) {
      ch = std::tolower(ch);
      should_prompt = true;
      stdio_putchar(ch);
      stdio_puts("\n");
      if (ch == 'p') {
        put_str("Rebooting to UF2 Bootloader!\n");
        rom_reset_usb_boot(0, 0);
      } else if (ch == 'r') {
        put_str("Rebooting to Application!\n");
        #define AIRCR_Register (*((volatile uint32_t *)(PPB_BASE + 0x0ED0C)))
        AIRCR_Register = 0x5FA0004;
      } else {
        put_str("Error: \"");
        stdio_putchar(ch);
        put_str("\" not valid command.\n");
      }
    }
  }
}


static void put_str(const char* str) {
  stdio_put_string(str, strlen(str), false, false);
}

static void print_usage() {
  put_str(USAGE_STR);
}

static void put_prompt() {
  put_str(PROMPT_STR);
}