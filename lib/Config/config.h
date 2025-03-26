#ifndef CONFIG_H
#define CONFIG_H

#include "Arduino.h"

//--------Wifi Configuration------------------
#define WIFI_SSID "Wokwi-GUEST"
#define WIFI_PASSWORD ""
//--------Wifi Configuration------------------

//-------- NTP Clock -------------------------
#define NTP_SERVER     "pool.ntp.org"
// Set time zone (UTC+5:30 = 19800 seconds)
#define UTC_OFFSET_LK     19800
#define UTC_OFFSET_DST_LK 0


//-------- Pin Assignment ------------------
#define BUZZER 5
#define LED_1 15

#define BTN_CANCEL 35
#define BTN_OK 37
#define BTN_UP 36
#define BTN_DOWN 38

#define DHT11_DATA_PIN 12




void initPins();

#endif // CONFIG_H