#include "gfx.hpp"

const CRGB black = CRGB::Black;

// For drawing

void GFX::drawString(char *str, uint8_t x, uint8_t y, CRGB color = CRGB::White)
{
  uint8_t i = 0;
  while (i < 255 && str[i] != 0)
  {
    uint8_t findex = str[i] - 32;
    Serial.print("Findex: ");
    Serial.println(findex);
    this->drawBitmap((uint8_t *)font[findex], fontW, fontH, x + (i * (fontW)), y, color);
    i++;
  }
}

void GFX::drawBitmap(const uint8_t *bitmap, uint8_t width, uint8_t height, uint8_t x, uint8_t y, CRGB color)
{
  return GFX::drawBitmap((uint8_t *)bitmap, width, height, x, y, color);
}

void GFX::drawBitmap(uint8_t *bitmap, uint8_t width, uint8_t height, uint8_t x, uint8_t y, CRGB color = CRGB::White)
{
  for (uint8_t h = 0; h < height; h++)
  {
    for (uint8_t w = 0; w < width; w++)
    {
      uint8_t col = w / 8;
      uint8_t bit = 7 - (w % 8);

      if (bitmap[col * height + h] & (1 << bit))
      {
        this->drawPixel(x + w, y + h, color);
      }
    }
  }
}

void GFX::fill(CRGB color)
{
  for (uint16_t i = 0; i < this->ledCount; i++)
  {
    this->pixels[i] = color;
  }
}

void GFX::drawPixel(uint8_t x, uint8_t y, CRGB color)
{
  if (x < 0 || y < 0)
    return;
  if (x >= this->width || y >= this->height)
    return;
  this->pixels[this->XY(x, y)] = color;
}

void GFX::drawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, CRGB color)
{
  int16_t dx = x2 - x1;
  int16_t dy = y2 - y1;
  bool dir = dx > dy;

  double M = dir ? dy / dx : dx / dy;

  int16_t steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

  double cx = dir ? x1 : y1;
  double cy = dir ? y1 : x1;

  for (int16_t s = 0; s <= steps; s++)
  {
    if (dir)
    {
      this->drawPixel(round(cx - 0.01), round(cy - 0.01), color);
    }
    else
    {
      this->drawPixel(round(cy - 0.01), round(cx - 0.01), color);
    }

    if (M == 1)
    {
      cx += 1;
      cy += 1;
    }
    else if (M < 1)
    {
      cx += 1;
      cy += M;
    }
    else if (M > 1)
    {
      cx += 1 / M;
      cy += 1;
    }
  }
}

void GFX::drawOutlinedSquare(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, CRGB color)
{
  uint8_t xs = (x1 > x2) ? x2 : x1;
  uint8_t xl = (x1 > x2) ? x1 : x2;

  uint8_t ys = (y1 > y2) ? y2 : y1;
  uint8_t yl = (y1 > y2) ? y1 : y2;

  for (uint8_t x = xs; x < xl; x++)
  {
    this->drawPixel(x, ys, color);
    this->drawPixel(x, yl, color);
  }
  for (uint8_t y = ys; y < yl; y++)
  {
    this->drawPixel(xs, y, color);
    this->drawPixel(xl, y, color);
  }
}

void GFX::drawFilledSquare(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, CRGB color)
{
  uint8_t xs = (x1 > x2) ? x2 : x1;
  uint8_t xl = (x1 > x2) ? x1 : x2;

  uint8_t ys = (y1 > y2) ? y2 : y1;
  uint8_t yl = (y1 > y2) ? y1 : y2;

  for (uint8_t x = xs; x < xl; x++)
  {
    for (uint8_t y = ys; y < yl; y++)
    {
      this->drawPixel(x, y, color);
    }
  }
}

CRGB GFX::getPixel(uint8_t x, uint8_t y)
{
  if (x < 0 || y < 0)
    return CRGB::Black;
  if (x >= this->width || y >= this->height)
    return CRGB::Black;
  return this->pixels[this->XY(x, y)];
}

bool GFX::getPixelIsSet(uint8_t x, uint8_t y)
{
  return this->getPixel(x, y) == black;
}

// Operational

GFX::GFX(uint8_t width, uint8_t height) : width(width), height(height), ledCount(height * width)
{
  this->pixels = new CRGB[ledCount];
}

void GFX::init()
{
  FastLED.addLeds<NEOPIXEL, LED_PIN>(this->pixels, this->width * this->height);
  //FastLED.setCorrection(TypicalLEDStrip);
  this->fill(CRGB::Black);
  FastLED.show();
}

void GFX::init(uint8_t brightness)
{
  FastLED.setBrightness(brightness);
  return this->init();
}

void GFX::clear()
{
  this->fill(CRGB::Black);
}

uint16_t GFX::XY(uint8_t x, uint8_t y)
{
  uint8_t reverseY = (this->height - 1) - y;

  if (reverseY & 0x01)
    return (reverseY * this->width) + x;

  uint8_t reverseX = (this->width - 1) - x;
  uint16_t output = (reverseY * this->width) + reverseX;

  /*if (output >= this->ledCount)
    output = this->ledCount - 1;
  if (output < 0)
    output = 0;*/
  return output;
}

void GFX::setBrightness(uint8_t scale)
{
  FastLED.setBrightness(scale);
}

void GFX::show()
{
  FastLED.show();
}