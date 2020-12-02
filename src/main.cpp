#include "main.hpp"

TaskHandle_t DrawTask;

GFX gfx = GFX(MATRIX_SIZE, MATRIX_SIZE);

void setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println("Hello");

  gfx.init(50);

  Ps3.attach(notify);
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
    ESP.restart();
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

void notify()
{
  // Runs on core 0
  //--------------- Digital D-pad button events --------------
  if (Ps3.event.button_down.up)
    Serial.println("Started pressing the up button");
  if (Ps3.event.button_up.up)
    Serial.println("Released the up button");

  if (Ps3.event.button_down.right)
    Serial.println("Started pressing the right button");
  if (Ps3.event.button_up.right)
    Serial.println("Released the right button");

  if (Ps3.event.button_down.down)
    Serial.println("Started pressing the down button");
  if (Ps3.event.button_up.down)
    Serial.println("Released the down button");

  if (Ps3.event.button_down.left)
    Serial.println("Started pressing the left button");
  if (Ps3.event.button_up.left)
    Serial.println("Released the left button");

  //---------- Digital select/start/ps button events ---------

  if (Ps3.event.button_down.start)
    Serial.println("Started pressing the start button");
  if (Ps3.event.button_up.start)
    Serial.println("Released the start button");

  //---------------------- Battery events ---------------------
  //if (Ps3.data.status.battery == ps3_status_battery_low)
  //  Serial.println("LOW");
}
