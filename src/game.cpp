#include "game.hpp"

GFX *gfx = nullptr;

void StartGame(GFX *graphics)
{
  gfx = graphics;

  uint32_t prevLoop = esp_timer_get_time();
  uint32_t frame = 0;

  while (true)
  {
    // TODO: add pause feature

    GameLoop(frame);
    Draw();

    frame = (frame + 1) % TICKSPEED;

    while (esp_timer_get_time() < prevLoop + TICKDELAY)
    {
      NOP();
    }
    prevLoop = esp_timer_get_time();
  }
}

void GameLoop(uint32_t frame)
{
  if(frame == 0){
    Serial.println("Second!");
  }
  
}

void Draw()
{
  if (gfx->isDirty())
    gfx->show();
}