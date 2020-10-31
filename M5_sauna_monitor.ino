#include <M5Stack.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "SHT3X.h"

#define ONE_WIRE_BUS 2 // DS18B20 on arduino pin2 corresponds to D4 on physical board
#define INTERVAL 1000 //measurement interval [ms]

SHT3X sht30;
Adafruit_BMP280 bme;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

float get_water_temp(void){
  DS18B20.requestTemperatures();
  float celsius = DS18B20.getTempCByIndex(0);
  return celsius;
}

float get_sauna_temp(void){
  float tmp;
  if(sht30.get()==0){
    tmp = sht30.cTemp;
  }
  return tmp;
}

float get_sauna_humid(void){
  float humid;
  if(sht30.get()==0){
    humid = sht30.humidity;
  }
  return humid;
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
  float sauna_temp = get_sauna_temp();
  float sauna_humid = get_sauna_humid();

  M5.Lcd.setCursor(0, 30);
  M5.Lcd.print("water bath\r\n");
  M5.Lcd.printf("  temperature :  %2.1f \r\n", water_temp);
  M5.Lcd.print("sauna\r\n");
  M5.Lcd.printf("  temperature :  %2.1f \r\n", sauna_temp);
  M5.Lcd.printf("  humidity    :  %2.1f \r\n", sauna_humid);

  delay(INTERVAL);
}
