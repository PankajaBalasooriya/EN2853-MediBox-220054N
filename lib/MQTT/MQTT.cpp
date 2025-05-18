#include "MQTT.h"
#include "LDR.h"
#include "Servo.h"

int LDRSamplingInterval = 5;
unsigned long LDRSamplingIntervalMilis = LDRSamplingInterval * milisInaSecond; 
float averagingTimePeriodMinutes = 2; 
unsigned long averagingTimePeriodMillis = int(averagingTimePeriodMinutes * millisInaMinute);
unsigned long tempSendingIntervalMilis = 5000;

void connectToBroker(){
  while(!mqttClient.connected()){
    Serial.print("Attempting MQTT connection...");
    if( mqttClient.connect("MediBox-220054N")){
      Serial.println("Connected..");
      mqttClient.subscribe("ENTC-220054N-ON-OFF");
      mqttClient.subscribe("ENTC-220054N-SCH-ON");
      mqttClient.subscribe("ENTC-220054N-LIGHT-SAMPLING-INTERVAL");
      mqttClient.subscribe("ENTC-220054N-LIGHT-DATA-SENDING-TIME");
      mqttClient.subscribe("ENTC-220054N-MIN-ANGLE");
      mqttClient.subscribe("ENTC-220054N-CONTROLLING-FACTOR");
      mqttClient.subscribe("ENTC-220054N-MED-TEMP");
    }
    else{
      Serial.print("Failed ");
      Serial.print(mqttClient.state());
      delay(5000);
    }
  }
}


  void reciveCallback(char* topic, byte* payload, unsigned int length){
    Serial.print("Message Arrived [");
    Serial.print(topic);
    Serial.print("] ");

    char payloadChar[length];
    for(int i = 0; i<length; i++){
      Serial.println((char)payload[i]);
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
    }else if(strcmp(topic, "ENTC-220054N-LIGHT-SAMPLING-INTERVAL") == 0){
      int samplingInterval = atoi(payloadChar);
      setLightSamplingInterval(samplingInterval);
    }else if(strcmp(topic, "ENTC-220054N-LIGHT-DATA-SENDING-TIME") == 0){
      int dataSendingTime = atoi(payloadChar);
      setAveragingTImePeriod(dataSendingTime);
    } else if(strcmp(topic, "ENTC-220054N-MIN-ANGLE") == 0){
      thetaOffset = atof(payloadChar);
      updateServoFromParameters();
      //setServoAngle((int)thetaOffset);
    } else if(strcmp(topic, "ENTC-220054N-CONTROLLING-FACTOR") == 0){
      gamma_ = atof(payloadChar);
      updateServoFromParameters();
    } else if(strcmp(topic, "ENTC-220054N-MED-TEMP") == 0){
      Tmed = atof(payloadChar);
      updateServoFromParameters();
    }
  }

  void setupMqtt(){
    mqttClient.setServer("broker.hivemq.com", 1883);
    mqttClient.setCallback(reciveCallback);
  }
//