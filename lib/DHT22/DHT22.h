#ifndef DHT22_H
#define DHT22_H

#include <Arduino.h>
#include <DHTesp.h>
#include "config.h"
#include "display.h"
#include "Wire.h"
#include "Adafruit_GFX.h"
#include <Adafruit_SSD1306.h>



extern DHTesp dhtSensor;



void check_temp();
TempAndHumidity get_DHT11_Data();
float get_temp();
float get_humidity();

void monitor_temp_humidity();




#endif // DHT22_H