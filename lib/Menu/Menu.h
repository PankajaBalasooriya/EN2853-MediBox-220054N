#ifndef ALARM_UI_H
#define ALARM_UI_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "config.h"


// UI Modes
#define MODE_MAIN_MENU   0
#define MODE_SUBMENU     1
#define MODE_EXECUTE     2
#define MODE_CLOCK       3

// Main Menu Options
#define MAX_MAIN_MENU_OPTIONS 5



// Function Prototypes
void initAlarmUI();
void updateUI();
void checkButtons();
void updateMainMenuUI();
void executeMenuAction();

// Specific Menu Action Prototypes
void setTimeZone();
void addAlarm();
void viewAlarms();
void deleteAlarm();


#endif // ALARM_UI_H