#include "Menu.h"
#include "display.h"

// Global menu object
Menu menu;

Menu::Menu() : 
    currentPage(MAIN_MENU), 
    selectedMenuItem(0), 
    timeZoneOffset(0) 
{
    // Initialize alarms as inactive
    for (int i = 0; i < 2; i++) {
        alarms[i].isActive = false;
        alarms[i].hours = 0;
        alarms[i].minutes = 0;
    }
}

void Menu::init() {
    // Configure button pins with internal pull-up resistors
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_OK, INPUT_PULLUP);
    pinMode(BTN_CANCEL, INPUT_PULLUP);

    // Attach interrupts
    attachInterrupt(digitalPinToInterrupt(BTN_UP), handleUpButton, FALLING);
    attachInterrupt(digitalPinToInterrupt(BTN_DOWN), handleDownButton, FALLING);
    attachInterrupt(digitalPinToInterrupt(BTN_OK), handleOkButton, FALLING);
    attachInterrupt(digitalPinToInterrupt(BTN_CANCEL), handleCancelButton, FALLING);

    // Initial display
    displayMainMenu();
}

void Menu::displayMainMenu() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    const char* menuItems[] = {
        "Set Timezone",
        "Set Alarms",
        "View Alarms",
        "Delete Alarm",
        "Temp & Humidity",
        "Settings"
    };
    const int numItems = sizeof(menuItems) / sizeof(menuItems[0]);

    for (int i = 0; i < numItems; i++) {
        display.setCursor(10, 10 + (i * 10));
        if (i == selectedMenuItem) {
            display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
        } else {
            display.setTextColor(SSD1306_WHITE);
        }
        display.print(menuItems[i]);
    }
    display.display();
}

void Menu::displaySetTimezone() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Set Timezone");
    display.println("Current Offset: " + String(timeZoneOffset));
    display.println("Use UP/DOWN to adjust");
    display.display();
}

void Menu::displaySetAlarms() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Set Alarm");
    
    for (int i = 0; i < 2; i++) {
        display.print("Alarm " + String(i+1) + ": ");
        if (alarms[i].isActive) {
            display.printf("%02d:%02d\n", alarms[i].hours, alarms[i].minutes);
        } else {
            display.println("Inactive");
        }
    }
    display.display();
}

void Menu::displayViewAlarms() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Active Alarms:");
    
    bool hasActiveAlarms = false;
    for (int i = 0; i < 2; i++) {
        if (alarms[i].isActive) {
            display.printf("Alarm %d: %02d:%02d\n", i+1, alarms[i].hours, alarms[i].minutes);
            hasActiveAlarms = true;
        }
    }
    
    if (!hasActiveAlarms) {
        display.println("No active alarms");
    }
    display.display();
}

void Menu::displayDeleteAlarm() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Delete Alarm");
    
    for (int i = 0; i < 2; i++) {
        display.print("Alarm " + String(i+1) + ": ");
        if (alarms[i].isActive) {
            display.printf("%02d:%02d\n", alarms[i].hours, alarms[i].minutes);
        } else {
            display.println("Inactive");
        }
    }
    display.display();
}

void Menu::displayTemperatureHumidity() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Temp & Humidity");
    // Placeholder for actual temperature and humidity display
    display.println("Temp: --°C");
    display.println("Humidity: --%");
    display.display();
}

void Menu::displaySettings() {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Settings");
    // Placeholder for additional settings
    display.display();
}

void Menu::processMenu() {
    // Process menu logic based on current page
    switch (currentPage) {
        case MAIN_MENU:
            displayMainMenu();
            break;
        case SET_TIMEZONE:
            displaySetTimezone();
            break;
        case SET_ALARMS:
            displaySetAlarms();
            break;
        case VIEW_ALARMS:
            displayViewAlarms();
            break;
        case DELETE_ALARM:
            displayDeleteAlarm();
            break;
        case TEMPERATURE_HUMIDITY:
            displayTemperatureHumidity();
            break;
        case SETTINGS:
            displaySettings();
            break;
    }
}

// Interrupt Service Routines (ISRs)
void IRAM_ATTR Menu::handleUpButton() {
    // Decrement selected menu item
    if (menu.selectedMenuItem > 0) {
        menu.selectedMenuItem--;
    }
    menu.processMenu();
}

void IRAM_ATTR Menu::handleDownButton() {
    // Increment selected menu item
    const int maxMenuItems = 6;  // Adjust based on number of menu items
    if (menu.selectedMenuItem < maxMenuItems - 1) {
        menu.selectedMenuItem++;
    }
    menu.processMenu();
}

void IRAM_ATTR Menu::handleOkButton() {
    // Handle OK button press based on current page
    switch (menu.currentPage) {
        case MAIN_MENU:
            // Navigate to selected submenu
            menu.currentPage = static_cast<MenuPage>(menu.selectedMenuItem + 1);
            break;
        // Add more specific handling for other pages
    }
    menu.processMenu();
}

void IRAM_ATTR Menu::handleCancelButton() {
    // Return to main menu or previous page
    if (menu.currentPage != MAIN_MENU) {
        menu.currentPage = MAIN_MENU;
    }
    menu.processMenu();
}

// Getter and setter methods
MenuPage Menu::getCurrentPage() const {
    return currentPage;
}

void Menu::setCurrentPage(MenuPage page) {
    currentPage = page;
}

void Menu::setAlarm(int alarmIndex, int hours, int minutes) {
    if (alarmIndex >= 0 && alarmIndex < 2) {
        alarms[alarmIndex].isActive = true;
        alarms[alarmIndex].hours = hours;
        alarms[alarmIndex].minutes = minutes;
    }
}

void Menu::deleteAlarm(int alarmIndex) {
    if (alarmIndex >= 0 && alarmIndex < 2) {
        alarms[alarmIndex].isActive = false;
        alarms[alarmIndex].hours = 0;
        alarms[alarmIndex].minutes = 0;
    }
}

void Menu::setTimeZone(int offset) {
    timeZoneOffset = offset;
}

int Menu::getTimeZoneOffset() const {
    return timeZoneOffset;
}