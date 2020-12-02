#include "main.hpp"

TaskHandle_t DrawTask;

GFX gfx = GFX(MATRIX_SIZE, MATRIX_SIZE);

void setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println("Hello");

  gfx.init(50);

  Ps3.begin(MAC_ADDRESS);
  delay(200);

  waitForConnect();

  gfx.clear();
  gfx.drawLine(8, 0, 8, 15, CRGB::White);
  gfx.show();
}

void loop()
{
  if (Ps3.isConnected())
  {
    Serial.println("Connected");
  }
  else
  {
    Serial.println("Disconnected");
  }

  delay(100);
}

void waitForConnect()
{

  bool anim = false;
  while (!Ps3.isConnected())
  {
    anim = !anim;
    gfx.clear();
    if (anim)
      gfx.drawString("?", 6, 0, CRGB::White);
    gfx.drawBitmap(controller_bmp, 16, 9, 0, 6, CRGB::White);
    gfx.show();

    delay(500);
  }
}