#include "Menu.h"
#include "config.h"
#include "display.h"
#include "clock.h"

// Global Variables
extern Adafruit_SSD1306 display;

volatile uint8_t currentMenuOption = 1;
volatile uint8_t previousMenuOption = 0;
volatile uint8_t uiMode = MODE_MAIN_MENU;
volatile uint8_t menuScrollIndex = 0;

// Debounce variables
unsigned long lastButtonPressTime = 0;
const unsigned long DEBOUNCE_DELAY = 200; // 200ms debounce time

// Global time zone variables
const int NUM_TIME_ZONES = 51; // Total number of time zones including half-hour offsets
volatile int currentTimeZoneIndex = 23; 


// Time zone array with UTC offsets and names
const struct TimeZone {
    float offset;      // Hours from UTC (-12.0 to +14.0)
    const char* name;  // Descriptive name or representative city
    long utcOffsetSeconds; // Offset in seconds for configTime()
} timeZones[NUM_TIME_ZONES] = {
    // Negative offsets (West of UTC)
    {-12.0, "Baker Island, US", convertHoursToSeconds(-12.0)},
    {-11.0, "Midway Island, US", convertHoursToSeconds(-11.0)},
    {-10.0, "Hawaii", convertHoursToSeconds(-10.0)},
    {-9.5, "Marquesas Islands", convertHoursToSeconds(-9.5)},
    {-9.0, "Alaska", convertHoursToSeconds(-9.0)},
    {-8.0, "Pacific Time (US & Canada)", convertHoursToSeconds(-8.0)},
    {-7.0, "Mountain Time (US & Canada)", convertHoursToSeconds(-7.0)},
    {-6.0, "Central Time (US & Canada)", convertHoursToSeconds(-6.0)},
    {-5.0, "Eastern Time (US & Canada)", convertHoursToSeconds(-5.0)},
    {-4.5, "Venezuela", convertHoursToSeconds(-4.5)},
    {-4.0, "Atlantic Time (Canada)", convertHoursToSeconds(-4.0)},
    {-3.5, "Newfoundland, Canada", convertHoursToSeconds(-3.5)},
    {-3.0, "Buenos Aires, Argentina", convertHoursToSeconds(-3.0)},
    {-2.0, "Mid-Atlantic", convertHoursToSeconds(-2.0)},
    {-1.0, "Azores, Cape Verde", convertHoursToSeconds(-1.0)},
    
    // UTC
    {0.0, "UTC/London", 0},
    
    // Positive offsets (East of UTC)
    {1.0, "Paris, Berlin", convertHoursToSeconds(1.0)},
    {2.0, "Athens, Cairo", convertHoursToSeconds(2.0)},
    {3.0, "Moscow, Nairobi", convertHoursToSeconds(3.0)},
    {3.5, "Tehran, Iran", convertHoursToSeconds(3.5)},
    {4.0, "Dubai, Abu Dhabi", convertHoursToSeconds(4.0)},
    {4.5, "Kabul, Afghanistan", convertHoursToSeconds(4.5)},
    {5.0, "Islamabad, Karachi", convertHoursToSeconds(5.0)},
    {5.5, "Mumbai, New Delhi", convertHoursToSeconds(5.5)},
    {5.75, "Kathmandu, Nepal", convertHoursToSeconds(5.75)},
    {6.0, "Dhaka, Bangladesh", convertHoursToSeconds(6.0)},
    {6.5, "Yangon, Myanmar", convertHoursToSeconds(6.5)},
    {7.0, "Bangkok, Jakarta", convertHoursToSeconds(7.0)},
    {8.0, "Beijing, Singapore", convertHoursToSeconds(8.0)},
    {8.75, "Eucla, Western Australia", convertHoursToSeconds(8.75)},
    {9.0, "Tokyo, Seoul", convertHoursToSeconds(9.0)},
    {9.5, "Darwin, Central Australia", convertHoursToSeconds(9.5)},
    {10.0, "Sydney, Melbourne", convertHoursToSeconds(10.0)},
    {10.5, "Lord Howe Island, Australia", convertHoursToSeconds(10.5)},
    {11.0, "Solomon Islands", convertHoursToSeconds(11.0)},
    {11.5, "Norfolk Island", convertHoursToSeconds(11.5)},
    {12.0, "New Zealand", convertHoursToSeconds(12.0)},
    {12.75, "Chatham Islands, NZ", convertHoursToSeconds(12.75)},
    {13.0, "Tonga", convertHoursToSeconds(13.0)},
    {14.0, "Kiribati (Line Islands)", convertHoursToSeconds(14.0)}
};




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

