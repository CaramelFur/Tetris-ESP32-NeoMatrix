#include "game.hpp"

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

void StartGame(GFX *graphics)
{
  gfx = graphics;

  uint32_t prevLoop = esp_timer_get_time();
  uint64_t frame = 0;
  uint32_t second = 0;

  GetNextTetromino();

  while (true)
  {
    //Serial.println(((uint32_t)frame));
    // TODO: add pause feature

    GameLoop(frame, second);
    Draw(second);

    frame++;

    if (frame % TICKSPEED == 0)
      second++;

    while (esp_timer_get_time() < prevLoop + TICKDELAY)
    {
      NOP();
    }
    prevLoop = esp_timer_get_time();
  }
}

void GameLoop(uint64_t frame, uint32_t second)
{
  if (!Ps3.isConnected() || gameOver)
    return;

  HandleInput(frame);

  if (frame % 35 == 0)
  {
    if (IsValidMove(&currentTetromino.matrix, currentTetromino.x, currentTetromino.y + 1))
      currentTetromino.y += 1;
    else if (PlaceTetromino())
    {
      // Game over
      gameOver = true;
    }
  }
}

void HandleInput(uint64_t frame)
{
  if (Ps3.data.button.left)
  {
    if (presslength.left % INPUTSPEED == 0 && IsValidMove(&currentTetromino.matrix, currentTetromino.x - 1, currentTetromino.y))
      currentTetromino.x -= 1;
    presslength.left++;
  }
  else
  {
    presslength.left = 0;
  }

  if (Ps3.data.button.right)
  {
    if (presslength.right % INPUTSPEED == 0 && IsValidMove(&currentTetromino.matrix, currentTetromino.x + 1, currentTetromino.y))
      currentTetromino.x += 1;
    presslength.right++;
  }
  else
  {
    presslength.right = 0;
  }

  if (Ps3.data.button.down)
  {
    if (presslength.down % (TICKSPEED / 30) == 0)
    {
      if (IsValidMove(&currentTetromino.matrix, currentTetromino.x, currentTetromino.y + 1))
        currentTetromino.y += 1;
      else
        PlaceTetromino();
    }

    presslength.down++;
  }
  else
  {
    presslength.down = 0;
  }

  if (Ps3.data.button.up || Ps3.data.button.cross)
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
    toCurrentTetromino = tetrominos[esp_random() % TETROMINO_COUNT];
  else
    toCurrentTetromino = nextTetromino;

  pos_int_t col = PLAYFIELD_WIDTH / 2 - (toCurrentTetromino.size / 2 + toCurrentTetromino.size % 2);
  pos_int_t row = toCurrentTetromino.name == 'I' ? -1 : -2;

  currentTetromino = CurrentTetromino{
      .matrix = toCurrentTetromino,
      .x = col,
      .y = row,
  };

  nextTetromino = tetrominos[esp_random() % TETROMINO_COUNT];
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

  for (pos_int_t row = PLAYFIELD_HEIGHT - 1; row >= 0;)
  {
    bool rowFilled = true;
    for (pos_int_t x = 0; x < PLAYFIELD_WIDTH; x++)
      if (playfield[x][row] == 0)
        rowFilled = false;

    if (rowFilled)
    {
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

  GetNextTetromino();

  return false;
}

void Draw(uint32_t second)
{
  gfx->clear();

  DrawPlayField();

  DrawCurrentTetromino();

  if (gameOver)
  {
    // Replace with gameover logo
    gfx->drawPixel(MATRIX_SIZE - 1, 0, CRGB::Red);
  }

  // Keepalive pixel
  gfx->drawPixel(MATRIX_SIZE - 1, MATRIX_SIZE - 1, second % 2 ? CRGB::Red : CRGB::Black);

  if (gfx->isDirty())
    gfx->show();
}

void DrawPlayField()
{
  for (pos_int_t x = 0; x < PLAYFIELD_WIDTH; x++)
    for (pos_int_t y = 0; y < PLAYFIELD_HEIGHT; y++)
      gfx->drawPixel(x, y, TetrisColorMap[playfield[x][y]]);

  gfx->drawLine(PLAYFIELD_WIDTH, 0, PLAYFIELD_WIDTH, PLAYFIELD_HEIGHT - 1, CRGB::White);
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

  pos_int_t nextTetrominoOffset = ((MATRIX_SIZE - PLAYFIELD_WIDTH - 1) / 2 - nextTetromino.size / 2) + PLAYFIELD_WIDTH + 1;
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