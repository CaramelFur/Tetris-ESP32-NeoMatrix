#include "Arduino.h"

#pragma once

extern uint32_t CurrentHighScore;

uint32_t GetHighScore();

bool UpdateHighScore(uint32_t score);

void ResetHighScore();