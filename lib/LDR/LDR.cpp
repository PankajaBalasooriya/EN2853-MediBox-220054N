#include "LDR.h"


float cumalativeLDRReadings = 0.0f;
int numberOfReadings = 60 * averagingTimePeriodMinutes / LDRSamplingInterval;
float CurrentLightIntensity = 0;


float readLightIntensity() {
  // Read the raw analog value from the LDR
  int rawValue = analogRead(LDR_PIN);
  
  // Normalize the value to a range between 0 and 1
  float normalized = 1.0f - (float)rawValue / (ADC_RESOLUTION - 1);
  
  cumalativeLDRReadings += normalized;
  return normalized;
}

float AverageLightIntensity(){
    float averageIntensity = cumalativeLDRReadings / numberOfReadings;
    cumalativeLDRReadings = 0.0f;
    CurrentLightIntensity = averageIntensity;
    return averageIntensity;
}

void calculateNumberofReadings(){
    cumalativeLDRReadings = 0.0f;
    numberOfReadings = 60 * averagingTimePeriodMinutes / LDRSamplingInterval;
    averagingTimePeriodMillis = int(averagingTimePeriodMinutes * 60000);
    LDRSamplingIntervalMilis = LDRSamplingInterval * 1000;
}

void setLightSamplingInterval(int t){
    LDRSamplingInterval = t;
    calculateNumberofReadings();
}

void setAveragingTImePeriod(float t){
    averagingTimePeriodMinutes = t;
    calculateNumberofReadings();
}
