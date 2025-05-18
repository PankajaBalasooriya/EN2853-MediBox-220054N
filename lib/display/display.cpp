#include "display.h"
#include "config.h"
#include "DHT22.h"
#include <Wire.h>

float currentTemperature = 0;
float currentHumidity = 0;

void display_init(){
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
        Serial.println(F("SSD1306 allocation failed!"));
        for(;;);
    }

    display.display();
    delay(4000);

    display.clearDisplay();


    print_line("Smart", 2, 10, 20); 
    print_line("MediBox!", 2, 30, 20);
    delay(2000);
    display.clearDisplay();
}

void print_line(String text, int text_size, int row, int column){
    display.setTextSize(text_size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(column, row);
    display.println(text);
    display.display();
}

// void print_time_now(){
//     display.clearDisplay();
    
//     // Set a larger text size for hours and minutes
//     display.setTextSize(2);
//     display.setTextColor(SSD1306_WHITE);
    
//     // Position for time
//     int centerX = SCREEN_WIDTH / 2;
//     int centerY = SCREEN_HEIGHT / 2;
    
//     // Create time string with leading zeros
//     char timeStr[10];
//     snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);
    
//     // Measure text width to center it
//     int16_t x1, y1;
//     uint16_t w, h;
//     display.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
    
//     // Center the time display
//     display.setCursor(centerX - (w/2), centerY - (h/2));
//     display.print(timeStr);
    
    
//     display.display();
// }


void print_time_now() {
    display.clearDisplay();
    
    // Set a larger text size for hours and minutes
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    
    // Create time string with leading zeros
    char timeStr[10];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);
    
    // Measure text width to center it
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
    
    // Center the time display at the top
    display.setCursor((SCREEN_WIDTH - w) / 2, 0);
    display.print(timeStr);
    
    // Switch to smaller text for date and sensor readings
    display.setTextSize(1);
    
    // Get current date information
    struct tm timeinfo;
    getLocalTime(&timeinfo);
    
    // Format and display date (DD/MM/YYYY)
    char dateStr[16];
    strftime(dateStr, sizeof(dateStr), "%d/%m/%Y", &timeinfo);
    
    // Center the date
    display.getTextBounds(dateStr, 0, 0, &x1, &y1, &w, &h);
    display.setCursor((SCREEN_WIDTH - w) / 2, 20);
    display.print(dateStr);
    
    // Add horizontal line separator
    display.drawLine(0, 30, SCREEN_WIDTH, 30, SSD1306_WHITE);
    
    // Get current temperature and humidity
    TempAndHumidity data = get_DHT11_Data();
    
    // Format and display temperature
    char tempStr[16];
    snprintf(tempStr, sizeof(tempStr), "Temp: %.1f C", currentTemperature);
    display.setCursor(5, 35);
    display.print(tempStr);

    
    // Format and display humidity
    char humStr[16];
    snprintf(humStr, sizeof(humStr), "Humidity: %.1f%%", currentHumidity);
    display.setCursor(5, 45);
    display.print(humStr);

    
    display.display();
}

void UpdateWeatherData(){
    TempAndHumidity data = get_DHT11_Data();

    currentTemperature = data.temperature;
    currentHumidity = data.humidity;
}