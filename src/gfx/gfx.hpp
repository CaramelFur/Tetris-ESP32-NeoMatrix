#include "FastLED.h"
#include "defines.hpp"

#pragma once

class GFX
{
public:
  GFX(uint8_t width, uint8_t height);

  void init();
  void init(uint8_t brightness);

  void fill(CRGB color);
  void setPixel(uint8_t x, uint8_t y, CRGB color);
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
