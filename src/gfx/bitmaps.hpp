#include <Arduino.h>
#include "FastLED.h"

#pragma once

struct Tetromino
{
  char name;
  uint8_t size;
  uint8_t *bitmap;
};

#define TETROMINO_COUNT 7

extern const uint8_t controller_bmp[];

extern const Tetromino tetrominos[];

extern const CRGB TetrisColorMap[];
