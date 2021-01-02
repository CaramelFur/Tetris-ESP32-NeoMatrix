// Card 3 is background music
#define MUSIC_CHIP_PIN 22

// Card 2 is effects
#define EFFECT_CHIP_PIN 21

// Other stuff
#define DEFAULT_VOLUME 20

#pragma once

enum MUSIC
{
  main = 1
};

enum EFFECT
{
  move_block = 1,
  rotate_block,
  block_landing,
  game_over,
  try_again,
  high_score,
  line_clear,
  tetris_clear,
};

void InitAudio();
void StartMusic(MUSIC music);
void StopMusic();
void PlayEffect(EFFECT effect);
void PauseMusic();
void StartMusic();
