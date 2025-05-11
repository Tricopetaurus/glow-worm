#include <cctype>

#include <pico/stdlib.h>
#include <pico/bootrom.h>

#include "usb.h"


// ================================
//  PRIVATE VARIABLES
// ================================
static const char* USAGE_STR =
    "=========================" "\n"
    " GLOW-WORM Firmware r1.1 " "\n"
    "=========================" "\n"
    " Commands:               " "\n"
    "   P: Program board with " "\n"
    "      a new .UF2 file    " "\n"
    "   R: Reboot the board   " "\n"
;

static const char* PROMPT_STR = "> ";

// ================================
//  PRIVATE FUNCTION DECLARATIONS
// ================================
/**
 * Convenience function to put a string out on the UART.
 * Strings are print without appending a newline
 */
static void put_str(const char* str);

/**
 * Print the USAGE_STR to the UART
 */
static void print_usage();

/**
 * Put the PROMPT_STR on the UART
 */
static void put_prompt();


// ================================
//  PUBLIC FUNCTION DEFINITIONS
// ================================
void usb::init() {
  stdio_usb_init();
}

void usb::loop() {
  static bool should_prompt = true;
  // Only run this loop if the USB is connected
  if (stdio_usb_connected()) {
    // A little bit of logic to only put the prompt "> " down when we've
    // processed a command.
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
        // NOTE: Once this command hits there are only two ways to get a working
        // device back.
        // 1. Drop in a .UF2 file that's been compiled for the rp2040
        // 2. Remove all power from the rp2040 (including battery), then turn it back on.
        put_str("Rebooting to UF2 Bootloader!\n");
        rom_reset_usb_boot(0, 0);
      } else if (ch == 'r') {
        // Honestly this was just pulled from a stackoverflow post somewhere.
        // It works. It would be better to review the datasheet and make sure
        // there isn't a more proper reboot command.
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


// ================================
//  PRIVATE FUNCTION DEFINITIONS
// ================================
static void put_str(const char* str) {
  stdio_put_string(str, strlen(str), false, false);
}

static void print_usage() {
  put_str(USAGE_STR);
}

static void put_prompt() {
  put_str(PROMPT_STR);
}