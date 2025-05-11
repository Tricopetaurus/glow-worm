/**
 * USB Module
 *
 * This module handles USB comms.
 * It will accept commands over the USB-CDC UART interface
 * These commands enable a developer to easily reboot and/or
 * reprogram the device.
 */
#pragma once

namespace usb {

  /**
   * Initialize the USB Module
   */
  void init();

  /**
   * Periodic loop for the USB Module.
   * Should be called regularly to check and consume commands.
   */
  void loop();
};