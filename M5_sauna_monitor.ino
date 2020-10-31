#include <M5Stack.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 2 // DS18B20 on arduino pin2 corresponds to D4 on physical board
#define INTERVAL 1000 //measurement interval [ms]

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

float get_water_temp(void){
  DS18B20.requestTemperatures();
  float celsius = DS18B20.getTempCByIndex(0);
  return celsius;
}

void setup() {
  M5.begin();
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(2);

  DS18B20.begin();
  assert(1 == DS18B20.getDS18Count());

  M5.Lcd.print("M5 sauna monitor\r\n");
}

void loop() {
  float water_temp = get_water_temp();
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf("water temp :  %2.1f", water_temp);

  delay(INTERVAL);
}
