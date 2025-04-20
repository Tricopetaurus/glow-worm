#pragma once

namespace lis {
  constexpr int INT_PIN = 22;
  constexpr int ADDRESS = 0x18;
  constexpr int REG_WHOAMI = 0x0F;
  constexpr int EXPECTED_WHOAMI = 0x33;

  constexpr int REG_CTRL0 = 0x1E;

  constexpr int REG_CTRL1 = 0x20;
  constexpr int CTRL1_ODR_400HZ = 0b0111 << 4;
  constexpr int CTRL1_ODR_200HZ = 0b0110 << 4;
  constexpr int CTRL1_LOWPWR_EN = 1 << 3;
  constexpr int CTRL1_Z_EN = 1 << 2;
  constexpr int CTRL1_Y_EN = 1 << 1;
  constexpr int CTRL1_X_EN = 1 << 0;
  constexpr int CTRL1_DEFAULT = (CTRL1_ODR_400HZ | CTRL1_LOWPWR_EN | CTRL1_X_EN | CTRL1_Y_EN | CTRL1_Z_EN);

  constexpr int REG_CTRL2 = 0x21;
  constexpr int CTRL2_CLICK_HIGHPASS_FLTR = 1 << 2;
  constexpr int CTRL2_DEFAULT = (CTRL2_CLICK_HIGHPASS_FLTR);

  constexpr int REG_CTRL3 = 0x22;
  constexpr int CTRL3_INT1_CLICK_EN = (1<<7);
  constexpr int CTRL3_DEFAULT = (CTRL3_INT1_CLICK_EN);

  constexpr int REG_CTRL4 = 0x23;
  constexpr int REG4_RANGE_8G = 0b10 << 4;
  constexpr int CTRL4_DEFAULT = REG4_RANGE_8G;

  constexpr int REG_CTRL5 = 0x24;
  constexpr int REG_CTRL6 = 0x25;
  constexpr int REG_STATUS = 0x27;
  constexpr int REG_INT1_CFG = 0x30;
  constexpr int REG_INT1_SRC = 0x31;
  constexpr int REG_INT1_THS = 0x32;
  constexpr int REG_INT1_DUR = 0x33;

  constexpr int REG_TAP_CFG = 0x38;
  // Enable X/Y/Z axis (D)ouble or (S)ingle tap events
  constexpr int TAP_CFG_ZD = (1<<5);
  constexpr int TAP_CFG_ZS = (1<<4);
  constexpr int TAP_CFG_YD = (1<<3);
  constexpr int TAP_CFG_YS = (1<<2);
  constexpr int TAP_CFG_XD = (1<<1);
  constexpr int TAP_CFG_XS = (1<<0);
  constexpr int TAP_CFG_DEFAULT = (TAP_CFG_XD);

  constexpr int REG_TAP_SRC = 0x39;
  constexpr int TAP_SRC_DTAP = (1<<5);
  constexpr int TAP_SRC_STAP = (1<<4);
  constexpr int TAP_SRC_NEG = (1<<3);
  constexpr int TAP_SRC_Z = (1<<2);
  constexpr int TAP_SRC_Y = (1<<1);
  constexpr int TAP_SRC_X = (1<<0);

  constexpr int REG_TAP_THS = 0x3A;
  constexpr int TAP_THS_DEFAULT = 80;

  constexpr int REG_TAP_DURATION = 0x3B;
  constexpr int TAP_DURATION_DEFAULT = 0x33; // If ODR is 400, this is 127ms. Values supplied in appnote

  constexpr int REG_TAP_TIME_LATENCY = 0x3C;
  constexpr int TAP_TIME_LATENCY_DEFAULT = 0x30;

  constexpr int REG_TAP_TIME_WINDOW = 0x3D;
  constexpr int TAP_TIME_WINDOW_DEFAULT = 100;
};