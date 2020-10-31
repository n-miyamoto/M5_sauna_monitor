#include <M5Stack.h>

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(2);
}

void loop() {
  // put your main code here, to run repeatedly:
  M5.Lcd.print("Hello World\r\n");

  while(1);
}
