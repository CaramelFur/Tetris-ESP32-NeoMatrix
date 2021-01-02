#include <esp_timer.h>

#include "defines.hpp"
#include "gfx/gfx.hpp"
#include "gfx/bitmaps.hpp"
#include "Ps3Controller.h"
#include "audio.hpp"

#include "game.hpp"

const uint16_t PointsPerLineCombo[] = {
    40,
    100,
    300,
    1200,
};

const CRGB levelColors[] = {
    CRGB::LimeGreen,
    CRGB::Yellow,
    CRGB::DarkRed,
};

GFX *gfx = nullptr;
Tetromino nextTetromino = {0};
CurrentTetromino currentTetromino;

Presslength presslength = Presslength{
    0,
    0,
    0,
    0,
};

uint8_t playfield[PLAYFIELD_WIDTH][PLAYFIELD_HEIGHT] = {0};

bool gameOver = false;
uint32_t waitFrames = 0;

uint16_t clearedLines = 0;
uint32_t score = 0;
uint8_t softDropCounter = 0;

void StartGame(GFX *graphics)
{
  gfx = graphics;

  uint32_t prevLoop = esp_timer_get_time();
  uint64_t frame = 0;
  uint32_t second = 0;

  GetNextTetromino();

  StartMusic(MUSIC::main);

  while (true)
  {
    //Serial.println(((uint32_t)frame));
    // TODO: add pause feature

    if (waitFrames == 0)
      GameLoop(frame, second);
    else
      waitFrames--;
    Draw(second);

    frame++;

    if (frame % TICKSPEED == 0)
    {
      second++;
      Serial.print("Mem: ");
      Serial.println(ESP.getFreeHeap());
    }

    while (esp_timer_get_time() < prevLoop + TICKDELAY)
    {
      NOP();
    }
    prevLoop = esp_timer_get_time();
  }
}

void Restart()
{
  gameOver = false;
  waitFrames = 0;

  clearedLines = 0;
  score = 0;
  softDropCounter = 0;

  memset(playfield, 0, PLAYFIELD_WIDTH * PLAYFIELD_HEIGHT);
  presslength = Presslength{
      0,
      0,
      0,
      0,
  };

  delete nextTetromino.bitmap;
  nextTetromino = {0};

  GetNextTetromino();
  StartMusic(MUSIC::main);
}

void GameLoop(uint64_t frame, uint32_t second)
{
  if (!Ps3.isConnected())
    return;

  if (gameOver)
  {
    if (Ps3.data.button.start)
      Restart();
    return;
  }

  HandleInput(frame);

  if (frame % (GetGravitySpeed(GetLevel(clearedLines))) == 0)
  {
    if (IsValidMove(&currentTetromino.matrix, currentTetromino.x, currentTetromino.y + 1))
      currentTetromino.y += 1;
    else if (PlaceTetromino())
    {
      // Game over
      gameOver = true;

      StopMusic();
      PlayEffect(EFFECT::game_over);
    }
  }
}

void HandleInput(uint64_t frame)
{
  if (Ps3.data.button.left)
  {
    if (presslength.left % INPUTSPEED == 0 && IsValidMove(&currentTetromino.matrix, currentTetromino.x - 1, currentTetromino.y))
    {
      currentTetromino.x -= 1;
      PlayEffect(EFFECT::move_block);
    }
    presslength.left++;
  }
  else
  {
    presslength.left = 0;
  }

  if (Ps3.data.button.right)
  {
    if (presslength.right % INPUTSPEED == 0 && IsValidMove(&currentTetromino.matrix, currentTetromino.x + 1, currentTetromino.y))
    {
      currentTetromino.x += 1;
      PlayEffect(EFFECT::move_block);
    }
    presslength.right++;
  }
  else
  {
    presslength.right = 0;
  }

  if (Ps3.data.button.down)
  {
    if (presslength.down >= 0)
    {
      if (presslength.down % (TICKSPEED / 20) == 0)
      {
        if (IsValidMove(&currentTetromino.matrix, currentTetromino.x, currentTetromino.y + 1))
        {
          softDropCounter += 1;
          currentTetromino.y += 1;
        }
        else
        {
          PlaceTetromino();
          score += softDropCounter;
          softDropCounter = 0;
          presslength.down = -20;
        }
      }

      presslength.down++;
    }
  }
  else
  {
    softDropCounter = 0;
    presslength.down = 0;
  }

  if (Ps3.data.button.up || Ps3.data.button.cross || Ps3.data.button.circle)
  {
    if (presslength.spin == 0)
    {
      uint8_t *newMatrix = RotateCurrentTetromino();

      Tetromino newTetromino = Tetromino{
          .name = currentTetromino.matrix.name,
          .size = currentTetromino.matrix.size,
          .bitmap = newMatrix,
      };

      if (IsValidMove(&newTetromino, currentTetromino.x, currentTetromino.y))
      {
        memcpy(currentTetromino.matrix.bitmap, newMatrix, newTetromino.size * newTetromino.size);
        PlayEffect(EFFECT::rotate_block);
      }

      delete newMatrix;
    }

    presslength.spin++;
  }
  else
  {
    presslength.spin = 0;
  }
}

