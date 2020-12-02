#include "FastLED.h"
#include "defines.hpp"
#include "font.hpp"

#pragma once

class GFX
{
public:
  void fill(CRGB color);
  void drawPixel(uint8_t x, uint8_t y, CRGB color);
  void drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, CRGB color);
  void drawOutlinedSquare(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, CRGB color);
  void drawFilledSquare(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, CRGB color);

  void drawBitmap(const uint8_t *bitmap, uint8_t width, uint8_t height, uint8_t x, uint8_t y, CRGB color);
  void drawBitmap(uint8_t *bitmap, uint8_t width, uint8_t height, uint8_t x, uint8_t y, CRGB color);
  void drawString(char *str, uint8_t x, uint8_t y, CRGB color);

  GFX(uint8_t width, uint8_t height);

  void init();
  void init(uint8_t brightness);

  void clear();

  void setBrightness(uint8_t scale);
  void show();

  CRGB *pixels;

private:
  uint16_t XY(uint8_t x, uint8_t y);

  const uint8_t width;
  const uint8_t height;
  const uint16_t ledCount;
};
