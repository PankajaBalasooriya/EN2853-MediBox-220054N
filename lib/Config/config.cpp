#include "config.h"


void initPins(){
    pinMode(LED_1, OUTPUT);
    pinMode(BUZZER, OUTPUT);
    pinMode(BTN_CANCEL, INPUT);
    pinMode(BTN_OK, INPUT);
    pinMode(BTN_UP, INPUT);
    pinMode(BTN_DOWN, INPUT);
    pinMode(DHT11_DATA_PIN, INPUT);
}