void GetNextTetromino()
{
  Tetromino toCurrentTetromino;
  if (nextTetromino.name == 0)
    toCurrentTetromino = CreateClone(tetrominos[esp_random() % TETROMINO_COUNT]);
  else
  {
    toCurrentTetromino = nextTetromino;
    delete currentTetromino.matrix.bitmap;
  }

  pos_int_t col = PLAYFIELD_WIDTH / 2 - (toCurrentTetromino.size / 2 + toCurrentTetromino.size % 2);
  pos_int_t row = toCurrentTetromino.name == 'I' ? -1 : -2;

  currentTetromino = CurrentTetromino{
      .matrix = toCurrentTetromino,
      .x = col,
      .y = row,
  };

  nextTetromino = CreateClone(tetrominos[esp_random() % TETROMINO_COUNT]);

  waitFrames += TICKSPEED / 3;
};

// Dont forget to delete the returned value eventually
uint8_t *RotateCurrentTetromino()
{
#define size currentTetromino.matrix.size
  uint8_t *buffer = new uint8_t[size * size];

  for (pos_int_t x = 0; x < size; x++)
    for (pos_int_t y = 0; y < size; y++)
      buffer[x * size + (size - y - 1)] = currentTetromino.matrix.bitmap[y * size + x];

  return buffer;
#undef size
}

bool IsValidMove(Tetromino *matrix, pos_int_t px, pos_int_t py)
{
  for (pos_int_t x = 0; x < matrix->size; x++)
  {
    for (pos_int_t y = 0; y < matrix->size; y++)
    {
      if (!matrix->bitmap[y * matrix->size + x])
        continue;

      // Check board bounds
      if (px + x < 0 || px + x >= PLAYFIELD_WIDTH || py + y >= PLAYFIELD_HEIGHT)
        return false;

      if (px + x >= 0 && py + y >= 0 && playfield[px + x][py + y])
        return false;
    }
  }

  return true;
}

// Return true if game over
bool PlaceTetromino()
{
  for (pos_int_t x = 0; x < currentTetromino.matrix.size; x++)
  {
    for (pos_int_t y = 0; y < currentTetromino.matrix.size; y++)
    {
      if (!currentTetromino.matrix.bitmap[y * currentTetromino.matrix.size + x])
        continue;

      if (currentTetromino.y + y < 0)
        return true;

      playfield[currentTetromino.x + x][currentTetromino.y + y] = currentTetromino.matrix.bitmap[y * currentTetromino.matrix.size + x];
    }
  }

  uint8_t rowsFilled = 0;

  for (pos_int_t row = PLAYFIELD_HEIGHT - 1; row >= 0;)
  {
    bool rowFilled = true;
    for (pos_int_t x = 0; x < PLAYFIELD_WIDTH; x++)
      if (playfield[x][row] == 0)
        rowFilled = false;

    if (rowFilled)
    {
      rowsFilled++;
      // Move every row one down
      for (pos_int_t r = row; r > 0; r--)
        for (pos_int_t c = 0; c < PLAYFIELD_WIDTH; c++)
          playfield[c][r] = playfield[c][r - 1];
      // Fill the topmost row with zeroes
      for (pos_int_t c = 0; c < PLAYFIELD_WIDTH; c++)
        playfield[c][0] = 0;
    }
    else
      row--;
  }

  if (rowsFilled > 4)
    rowsFilled = 4;

  if (rowsFilled == 0)
    PlayEffect(EFFECT::block_landing);
  else
  {
    if (rowsFilled < 4)
      PlayEffect(EFFECT::line_clear);
    else
      PlayEffect(EFFECT::tetris_clear);

    clearedLines += rowsFilled;
    score += PointsPerLineCombo[rowsFilled - 1] * (GetLevel(clearedLines) + 1);
  }

  GetNextTetromino();

  return false;
}

Tetromino CreateClone(Tetromino tetromino)
{
  Tetromino newTetromino = tetromino;

  newTetromino.bitmap = new uint8_t[tetromino.size * tetromino.size];
  memcpy(newTetromino.bitmap, tetromino.bitmap, tetromino.size * tetromino.size);

  return newTetromino;
}

void Draw(uint32_t second)
{
  gfx->clear();

  DrawPlayField();

  DrawScores();

  // Keepalive pixel
  gfx->drawPixel(PLAYFIELD_WIDTH, MATRIX_SIZE - 1, second % 2 ? CRGB::White : CRGB::Gray);

  if (gfx->isDirty())
    gfx->show();
}

