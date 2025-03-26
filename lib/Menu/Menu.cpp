#include "Menu.h"
#include "config.h"
#include "display.h"

// Global Variables
extern Adafruit_SSD1306 display;

volatile uint8_t currentMenuOption = 1;
volatile uint8_t previousMenuOption = 0;
volatile uint8_t uiMode = MODE_MAIN_MENU;
volatile uint8_t menuScrollIndex = 0;

// Debounce variables
unsigned long lastButtonPressTime = 0;
const unsigned long DEBOUNCE_DELAY = 200; // 200ms debounce time

// Main Menu Items
const char* mainMenu[MAX_MAIN_MENU_OPTIONS] = {
    "1: Set Time Zone",
    "2: Add Alarm",
    "3: View Alarms", 
    "4: Delete Alarm",
    "5: Exit"
};

void initAlarmUI() {


    // Setup Button Pins
    pinMode(BTN_UP_PIN, INPUT_PULLUP);
    pinMode(BTN_DOWN_PIN, INPUT_PULLUP);
    pinMode(BTN_OK_PIN, INPUT_PULLUP);
    pinMode(BTN_BACK_PIN, INPUT_PULLUP);

    // Initial UI Update
    //updateMainMenuUI();
}

void checkButtons() {
    unsigned long currentTime = millis();

    // Debounce check
    if (currentTime - lastButtonPressTime < DEBOUNCE_DELAY) {
        return;
    }

    // UP Button
    if (digitalRead(BTN_UP_PIN) == LOW) {
        if (uiMode == MODE_MAIN_MENU) {
            if (currentMenuOption > 1) {
                currentMenuOption--;
                
                // Adjust scroll index for pagination
                if (currentMenuOption % 3 == 0) {
                    if (menuScrollIndex > 0) {
                        menuScrollIndex--;
                    }
                }
            } else {
                // Wrap to bottom
                currentMenuOption = MAX_MAIN_MENU_OPTIONS;
                menuScrollIndex = (MAX_MAIN_MENU_OPTIONS - 1) / 3;
            }
            updateMainMenuUI();
            lastButtonPressTime = currentTime;
        }
    }
    // DOWN Button
    else if (digitalRead(BTN_DOWN_PIN) == LOW) {
        if (uiMode == MODE_MAIN_MENU) {
            if (currentMenuOption < MAX_MAIN_MENU_OPTIONS) {
                currentMenuOption++;
                
                // Adjust scroll index for pagination
                if (currentMenuOption % 3 == 1) {
                    if (menuScrollIndex < (MAX_MAIN_MENU_OPTIONS - 1) / 3) {
                        menuScrollIndex++;
                    }
                }
            } else {
                // Wrap to top
                currentMenuOption = 1;
                menuScrollIndex = 0;
            }
            updateMainMenuUI();
            lastButtonPressTime = currentTime;
        }
    }
    // OK/MENU Button
    else if (digitalRead(BTN_OK_PIN) == LOW) {
        if (uiMode == MODE_MAIN_MENU) {
            uiMode = MODE_EXECUTE;
            executeMenuAction();
            lastButtonPressTime = currentTime;
        }
        else if(uiMode == MODE_CLOCK){
            uiMode = MODE_MAIN_MENU;
            updateMainMenuUI();
            lastButtonPressTime = currentTime;
        }
    }
    // BACK/CANCEL Button
    else if (digitalRead(BTN_BACK_PIN) == LOW) {
        if (uiMode == MODE_EXECUTE) {
            uiMode = MODE_MAIN_MENU;
            updateMainMenuUI();
            lastButtonPressTime = currentTime;
        }
        else if(uiMode == MODE_MAIN_MENU){
            uiMode = MODE_CLOCK;
            lastButtonPressTime = currentTime;
        }
    }
}

void updateMainMenuUI() {
    display.clearDisplay();
    display.setCursor(40, 0);
    display.setTextSize(1);
    display.println("MediBox");

    // Display menu items with pagination and proper highlighting
    for (int i = 0; i < 3; i++) {
        int menuIndex = i + (menuScrollIndex * 3);
        if (menuIndex < MAX_MAIN_MENU_OPTIONS) {
            // If this is the currently selected menu item
            if (currentMenuOption == menuIndex + 1) {
                // Draw a filled rectangle for highlighting
                display.fillRect(0, 16 + (i * 16), SCREEN_WIDTH, 16, SSD1306_WHITE);
                
                // Set text color to black for the selected item
                display.setTextColor(SSD1306_BLACK);
            } else {
                // Reset to default white text color
                display.setTextColor(SSD1306_WHITE);
            }

            display.setCursor(10, 22 + (i * 16));
            display.println(mainMenu[menuIndex]);
        }
    }

    // Reset text color to default
    display.setTextColor(SSD1306_WHITE);
    display.display();
}

void executeMenuAction() {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);

    switch(currentMenuOption) {
        case 1:
            setTimeZone();
            break;
        case 2:
            addAlarm();
            break;
        case 3:
            viewAlarms();
            break;
        case 4:
            deleteAlarm();
            break;
        case 5:
            uiMode = MODE_CLOCK;
            break;
    }

    display.display();
}

// Placeholder implementations for menu actions
void setTimeZone() {
    display.println("Set Time Zone");
    display.println("Use +/- buttons");
    display.println("to adjust time zone");
}

void addAlarm() {
    display.println("Add New Alarm");
    display.println("Select hour, minute");
    display.println("and AM/PM");
}

void viewAlarms() {
    display.println("View Existing");
    display.println("Alarms");
}

void deleteAlarm() {
    display.println("Delete Alarm");
    display.println("Select alarm to");
    display.println("remove");
}