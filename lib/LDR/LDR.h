#ifndef LDR_H
#define LDR_H

#include <Arduino.h>
#include <config.h>

/**
 * Function to read the LDR value and normalize it to a range of 0 to 1
 * 
 * @return float - Normalized light intensity value between 0 and 1
 *         0 represents minimum intensity (darkness)
 *         1 represents maximum intensity (brightness)
 */
float readLightIntensity();


/**
 * Function to calculate the average light intensity based on accumulated readings
 * and reset the cumulative values
 * 
 * @return float - Average normalized light intensity value between 0 and 1
 */
float AverageLightIntensity();


/**
 * Function to calculate the number of readings based on the sampling interval and averaging period
 */
void calculateNumberOfReadings();


/**
 * Function to set the LDR sampling interval in seconds
 * 
 * @param t - Sampling interval in seconds
 */
void setLightSamplingInterval(int t);


/**
 * Function to set the averaging time period in minutes
 * 
 * @param t - Averaging time period in minutes
 */
void setAveragingTimePeriod(float t);

#endif // LDR_H