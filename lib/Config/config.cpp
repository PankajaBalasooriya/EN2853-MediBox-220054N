#include "config.h"
#include "display.h"
#include "Adafruit_GFX.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>




void initPins(){
    pinMode(LED_1, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(DHT11_DATA_PIN, INPUT);
}



void displayWiFiStatus(bool connected) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    
    if (connected) {
      // Display WiFi bitmap when connecting
      display.drawBitmap(
        (SCREEN_WIDTH - 128) / 2,  // Center the bitmap horizontally
        (SCREEN_HEIGHT - 64) / 2,  // Center the bitmap vertically
        wifi_bitmap, 
        128, 64, 
        SSD1306_WHITE
      );
      display.setCursor(0, SCREEN_HEIGHT - 10);
      display.print("Connecting to WiFi...");
    } else {
      display.setCursor(0, 0);
      display.println("WiFi Connection");
      display.println("Failed!");
      display.println("");
      display.println("Check:");
      display.println("- SSID");
      display.println("- Password");
      display.println("- Network");
    }
    
    display.display();
  }
  
  void setupWiFi() {
    // Clear display before WiFi connection attempt
    display.clearDisplay();
    display.display();
  
    // Attempt to connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
    // Show connecting status with bitmap
    displayWiFiStatus(true);
  
    // Wait for WiFi connection with timeout
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 20) {
      delay(500);
      attempts++;
    }
  
    if (WiFi.status() != WL_CONNECTED) {
      // WiFi connection failed
      displayWiFiStatus(false);
      delay(3000);  // Show error for 3 seconds
      return;
    }
  
    // WiFi connected successfully
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 0);
    display.println("WiFi Connected!");
    display.print("IP: ");
    display.println(WiFi.localIP());
    display.display();
    delay(500);
  }
  