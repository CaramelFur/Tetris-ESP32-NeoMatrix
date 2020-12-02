#include "main.hpp"

TaskHandle_t DrawTask;

GFX graphics = GFX(MATRIX_SIZE, MATRIX_SIZE);

void setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println("Hello");

  graphics.init(50);

  Ps3.begin((char *)MAC_ADDRESS);
  delay(200);

  waitForConnect();

  Serial.println("Successfully connected to controller, starting game!");

  StartGame(&graphics);
}

void loop()
{
  delay(100);
}

void waitForConnect()
{

  bool anim = false;
  while (!Ps3.isConnected())
  {
    anim = !anim;
    graphics.clear();
    if (anim)
      graphics.drawChar('?', 6, 0, CRGB::White);
    graphics.drawBitmap(controller_bmp, 16, 9, 0, 6, CRGB::White);
    graphics.show();

    delay(500);
  }
}
