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



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;



void setup() {
  // put your setup code here, to run once:
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD, 6);

  configTime(UTC_OFFSET_LK, UTC_OFFSET_DST_LK, NTP_SERVER);
  
  initPins();

  Serial.begin(9600);

  display_init();

  //menu.init();

}

void loop() {
  // put your main code here, to run repeatedly:
  update_time_with_check_alarm();
  
  
}