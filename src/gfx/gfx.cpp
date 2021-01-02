#include "gfx.hpp"

const CRGB black = CRGB::Black;

// For drawing

void GFX::drawString(const char *str, pos_int_t x, pos_int_t y, CRGB color = CRGB::White)
{
  uint8_t i = 0;
  while (i < 255 && str[i] != 0)
  {
    this->drawChar(str[i], x + (i * (fontW)), y, color);
    i++;
  }
}

void GFX::drawChar(char chr, pos_int_t x, pos_int_t y, CRGB color)
{
  uint8_t findex = chr - 32;
  this->drawBitmap((uint8_t *)font[findex], fontW, fontH, x, y, color);
}

void GFX::drawBitmap(const uint8_t *bitmap, uint8_t width, uint8_t height, pos_int_t x, pos_int_t y, CRGB color)
{
  return this->drawBitmap(bitmap, width, height, x, y, 0, color);
}

void GFX::drawBitmap(const uint8_t *bitmap, uint8_t width, uint8_t height, pos_int_t x, pos_int_t y, int8_t rotate, CRGB color = CRGB::White)
{
  int8_t crotate = rotate % 4;
  if (crotate < 0)
    crotate += 4;

  for (uint8_t h = 0; h < height; h++)
  {
    for (uint8_t w = 0; w < width; w++)
    {
      uint8_t col = w / 8;
      uint8_t bit = 7 - (w % 8);

      uint8_t dx = (crotate == 2 || crotate == 3) ? width - 1 - w : w;
      uint8_t dy = (crotate == 1 || crotate == 2) ? height - 1 - h : h;

      if (bitmap[col * height + h] & (1 << bit))
      {
        if (crotate % 2 == 1)
          this->drawPixel(y + dy, x + dx, color);
        else
          this->drawPixel(x + dx, y + dy, color);
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
  this->dirty = true;
}

void GFX::drawPixel(pos_int_t x, pos_int_t y, CRGB color)
{
  if (x < 0 || y < 0)
    return;
  if (x >= this->width || y >= this->height)
    return;
  this->pixels[this->XY(x, y)] = color;
  this->dirty = true;
}

void GFX::drawLine(pos_int_t x1, pos_int_t y1, pos_int_t x2, pos_int_t y2, CRGB color)
{
  if (y1 == y2)
  {
    pos_int_t from = (x1 < x2) ? x1 : x2;
    pos_int_t to = (x1 < x2) ? x2 : x1;
    for (pos_int_t x = from; x <= to; x++)
    {
      this->drawPixel(x, y1, color);
    }
    return;
  }
  if (x1 == x2)
  {
    pos_int_t from = (y1 < y2) ? y1 : y2;
    pos_int_t to = (y1 < y2) ? y2 : y1;
    for (pos_int_t y = from; y <= to; y++)
    {
      this->drawPixel(x1, y, color);
    }
    return;
  }

  pos_int_t dx = x2 - x1;
  pos_int_t dy = y2 - y1;
  bool dir = dx > dy;

  double M = dir ? (double)dy / (double)dx : (double)dx / (double)dy;

  pos_int_t steps = (abs(dx) > abs(dy)) ? abs(dx) : abs(dy);

  double cx = dir ? x1 : y1;
  double cy = dir ? y1 : x1;

  for (pos_int_t s = 0; s <= steps; s++)
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

void GFX::drawOutlinedSquare(pos_int_t x1, pos_int_t y1, pos_int_t x2, pos_int_t y2, CRGB color)
{
  pos_int_t xs = (x1 > x2) ? x2 : x1;
  pos_int_t xl = (x1 > x2) ? x1 : x2;

  pos_int_t ys = (y1 > y2) ? y2 : y1;
  pos_int_t yl = (y1 > y2) ? y1 : y2;

  for (pos_int_t x = xs; x < xl; x++)
  {
    this->drawPixel(x, ys, color);
    this->drawPixel(x, yl, color);
  }
  for (pos_int_t y = ys; y < yl; y++)
  {
    this->drawPixel(xs, y, color);
    this->drawPixel(xl, y, color);
  }
}

void GFX::drawFilledSquare(pos_int_t x1, pos_int_t y1, pos_int_t x2, pos_int_t y2, CRGB color)
{
  pos_int_t xs = (x1 > x2) ? x2 : x1;
  pos_int_t xl = (x1 > x2) ? x1 : x2;

  pos_int_t ys = (y1 > y2) ? y2 : y1;
  pos_int_t yl = (y1 > y2) ? y1 : y2;

  for (pos_int_t x = xs; x <= xl; x++)
  {
    for (pos_int_t y = ys; y <= yl; y++)
    {
      this->drawPixel(x, y, color);
    }
  }
}

CRGB GFX::getPixel(pos_int_t x, pos_int_t y)
{
  if (x < 0 || y < 0)
    return CRGB::Black;
  if (x >= this->width || y >= this->height)
    return CRGB::Black;
  return this->pixels[this->XY(x, y)];
}

bool GFX::getPixelIsSet(pos_int_t x, pos_int_t y)
{
  return this->getPixel(x, y) == black;
}

// Operational

GFX::GFX(uint8_t width, uint8_t height) : width(width), height(height), ledCount(height * width)
{
  this->pixels = new CRGB[ledCount];
  this->dirty = true;
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

uint16_t GFX::XY(pos_int_t x, pos_int_t y)
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
  this->dirty = false;
}

bool GFX::isDirty()
{
  return this->dirty;
}