void setTimeZone() {
    bool selectingTimeZone = true;
    unsigned long lastUpdateTime = 0;
    unsigned long currentTime;

    // Clear display and set initial state
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    while (selectingTimeZone) {
        currentTime = millis();

        // Update display periodically to show current selection
        if (currentTime - lastUpdateTime >= 200) {  // Update every 200ms
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("Select Time Zone:");
            
            // Show current time zone with offset
            display.print("UTC");
            if (timeZones[currentTimeZoneIndex].offset >= 0) {
                display.print("+");
            }
            
            // Handle float offset display
            int intPart = abs(static_cast<int>(timeZones[currentTimeZoneIndex].offset));
            int fracPart = abs(static_cast<int>((abs(timeZones[currentTimeZoneIndex].offset) - intPart) * 60));
            
            display.print(intPart);
            if (fracPart > 0) {
                display.print(":");
                if (fracPart < 10) display.print("0");
                display.print(fracPart);
            }
            display.print(" ");
            display.println(timeZones[currentTimeZoneIndex].name);

            display.println("\nUP/DOWN: Change");
            display.println("OK: Confirm");
            display.println("BACK: Cancel");

            display.display();
            lastUpdateTime = currentTime;
        }

        // Check button inputs
        if (digitalRead(BTN_UP_PIN) == LOW) {
            // Move to previous time zone with debounce
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                if (currentTimeZoneIndex > 0) {
                    currentTimeZoneIndex--;
                } else {
                    // Wrap around to the last time zone
                    currentTimeZoneIndex = NUM_TIME_ZONES - 1;
                }
                lastButtonPressTime = millis();
            }
        }
        else if (digitalRead(BTN_DOWN_PIN) == LOW) {
            // Move to next time zone with debounce
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                if (currentTimeZoneIndex < NUM_TIME_ZONES - 1) {
                    currentTimeZoneIndex++;
                } else {
                    // Wrap around to the first time zone
                    currentTimeZoneIndex = 0;
                }
                lastButtonPressTime = millis();
            }
        }
        else if (digitalRead(BTN_OK_PIN) == LOW) {
            // Confirm time zone selection
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                // Set the time zone using configTime
                long utcOffset = timeZones[currentTimeZoneIndex].utcOffsetSeconds;
                configTime(utcOffset, 0, NTP_SERVER);

                // Clear display and show confirmation
                display.clearDisplay();
                display.setCursor(0, 0);
                display.println("Time Zone Set:");
                display.print("UTC");
                if (timeZones[currentTimeZoneIndex].offset >= 0) {
                    display.print("+");
                }
                
                // Display offset with proper formatting
                int intPart = abs(static_cast<int>(timeZones[currentTimeZoneIndex].offset));
                int fracPart = abs(static_cast<int>((abs(timeZones[currentTimeZoneIndex].offset) - intPart) * 60));
                
                display.print(intPart);
                if (fracPart > 0) {
                    display.print(":");
                    if (fracPart < 10) display.print("0");
                    display.print(fracPart);
                }
                display.print(" ");
                display.println(timeZones[currentTimeZoneIndex].name);
                
                display.println("\nSynchronizing...");
                display.display();

                // Wait for time synchronization
                delay(2000);  // Give some time for NTP sync
                
                // Optional: Add error checking for time sync
                time_t now = time(nullptr);
                if (now > 1000) {
                    display.println("Time Sync Success!");
                } else {
                    display.println("Time Sync Failed!");
                }
                display.display();
                
                delay(1500);  // Show confirmation for 1.5 seconds
                selectingTimeZone = false;
                uiMode = MODE_MAIN_MENU;
                updateMainMenuUI();
                
                lastButtonPressTime = millis();
            }
        }
        else if (digitalRead(BTN_BACK_PIN) == LOW) {
            // Cancel time zone selection
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                uiMode = MODE_MAIN_MENU;
                updateMainMenuUI();
                selectingTimeZone = false;
                lastButtonPressTime = millis();
            }
        }
    }
}

