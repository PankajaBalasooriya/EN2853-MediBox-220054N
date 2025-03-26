#ifndef MENU_H
#define MENU_H

#include <Arduino.h>
#include "display.h"
#include "config.h"


// Menu page enumeration
enum MenuPage {
    MAIN_MENU,
    SET_TIMEZONE,
    SET_ALARMS,
    VIEW_ALARMS,
    DELETE_ALARM,
    TEMPERATURE_HUMIDITY,
    SETTINGS
};

class Menu {
private:
    MenuPage currentPage;
    int selectedMenuItem;
    int timeZoneOffset;
    
    // Alarm-related variables
    struct Alarm {
        bool isActive;
        int hours;
        int minutes;
    };
    Alarm alarms[2];

    // Private methods for menu navigation and handling
    void displayMainMenu();
    void displaySetTimezone();
    void displaySetAlarms();
    void displayViewAlarms();
    void displayDeleteAlarm();
    void displayTemperatureHumidity();
    void displaySettings();

    // Interrupt handlers
    static void IRAM_ATTR handleUpButton();
    static void IRAM_ATTR handleDownButton();
    static void IRAM_ATTR handleOkButton();
    static void IRAM_ATTR handleCancelButton();

public:
    Menu();
    void init();
    void processMenu();
    
    // Getter and setter methods
    MenuPage getCurrentPage() const;
    void setCurrentPage(MenuPage page);
    
    // Alarm management methods
    void setAlarm(int alarmIndex, int hours, int minutes);
    void deleteAlarm(int alarmIndex);
    void viewAlarms();
    
    // Timezone management
    void setTimeZone(int offset);
    int getTimeZoneOffset() const;
};

extern Menu menu;  // Global menu object

#endif // MENU_H