#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>



#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

extern Adafruit_SSD1306 display;

extern int days;
extern int hours;
extern int minutes;
extern int seconds;

extern float currentTemperature;

void display_init();
void print_line(String text, int text_size, int row, int column);
void print_time_now();
void UpdateWeatherData();

#endif // DISPLAY_H