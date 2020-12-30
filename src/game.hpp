#include <Arduino.h>
#include <esp_timer.h>

#include "defines.hpp"
#include "gfx/gfx.hpp"
#include "gfx/bitmaps.hpp"
#include "Ps3Controller.h"

#pragma once

#define NOP() asm volatile("nop")

struct CurrentTetromino
{
  Tetromino matrix;
  pos_int_t x;
  pos_int_t y;
};

struct Presslength {
  uint32_t left;
  uint32_t right;
  uint32_t spin;
  uint32_t down;
};

void StartGame(GFX *graphics);

void GameLoop(uint64_t frame, uint32_t second);
void HandleInput(uint64_t frame);
void GetNextTetromino();
uint8_t * RotateCurrentTetromino();
bool IsValidMove(Tetromino* matrix, pos_int_t x, pos_int_t y);
bool PlaceTetromino();

void Draw(uint32_t second);
void DrawPlayField();
void DrawCurrentTetromino();
