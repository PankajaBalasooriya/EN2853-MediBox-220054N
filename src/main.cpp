#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>
#include <ESP32Servo.h>

#include "Menu.h"
#include "config.h"
#include "buzzer.h"
#include "display.h"
#include "clock.h" 
#include "Menu.h"
#include "DHT22.h"
#include "LDR.h"
#include "Servo.h"
#include "MQTT.h"


extern int days;
extern int hours;
extern int minutes;
extern int seconds;

extern unsigned long timeNow;
extern unsigned long timeLast;

extern bool alarm_enabled;
extern int n_alarms;
extern int alarm_hours[];
extern int alarm_minutes[];
extern bool alarm_triggered[];

bool isScheduledON = false;

unsigned long scheduledOnTime;

extern float currentTemperature;
extern float currentHumidity;

extern volatile uint8_t uiMode;

unsigned long previousMillisSampleLight = 0;
unsigned long previousMillisSendLightData = 0;
unsigned long previousMillisSendTempData = 0;

char tempArr[6];

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;

WiFiClient espClient;
PubSubClient mqttClient(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);



unsigned long getTime(){
  timeClient.update();
  return timeClient.getEpochTime();
}

void checkSchedule(){
  if(isScheduledON){
    unsigned long currentTime = getTime();
    if(currentTime > scheduledOnTime){
      //buzzer on
      isScheduledON = false;
      Serial.println("Scheduled On!");
    }
  }
}


void setup() {

  display_init();

  setupWiFi();
  
  timeClient.begin();
  timeClient.setTimeOffset(5.5 * 3600);
  

  configTime(UTC_OFFSET_LK, UTC_OFFSET_DST_LK, NTP_SERVER);
  
  initPins();
  analogReadResolution(ADC_RESOLUTION_BITS);

  dhtSensor.setup(DHT11_DATA_PIN ,DHTesp::DHT22);

  Serial.begin(9600);

  setupMqtt();

  initAlarmUI();

  initSlidingWindow();
  
  uiMode = MODE_CLOCK;

}
//

void loop() {
  
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  if(uiMode == MODE_CLOCK){
    update_time_with_check_alarm();
  }
  
  checkButtons();

  checkSchedule();

  if(!mqttClient.connected()){
    connectToBroker();
  }
  
  mqttClient.loop();

  if (currentMillis - previousMillisSampleLight >= LDRSamplingIntervalMilis) {
    previousMillisSampleLight = currentMillis;
    readLightIntensity();
    UpdateWeatherData();
    updateServoFromParameters();
  }

  if (currentMillis - previousMillisSendLightData >= averagingTimePeriodMillis) {
    previousMillisSendLightData = currentMillis;
    Serial.print("Light Data Sent.");
    String(AverageLightIntensity(), 2).toCharArray(tempArr, 6);
    mqttClient.publish("ENTC-220054N-LIGHT-INTENSITY", tempArr);
  }

  if (currentMillis - previousMillisSendTempData >= tempSendingIntervalMilis) {
    previousMillisSendTempData = currentMillis;
    String(currentTemperature, 2).toCharArray(tempArr, 6);
    mqttClient.publish("ENTC-TEMP-220054N", tempArr);

    String(currentHumidity, 2).toCharArray(tempArr, 6);
    mqttClient.publish("ENTC-HUMIDITY-220054N", tempArr);
  }
}