#include <Arduino.h>

#pragma once

#define NOP() asm volatile("nop")

struct CurrentTetromino
{
  Tetromino matrix;
  pos_int_t x;
  pos_int_t y;
};

struct Presslength {
  int32_t left;
  int32_t right;
  int32_t spin;
  int32_t down;
  int32_t start;
  int32_t select;
};

void StartGame(GFX *graphics);
void Restart();
void GameLoop(uint64_t frame, uint32_t second);
void HandleInput(uint64_t frame, bool doGame);
void GetNextTetromino();
uint8_t * RotateCurrentTetromino();
bool IsValidMove(Tetromino* matrix, pos_int_t x, pos_int_t y);
bool PlaceTetromino();

Tetromino CreateClone(Tetromino tetromino);

void Draw(uint32_t second);
void DrawPlayField();
void DrawCurrentTetromino();
void DrawScores();

uint16_t GetLevel(uint32_t linesCleared);
uint8_t GetGravitySpeed(uint16_t level);
