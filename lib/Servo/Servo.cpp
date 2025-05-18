#include "Servo.h"
#include <math.h>
#include "LDR.h"

Servo SlidingWindow;

float thetaOffset = 30.0;  // default minimum angle
float gamma_ = 0.75;        // default controlling factor
float Tmed = 30.0;         // default ideal medicine temperature
extern float CurrentLightIntensity;
extern float currentTemperature;
extern int LDRSamplingInterval;
extern float averagingTimePeriodMinutes;


void initSlidingWindow(){

    // Allow allocation of all timers for servo
    ESP32PWM::allocateTimer(0);
    ESP32PWM::allocateTimer(1);
    ESP32PWM::allocateTimer(2);
    ESP32PWM::allocateTimer(3);

    // For a Standard 50hz servo
    SlidingWindow.setPeriodHertz(50);
    SlidingWindow.attach(SERVO_PIN, 500, 2500);

    setServoAngle(90);
}

void setServoAngle(int angle){
    if (angle >= 180) {
    angle = 180;
  }
  if (angle <= 0) {
    angle = 0;
  }
    SlidingWindow.write(angle);
}

float calculateServoAngle(
  float I,          // Light intensity (normalized 0.0 to 1.0)
  float T,          // Current temperature in °C
  float ts,         // Sampling interval in seconds
  float tu,         // Upload/sending interval in seconds
  float thetaOffset,// Minimum angle (θoffset)
  float gamma,      // Controlling factor (γ)
  float Tmed        // Ideal storage temperature (Tmed)
) {
  // Avoid division by zero or log of zero/negative
  if (tu <= 0 || ts <= 0 || Tmed <= 0) {
    return thetaOffset;
  }

  float angle = thetaOffset + (180.0 - thetaOffset) * I * gamma * log(ts / tu) * (T / Tmed);

  // Clamp angle between 0 and 180 degrees
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;

  return angle;
}

void updateServoFromParameters() {
  float angle = calculateServoAngle(
    CurrentLightIntensity,
    currentTemperature,
    LDRSamplingInterval,
    averagingTimePeriodMinutes * 60,
    thetaOffset,
    gamma_,
    Tmed
  );
  Serial.print("Servo Angle: ");
  Serial.println(int(angle));
  
  setServoAngle((int)angle);
}
//