#include "gfx.hpp"

GFX::GFX(uint8_t width, uint8_t height) : width(width), height(height), ledCount(height * width)
{
  this->pixels = new CRGB[ledCount];
}

void GFX::init()
{
  FastLED.addLeds<NEOPIXEL, LED_PIN>(this->pixels, this->width * this->height);
  FastLED.setCorrection(TypicalLEDStrip);
  this->fill(CRGB::Black);
  FastLED.show();
}

void GFX::init(uint8_t brightness)
{
  FastLED.setBrightness(brightness);
  return this->init();
}

void GFX::fill(CRGB color)
{
  for (uint16_t i = 0; i < this->ledCount; i++)
  {
    this->pixels[i] = color;
  }
}

void GFX::clear()
{
  this->fill(CRGB::Black);
}

void GFX::setPixel(uint8_t x, uint8_t y, CRGB color)
{
  this->pixels[this->XY(x, y)] = color;
}

uint16_t GFX::XY(uint8_t x, uint8_t y)
{
  uint8_t reverseY = (this->height - 1) - y;

  if (reverseY & 0x01)
    return (reverseY * this->width) + x;
  
  uint8_t reverseX = (this->width - 1) - x;
  return (reverseY * this->width) + reverseX;
}

void GFX::setBrightness(uint8_t scale)
{
  FastLED.setBrightness(scale);
}

void GFX::show()
{
  FastLED.show();
}