#ifndef SERVO_H
#define SERVO_H

#include <Arduino.h>
#include <config.h>
#include <ESP32Servo.h>


extern float thetaOffset;
extern float gamma_;
extern float Tmed;

void initSlidingWindow();
void setServoAngle(int angle);
float calculateServoAngle(
  float I,          // Light intensity (normalized 0.0 to 1.0)
  float T,          // Current temperature in °C
  float ts,         // Sampling interval in seconds
  float tu,         // Upload/sending interval in seconds
  float thetaOffset,// Minimum angle (θoffset)
  float gamma,      // Controlling factor (γ)
  float Tmed        // Ideal storage temperature (Tmed)
);
void updateServoFromParameters();

#endif // SERVO_H