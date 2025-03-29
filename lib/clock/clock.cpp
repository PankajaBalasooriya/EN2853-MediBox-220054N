// clock.cpp
#include "clock.h"
#include "display.h"
#include <time.h>
#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Define the variables here
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

unsigned long timeNow = 0;
unsigned long timeLast = 0;

bool alarm_enabled = true;
int n_alarms = 2;
int alarm_hours[] = {0, 0};
int alarm_minutes[] = {0, 0};
bool alarm_triggered[] = {false, false};




void update_time() {
    struct tm timeinfo;
    getLocalTime(&timeinfo);

    char timeHour[3];
    strftime(timeHour, 3, "%H", &timeinfo);
    hours = atoi(timeHour);

    char timeMinute[3];
    strftime(timeMinute, 3, "%M", &timeinfo);
    minutes = atoi(timeMinute);

    char timeSecond[3];
    strftime(timeSecond, 3, "%S", &timeinfo);
    seconds = atoi(timeSecond);

    char timeDay[3];
    strftime(timeDay, 3, "%d", &timeinfo);
    days = atoi(timeDay);
}

void update_time_with_check_alarm() {
    update_time();
    print_time_now();
    

    if(alarm_enabled) {
        for(int i = 0; i < n_alarms; i++) {
            if(!alarm_triggered[i] && 
               alarm_hours[i] == hours && 
               alarm_minutes[i] == minutes) {
                display.clearDisplay();
                print_line("MEDICINE TIME!", 1,0,2);    
                digitalWrite(LED_1, HIGH);

                ring_alarm();

                digitalWrite(LED_1 ,LOW);;
                display.clearDisplay(); 
                alarm_triggered[i] = true;
            }   
        }
    }
}

bool set_alarm(int alarm_index, int hour, int minute) {
    if(alarm_index < 0 || alarm_index >= n_alarms) {
        return false;
    }

    // Validate input
    if(hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        return false;
    }

    alarm_hours[alarm_index] = hour;
    alarm_minutes[alarm_index] = minute;
    alarm_triggered[alarm_index] = false;
    return true;
}

void delete_alarm(int alarm_index) {
    if(alarm_index >= 0 && alarm_index < n_alarms) {
        alarm_hours[alarm_index] = 0;
        alarm_minutes[alarm_index] = 0;
        alarm_triggered[alarm_index] = false;
    }
}

void snooze_alarm(int snooze_minutes) {  // Remove default argument here
  for(int i = 0; i < n_alarms; i++) {
      if(alarm_triggered[i]) {
          // Add snooze time
          alarm_minutes[i] += snooze_minutes;
          
          // Handle hour rollover
          if(alarm_minutes[i] >= 60) {
              alarm_hours[i]++;
              alarm_minutes[i] -= 60;
          }
          
          // Handle day rollover
          if(alarm_hours[i] >= 24) {
              alarm_hours[i] -= 24;
          }
          
          alarm_triggered[i] = false;
      }
  }
}

String get_time_string() {
    char timeStr[9];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);
    return String(timeStr);
}

