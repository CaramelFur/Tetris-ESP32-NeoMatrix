#include "EEPROM.h"

#include "highscore.hpp"

uint32_t CurrentHighScore = 0;

uint32_t GetHighScore()
{
  CurrentHighScore = EEPROM.readUInt(0);
  return CurrentHighScore;
}

bool UpdateHighScore(uint32_t score)
{
  uint32_t currentHighScore = GetHighScore();

  Serial.println("hhhh");
  Serial.println(score);
  Serial.println(currentHighScore);

  if (score > currentHighScore)
  {
    EEPROM.writeUInt(0, score);
    EEPROM.commit();
    return true;
  }
  else
  {
    return false;
  }
}

void ResetHighScore()
{
  EEPROM.writeUInt(0, 0);
  EEPROM.commit();
  CurrentHighScore = 0;
}