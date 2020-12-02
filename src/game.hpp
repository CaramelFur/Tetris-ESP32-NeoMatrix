#include <Arduino.h>
#include <esp_timer.h>

#include "defines.hpp"
#include "gfx/gfx.hpp"


#pragma once

#define NOP() asm volatile ("nop")

void StartGame(GFX *graphics);

void GameLoop(uint32_t frame);

void Draw();
