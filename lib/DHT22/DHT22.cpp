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



// Add this to DHT22.cpp
void monitor_temp_humidity() {
  TempAndHumidity data = get_DHT11_Data();
  bool warning = false;
  
  display.clearDisplay();
  
  // Temperature warning check
  if (data.temperature < 24.0) {
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("WARNING!");
      display.setCursor(0, 10);
      display.println("Temperature too LOW");
      display.setCursor(0, 20);
      display.print(data.temperature, 1);
      display.print(" C (< 24 C)");
      warning = true;
      
  } 
  else if (data.temperature > 32.0) {
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 0);
      display.println("WARNING!");
      display.setCursor(0, 10);
      display.println("Temperature too HIGH");
      display.setCursor(0, 20);
      display.print(data.temperature, 1);
      display.print(" C (> 32 C)");
      warning = true;
  }

  
  // Humidity warning check
  if (data.humidity < 65.0) {
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 35);
      display.println("WARNING!");
      display.setCursor(0, 45);
      display.println("Humidity too LOW");
      display.setCursor(0, 55);
      display.print(data.humidity, 1);
      display.print("% (< 65%)");
      warning = true;
      
  } 
  else if (data.humidity > 80.0) {
      display.setTextSize(1);
      display.setTextColor(SSD1306_WHITE);
      display.setCursor(0, 35);
      display.println("WARNING!");
      display.setCursor(0, 45);
      display.println("Humidity too HIGH");
      display.setCursor(0, 55);
      display.print(data.humidity, 1);
      display.print("% (> 80%)");
      warning = true; 
  }

  
  // Sound alert if there's a warning
  if (warning) {
      display.display();
      // Play a warning tone
      tone(BUZZER, 1000, 500);
      delay(700);
      noTone(BUZZER);
      
      // Wait for button press to acknowledge
      unsigned long startTime = millis();
      while (millis() - startTime < 20000) { // Show warning for 20 seconds or until button press
          if (digitalRead(BTN_OK_PIN) == LOW) {
              break;
          }
          delay(100);
      }
  }
  
  // Return to normal display if warning was shown
  if (warning) {
      display.clearDisplay();
  }
}