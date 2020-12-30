#include "FastLED.h"
#include "defines.hpp"
#include "font.hpp"

#pragma once

typedef int16_t pos_int_t;

class GFX
{
public:
  void fill(CRGB color);
  void drawPixel(pos_int_t x, pos_int_t y, CRGB color);
  void drawLine(pos_int_t x1, pos_int_t y1, pos_int_t x2, pos_int_t y2, CRGB color);
  void drawOutlinedSquare(pos_int_t x1, pos_int_t y1, pos_int_t x2, pos_int_t y2, CRGB color);
  void drawFilledSquare(pos_int_t x1, pos_int_t y1, pos_int_t x2, pos_int_t y2, CRGB color);

  void drawBitmap(const uint8_t *bitmap, uint8_t width, uint8_t height, pos_int_t x, pos_int_t y, CRGB color);
  void drawBitmap(const uint8_t *bitmap, uint8_t width, uint8_t height, pos_int_t x, pos_int_t y, int8_t rotate, CRGB color);
  void drawString(const char *str, pos_int_t x, pos_int_t y, CRGB color);
  void drawChar(char chr, pos_int_t x, pos_int_t y, CRGB color);

  CRGB getPixel(pos_int_t x, pos_int_t y);
  bool getPixelIsSet(pos_int_t x, pos_int_t y);

  GFX(uint8_t width, uint8_t height);

  void init();
  void init(uint8_t brightness);

  void clear();

  void setBrightness(uint8_t scale);
  void show();

  bool isDirty();

  CRGB *pixels;

private:
  uint16_t XY(pos_int_t x, pos_int_t y);

  bool dirty = false;

  const uint8_t width;
  const uint8_t height;
  const uint16_t ledCount;
};
