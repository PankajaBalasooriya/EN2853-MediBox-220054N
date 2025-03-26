#include "display.h"
#include "config.h"
#include "DHT22.h"
#include <Wire.h>

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

void print_time_now(){
    display.clearDisplay();
    
    // Set a larger text size for hours and minutes
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    
    // Position for time
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    
    // Create time string with leading zeros
    char timeStr[10];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);
    
    // Measure text width to center it
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(timeStr, 0, 0, &x1, &y1, &w, &h);
    
    // Center the time display
    display.setCursor(centerX - (w/2), centerY - (h/2));
    display.print(timeStr);
    
    
    display.display();
}