void addAlarm() {
    int selectedHour = 0;
    int selectedMinute = 0;
    bool selectingHour = true;
    bool selectingMinute = false;
    int availableAlarmIndex = -1;

    // Find first available alarm slot
    for (int i = 0; i < n_alarms; i++) {
        if (alarm_hours[i] == 0 && alarm_minutes[i] == 0) {
            availableAlarmIndex = i;
            break;
        }
    }

    if (availableAlarmIndex == -1) {
        display.clearDisplay();
        display.setTextSize(1);
        display.setCursor(0, 0);
        display.println("No more alarms");
        display.println("can be added.");
        display.display();
        delay(2000);
        uiMode = MODE_MAIN_MENU;
        updateMainMenuUI();
        return;
    }

    unsigned long lastUpdateTime = 0;
    unsigned long currentTime;

    while (selectingHour || selectingMinute) {
        currentTime = millis();

        // Update display periodically
        if (currentTime - lastUpdateTime >= 200) {
            display.clearDisplay();
            display.setTextSize(1);
            display.setCursor(0, 0);
            display.println("Add New Alarm");
            
            if (selectingHour) {
                display.setTextSize(1);
                display.print("Hour: ");
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
                display.print(selectedHour);
                display.setTextColor(SSD1306_WHITE);
                display.print("  Minute: ");
                display.print(selectedMinute);
            } else {
                display.setTextSize(1);
                display.print("Hour: ");
                display.print(selectedHour);
                display.print("  Minute: ");
                display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
                display.print(selectedMinute);
                display.setTextColor(SSD1306_WHITE);
            }

            display.println("\n\nUP/DOWN: Change");
            display.println("OK: Select");
            display.println("BACK: Cancel");

            display.display();
            lastUpdateTime = currentTime;
        }

        // Button handling
        if (digitalRead(BTN_UP_PIN) == LOW) {
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                if (selectingHour) {
                    selectedHour = (selectedHour + 1) % 24;
                } else {
                    selectedMinute = (selectedMinute + 1) % 60;
                }
                lastButtonPressTime = millis();
            }
        }
        else if (digitalRead(BTN_DOWN_PIN) == LOW) {
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                if (selectingHour) {
                    selectedHour = (selectedHour - 1 + 24) % 24;
                } else {
                    selectedMinute = (selectedMinute - 1 + 60) % 60;
                }
                lastButtonPressTime = millis();
            }
        }
        else if (digitalRead(BTN_OK_PIN) == LOW) {
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                if (selectingHour) {
                    selectingHour = false;
                    selectingMinute = true;
                } else {
                    // Set the alarm
                    if (set_alarm(availableAlarmIndex, selectedHour, selectedMinute)) {
                        display.clearDisplay();
                        display.setCursor(0, 0);
                        display.println("Alarm Set:");
                        display.print(selectedHour);
                        display.print(":");
                        if (selectedMinute < 10) display.print("0");
                        display.println(selectedMinute);
                        display.display();
                        delay(1500);
                    }
                    uiMode = MODE_MAIN_MENU;
                    updateMainMenuUI();
                    return;
                }
                lastButtonPressTime = millis();
            }
        }
        else if (digitalRead(BTN_BACK_PIN) == LOW) {
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                uiMode = MODE_MAIN_MENU;
                updateMainMenuUI();
                return;
            }
        }
    }
}

void viewAlarms() {
    int displayPage = 0;
    unsigned long lastUpdateTime = 0;
    unsigned long currentTime;

    while (uiMode == MODE_EXECUTE) {
        currentTime = millis();

        // Update display periodically
        if (currentTime - lastUpdateTime >= 200) {
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("Existing Alarms:");

            int alarmsFound = 0;
            for (int i = 0; i < n_alarms; i++) {
                if (alarm_hours[i] != 0 || alarm_minutes[i] != 0) {
                    display.print(i + 1);
                    display.print(": ");
                    if (alarm_hours[i] < 10) display.print("0");
                    display.print(alarm_hours[i]);
                    display.print(":");
                    if (alarm_minutes[i] < 10) display.print("0");
                    display.println(alarm_minutes[i]);
                    alarmsFound++;
                }
            }

            if (alarmsFound == 0) {
                display.println("No alarms set.");
            }

            display.println("\nBACK: Return");
            display.display();
            lastUpdateTime = currentTime;
        }

        // Button handling
        if (digitalRead(BTN_BACK_PIN) == LOW) {
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                uiMode = MODE_MAIN_MENU;
                updateMainMenuUI();
                return;
            }
        }
    }
}

