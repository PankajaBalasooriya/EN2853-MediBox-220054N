#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHTesp.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

#include "Menu.h"
#include "config.h"
#include "buzzer.h"
#include "display.h"
#include "clock.h" 
#include "Menu.h"
#include "DHT22.h"
#include "LDR.h"


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

extern volatile uint8_t uiMode;

char tempArr[6];


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHTesp dhtSensor;


WiFiClient espClient;
PubSubClient mqttClient(espClient);

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

bool isScheduledON = false;

unsigned long scheduledOnTime;

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
  
  uiMode = MODE_CLOCK;

}

void connectToBroker(){
  while(!mqttClient.connected()){
    Serial.print("Attempting MQTT connection...");
    if( mqttClient.connect("MediBox-220054N")){
      Serial.println("Connected..");
      mqttClient.subscribe("ENTC-220054N-ON-OFF");
      mqttClient.subscribe("ENTC-220054N-SCH-ON");
    }
    else{
      Serial.print("Failed ");
      Serial.print(mqttClient.state());
      delay(5000);
    }
  }
}

unsigned long previousMillisSampleLight = 0;
unsigned long previousMillisSendLightData = 0;
//unsigned long previousMillis1Sec = 0;


void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  if(uiMode == MODE_CLOCK){
    update_time_with_check_alarm();
  }
  
  checkButtons();

  if(!mqttClient.connected()){
    connectToBroker();
  }
  
  mqttClient.loop();

  if (currentMillis - previousMillisSampleLight >= LDRSamplingIntervalMilis) {
    previousMillisSampleLight = currentMillis;
    readLightIntensity();
  }

  if (currentMillis - previousMillisSendLightData >= averagingTimePeriodMillis) {
    previousMillisSendLightData = currentMillis;
    Serial.print("Temp Data Sent.");
    String(AverageLightIntensity(), 2).toCharArray(tempArr, 6);
    mqttClient.publish("ENTC-TEMP-220054N", tempArr);
  }

  //Serial.println(averagingTimePeriodMillis);


  
  
  // TempAndHumidity data = get_DHT11_Data();
  // String(data.temperature, 2).toCharArray(tempArr, 6);
  // //Serial.println(tempArr);
  // mqttClient.publish("ENTC-TEMP-220054N", tempArr);

  checkSchedule();

  //delay(1000);
  
}