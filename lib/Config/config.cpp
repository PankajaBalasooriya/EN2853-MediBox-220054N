#include "config.h"
#include "display.h"
#include "Adafruit_GFX.h"
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#include <PubSubClient.h>

//extern WiFiClient espClient;
extern PubSubClient mqttClient;
extern bool isScheduledON;
extern unsigned long scheduledOnTime;

void initPins(){
    pinMode(LED_1, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(DHT11_DATA_PIN, INPUT);
}

long convertHoursToSeconds(float hours) {
  return static_cast<long>(hours * 3600.0f);
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

  void reciveCallback(char* topic, byte* payload, unsigned int length){
    Serial.print("Message Arrived [");
    Serial.print(topic);
    Serial.print("] ");

    char payloadChar[length];
    for(int i = 0; i<length; i++){
      Serial.print((char)payload[i]);
      payloadChar[i] = (char)payload[i];
    }

    if(strcmp(topic, "ENTC-220054N-ON-OFF") == 0){
      if(payloadChar[0] == '1'){
        Serial.println("ON");
      }
      else{
        Serial.println("OFF");
      }
    }else if(strcmp(topic, "ENTC-220054N-SCH-ON") == 0){
      if(payloadChar[0] = 'N'){
        isScheduledON = false;
      }else{
        isScheduledON = true;
        scheduledOnTime =atol(payloadChar);
      }
    }
  }

  void setupMqtt(){
    mqttClient.setServer("test.mosquitto.org", 1883);
    mqttClient.setCallback(reciveCallback);
  }

  
  // Function to display the medicine pill bitmap with animation
  void display_medicine_icon() {
	  // Display the medicine bitmap in the center top of the screen
	  display.drawBitmap(
		  (SCREEN_WIDTH - 32) / 2,  // Center horizontally
		  2,                        // Position at top with small margin
		  medicineBitmap,           // Bitmap data
		  32,                       // Width
		  32,                       // Height
		  WHITE                     // Color
	  );
	  display.display();
	  
	  // Simple animation: Blink the icon
	  for (int i = 0; i < 3; i++) {
		  delay(300);
		  display.clearDisplay();
		  display.display();
		  delay(300);
		  display.drawBitmap(
			  (SCREEN_WIDTH - 32) / 2,
			  2,
			  medicineBitmap,
			  32, 32,
			  WHITE
		  );
		  display.display();
	  }
  }
  