#include <M5Stack.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "SHT3X.h"

#include <Ambient.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include "settings.h"

//#define DEBUG
#define ONE_WIRE_BUS 2 // DS18B20 on arduino pin2 corresponds to D4 on physical board
#ifdef DEBUG
#define INTERVAL 1000 //measurement interval [ms]
#else
#define INTERVAL 10000 //measurement interval [ms]
#endif


SHT3X sht30;
Adafruit_BMP280 bme;

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

Ambient ambient;
WiFiClient client;
unsigned int channelId = CHANNEL_ID; //Channel ID of Ambient
const char* writeKey = WRITE_KEY; //Write key of Ambient
const char* ssid = SSID;
const char* password = PASSWORD;

float get_water_temp(void){
#ifdef DEBUG 
  return 16.0;
#endif

  DS18B20.requestTemperatures();
  float celsius = DS18B20.getTempCByIndex(0);
  return celsius;
}

float get_sauna_temp(void){
#ifdef DEBUG 
  return 85.0;
#endif
  float tmp;
  if(sht30.get()==0){
    tmp = sht30.cTemp;
  }
  return tmp;
}

float get_sauna_humid(void){
#ifdef DEBUG 
  return 10.0;
#endif
  float humid;
  if(sht30.get()==0){
    humid = sht30.humidity;
  }
  return humid;
}

void setup() {
  M5.begin();

  //init Lcd
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Lcd.setTextSize(2);
  M5.Lcd.print("M5 sauna monitor\r\n");

  //setup WiFi
  WiFi.begin(ssid, password);
  int cnt = 100;
  while (WiFi.status() != WL_CONNECTED) { 
    delay(100);
    M5.Lcd.print(".");
    cnt--;
    assert(cnt>0);
  }
  M5.Lcd.print(SSID);
  M5.Lcd.print(" WiFi connected\r\nIP address: ");
  M5.Lcd.println(WiFi.localIP());

  //setup ambient
  ambient.begin(channelId, writeKey, &client);

#ifdef DEBUG
  return;
#endif

  //init sensors
  DS18B20.begin();
  assert(1 == DS18B20.getDS18Count());

}

void loop() {
  float water_temp = get_water_temp();
  float sauna_temp = get_sauna_temp();
  float sauna_humid = get_sauna_humid();

  //show sensor data on LCD
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.print("water bath\r\n");
  M5.Lcd.printf("  temperature :  %2.1f \r\n", water_temp);
  M5.Lcd.print("sauna\r\n");
  M5.Lcd.printf("  temperature :  %2.1f \r\n", sauna_temp);
  M5.Lcd.printf("  humidity    :  %2.1f \r\n", sauna_humid);

  //send data to ambient
  ambient.set(1, (double)water_temp);
  ambient.set(2, (double)sauna_temp);
  ambient.set(3, (double)sauna_humid);

  ambient.send();

  delay(INTERVAL);
}
