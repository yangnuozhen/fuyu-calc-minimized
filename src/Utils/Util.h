#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>
#include <U8g2lib.h>
#include <U8x8lib.h>
#include <BleKeyboard.h>
#include <string>
#include <cmath>
#include <sstream>
#include <FS.h>
#include <SD.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

#define SPI_CLK 18
#define SPI_DATA 23
#define SPI_CS 5
#define SPI_DC 16
#define SPI_RESET 17
#define WAKEUP_PIN 36

extern U8G2_SSD1322_NHD_256X64_F_4W_HW_SPI u8g2;
const std::string VERSION = "minimal";
const int REFRESH_DURATION = 10;
const float DELTA_TIME = 100;
extern bool sample;
const int WIDTH = 256;
const int HEIGHT = 64;

const byte ROWCNT = 5;
const byte COLCNT = 6;
extern byte ROW_MISO[ROWCNT];
extern byte COL_MOSI[COLCNT];
extern char keys[ROWCNT][COLCNT];

extern bool HIDE_SIDEBAR;
extern bool SHOW_BATTERY_PERCENTAGE;
extern bool sdAvailable;
extern BleKeyboard bleKeyboard;

enum KeyStatus {
    NOT_PRESSED, PRESSED, RISING_EDGE, FALLING_EDGE
};

enum AnimationType {
    LINEAR, INDENT, INDENTINV, BOUNCE
};

extern AnimationType menuScroll;
extern AnimationType menuEntry;
extern AnimationType cursorScroll;
extern AnimationType sliderMove;

std::string getKeyString(uint16_t x);
std::string numToStr(long double x);
uint8_t getBatteryLevel();
float getBatteryVoltage();
std::string getBatteryPercentageStr();
#endif