void deleteAlarm() {
    int selectedAlarmIndex = 0;
    unsigned long lastUpdateTime = 0;
    unsigned long currentTime;

    while (uiMode == MODE_EXECUTE) {
        currentTime = millis();

        // Update display periodically
        if (currentTime - lastUpdateTime >= 200) {
            display.clearDisplay();
            display.setCursor(0, 0);
            display.println("Delete Alarm");

            int validAlarms[n_alarms];
            int validAlarmsCount = 0;

            // Find valid alarms
            for (int i = 0; i < n_alarms; i++) {
                if (alarm_hours[i] != 0 || alarm_minutes[i] != 0) {
                    validAlarms[validAlarmsCount] = i;
                    validAlarmsCount++;
                }
            }

            if (validAlarmsCount == 0) {
                display.println("No alarms to delete.");
                display.println("\nBACK: Return");
                display.display();
                
                // Wait for back button
                if (digitalRead(BTN_BACK_PIN) == LOW) {
                    if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                        uiMode = MODE_MAIN_MENU;
                        updateMainMenuUI();
                        return;
                    }
                }
                continue;
            }

            // Ensure selected index is within valid range
            if (selectedAlarmIndex >= validAlarmsCount) {
                selectedAlarmIndex = 0;
            }

            // Highlight selected alarm
            for (int i = 0; i < validAlarmsCount; i++) {
                if (i == selectedAlarmIndex) {
                    display.setTextColor(SSD1306_BLACK, SSD1306_WHITE);
                } else {
                    display.setTextColor(SSD1306_WHITE);
                }

                display.print(validAlarms[i] + 1);
                display.print(": ");
                if (alarm_hours[validAlarms[i]] < 10) display.print("0");
                display.print(alarm_hours[validAlarms[i]]);
                display.print(":");
                if (alarm_minutes[validAlarms[i]] < 10) display.print("0");
                display.println(alarm_minutes[validAlarms[i]]);
            }

            display.setTextColor(SSD1306_WHITE);
            display.println("\nUP/DOWN: Select");
            display.println("OK: Delete");
            display.println("BACK: Cancel");
            display.display();
            lastUpdateTime = currentTime;
        }

        // Button handling
        if (digitalRead(BTN_UP_PIN) == LOW) {
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                selectedAlarmIndex = (selectedAlarmIndex - 1 + n_alarms) % n_alarms;
                lastButtonPressTime = millis();
            }
        }
        else if (digitalRead(BTN_DOWN_PIN) == LOW) {
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                selectedAlarmIndex = (selectedAlarmIndex + 1) % n_alarms;
                lastButtonPressTime = millis();
            }
        }
        else if (digitalRead(BTN_OK_PIN) == LOW) {
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                // Find the actual alarm index to delete
                int validAlarms[n_alarms];
                int validAlarmsCount = 0;

                for (int i = 0; i < n_alarms; i++) {
                    if (alarm_hours[i] != 0 || alarm_minutes[i] != 0) {
                        validAlarms[validAlarmsCount] = i;
                        validAlarmsCount++;
                    }
                }

                if (selectedAlarmIndex < validAlarmsCount) {
                    int alarmToDelete = validAlarms[selectedAlarmIndex];
                    delete_alarm(alarmToDelete);

                    display.clearDisplay();
                    display.setCursor(0, 0);
                    display.println("Alarm Deleted");
                    display.display();
                    delay(1500);
                }

                uiMode = MODE_MAIN_MENU;
                updateMainMenuUI();
                return;
            }
        }
        else if (digitalRead(BTN_BACK_PIN) == LOW) {
            if (millis() - lastButtonPressTime > DEBOUNCE_DELAY) {
                uiMode = MODE_MAIN_MENU;
                updateMainMenuUI();
                return;
            }
        }
    }
}