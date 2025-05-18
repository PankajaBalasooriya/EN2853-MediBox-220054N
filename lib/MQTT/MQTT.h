#ifndef MQTT_H
#define MQTT_H

#include <Arduino.h>
#include <config.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

extern PubSubClient mqttClient;
extern bool isScheduledON;
extern unsigned long scheduledOnTime;

extern int LDRSamplingInterval;
extern unsigned long LDRSamplingIntervalMilis;
extern float averagingTimePeriodMinutes;
extern unsigned long averagingTimePeriodMillis;
extern unsigned long tempSendingIntervalMilis;

void connectToBroker();
void reciveCallback(char* topic, byte* payload, unsigned int length);
void setupMqtt();

#endif // MQTT_H