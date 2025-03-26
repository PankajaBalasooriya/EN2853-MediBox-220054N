#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
#include "config.h"
#include "buzzer.h"
#include "display.h"

// Declare variables as extern
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

// Time management functions
void update_time();
void update_time_with_check_alarm();
void print_time_now();

// New time zone and NTP functions
void set_time_zone(long offset_seconds);
bool sync_time_from_ntp();

// Alarm management functions
bool set_alarm(int alarm_index, int hour, int minute);
void delete_alarm(int alarm_index);
void snooze_alarm(int snooze_minutes = 5);  // Default argument stays in header

// Utility functions
String get_time_string();


#endif // CLOCK_H