void DrawPlayField()
{
  if (gameOver)
  {
    Serial.println("Go!");
    gfx->drawLine(0, 0, PLAYFIELD_WIDTH - 1, PLAYFIELD_HEIGHT - 1, CRGB::Red);
    gfx->drawLine(0, PLAYFIELD_HEIGHT - 1, PLAYFIELD_WIDTH - 1, 0, CRGB::Red);
  }
  else
  {
    for (pos_int_t x = 0; x < PLAYFIELD_WIDTH; x++)
      for (pos_int_t y = 0; y < PLAYFIELD_HEIGHT; y++)
        gfx->drawPixel(x, y, TetrisColorMap[playfield[x][y]]);

    DrawCurrentTetromino();
  }

  gfx->drawLine(PLAYFIELD_WIDTH, 0, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT - 1, CRGB::White);
  gfx->drawLine(PLAYFIELD_WIDTH, 2, MATRIX_SIZE - 1, 2, CRGB::White);
  gfx->drawLine(PLAYFIELD_WIDTH, 6, MATRIX_SIZE - 1, 6, CRGB::White);
  gfx->drawLine(MATRIX_SIZE - 3, 0, MATRIX_SIZE - 3, 2, CRGB::White);
}

void DrawScores()
{
  uint32_t tempScore = score;

  if (tempScore > 9999999)
    tempScore = 9999999;

  uint8_t i = 0;

  while (tempScore > 0)
  {
    gfx->drawLine(MATRIX_SIZE - 1 - i, MATRIX_SIZE, MATRIX_SIZE - 1 - i, MATRIX_SIZE - (tempScore % 10), CRGB::LawnGreen);
    tempScore = tempScore / 10;
    i++;
  }

  uint16_t level = GetLevel(clearedLines);
  if (level > 3 * LEVELCOUNT_HEIGHT * LEVELCOUNT_WIDTH)
    level = 3 * LEVELCOUNT_HEIGHT * LEVELCOUNT_WIDTH;

  for (pos_int_t y = 0; y < LEVELCOUNT_HEIGHT; y++)
  {
    for (pos_int_t x = 0; x < LEVELCOUNT_WIDTH; x++)
    {
      uint16_t lvlColor = level / (LEVELCOUNT_WIDTH * LEVELCOUNT_HEIGHT);
      uint16_t levelPart = level % (LEVELCOUNT_WIDTH * LEVELCOUNT_HEIGHT);
      if ((y * LEVELCOUNT_WIDTH + x + 1) <= levelPart)
      {
        gfx->drawPixel(PLAYFIELD_WIDTH + 1 + x, 3 + y, levelColors[lvlColor]);
      }
      else if (lvlColor > 0)
      {
        gfx->drawPixel(PLAYFIELD_WIDTH + 1 + x, 3 + y, levelColors[lvlColor - 1]);
      }
    }
  }
}

void DrawCurrentTetromino()
{
  for (pos_int_t x = 0; x < currentTetromino.matrix.size; x++)
  {
    for (pos_int_t y = 0; y < currentTetromino.matrix.size; y++)
    {
      uint8_t color = currentTetromino.matrix.bitmap[y * currentTetromino.matrix.size + x];
      if (color != 0)
      {
        gfx->drawPixel(
            currentTetromino.x + x,
            currentTetromino.y + y,
            TetrisColorMap[color]);
      }
    }
  }

  pos_int_t nextTetrominoOffset = PLAYFIELD_WIDTH + 1;
  for (pos_int_t x = 0; x < nextTetromino.size; x++)
  {
    for (pos_int_t y = 0; y < nextTetromino.size; y++)
    {
      uint8_t color = nextTetromino.bitmap[y * nextTetromino.size + x];
      if (color != 0)
      {
        gfx->drawPixel(
            nextTetrominoOffset + x,
            y,
            TetrisColorMap[color]);
      }
    }
  }
}

uint16_t GetLevel(uint32_t linesCleared)
{
  return (linesCleared - (linesCleared % 10)) / 10;
}

uint8_t GetGravitySpeed(uint16_t level)
{
  if (level == 0)
    return 48;
  else if (level == 1)
    return 43;
  else if (level == 2)
    return 38;
  else if (level == 3)
    return 33;
  else if (level == 4)
    return 28;
  else if (level == 5)
    return 23;
  else if (level == 6)
    return 18;
  else if (level == 7)
    return 13;
  else if (level == 8)
    return 8;
  else if (level == 9)
    return 6;
  else if (level >= 10 && level <= 12)
    return 5;
  else if (level >= 13 && level <= 15)
    return 4;
  else if (level >= 16 && level <= 18)
    return 3;
  else if (level >= 19 && level <= 28)
    return 2;
  else if (level >= 29)
    return 1;
}