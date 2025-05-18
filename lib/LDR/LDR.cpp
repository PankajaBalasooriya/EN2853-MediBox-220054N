#include "LDR.h"
#include "MQTT.h"


float cumalativeLDRReadings = 0.0f;
uint16_t readingCount = 0;
float CurrentLightIntensity = 0;


float readLightIntensity() {
  // Read the raw analog value from the LDR
  int rawValue = analogRead(LDR_PIN);
  
  // Normalize the value to a range between 0 and 1
  float normalized = 1.0f - (float)rawValue / (ADC_RESOLUTION - 1);
  
  cumalativeLDRReadings += normalized;
  readingCount++;
  return normalized;
}

float AverageLightIntensity(){
    float averageIntensity = cumalativeLDRReadings / readingCount;
    cumalativeLDRReadings = 0.0f;
    readingCount = 0;
    CurrentLightIntensity = averageIntensity;
    return averageIntensity;
}


void setLightSamplingInterval(int t){
    LDRSamplingInterval = t;
}

void setAveragingTImePeriod(float t){
    averagingTimePeriodMinutes = t;
}
