#include "DHT22.h"


void check_temp(){
    TempAndHumidity data = get_DHT11_Data();
    
    if (data.temperature > 35){
      display.clearDisplay();
      print_line("TEMP HIGH", 0, 40, 1);
    }
    else if (data.temperature < 25){
      display.clearDisplay();
      print_line("TEMP LOW", 0, 40, 1);
    }
    if (data.humidity > 40){
      display.clearDisplay();
      print_line("HUMIDITY HIGH", 0, 50, 1);
    }
    else if (data.humidity  < 20){
      display.clearDisplay();
      print_line("HUMIDITY LOW", 0, 50, 1);
    }
}

TempAndHumidity get_DHT11_Data(){
    return dhtSensor.getTempAndHumidity();
}

float get_temp(){
    return get_DHT11_Data().temperature;
}

float get_humidity(){
    return get_DHT11_Data().humidity;
}