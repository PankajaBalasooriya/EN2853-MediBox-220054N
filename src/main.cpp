#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include "Menu.h"
#include "config.h"
#include "buzzer.h"
#include "display.h"
#include "clock.h" 
#include "Menu.h"
#include "DHT22.h"


extern int days;
extern int hours;
extern int minutes;
extern int seconds;

extern unsigned long timeNow;
extern unsigned long timeLast;

extern bool alarm_enabled;
extern int n_alarms;
extern int alarm_hours[];
extern int alarm_minutes[];
extern bool alarm_triggered[];

extern volatile uint8_t uiMode;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;






void setup() {
  display_init();

  setupWiFi();

  configTime(UTC_OFFSET_LK, UTC_OFFSET_DST_LK, NTP_SERVER);
  
  initPins();

  dhtSensor.setup(DHT11_DATA_PIN ,DHTesp::DHT22);

  Serial.begin(9600);

  initAlarmUI();
  
  uiMode = MODE_CLOCK;

}

void loop() {
  // put your main code here, to run repeatedly:

  if(uiMode == MODE_CLOCK){
    update_time_with_check_alarm();
  }
  
  checkButtons();
  
  
}