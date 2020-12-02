#include <Arduino.h>
#include "FastLED.h"

#pragma once

struct TetrisPiece
{
  uint8_t size;
  CRGB color;
  uint8_t* bitmap;
};

extern const uint8_t controller_bmp[];

extern const TetrisPiece tetris_pieces[];

