#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

#define LED_1 20
#define BUZZER 19

// Global Varaibles
int days = 0;
int hours = 0;
int minutes = 0;
int seconds = 0;

unsigned long timeNow = 0;
unsigned long timeLast = 0;

bool alarm_enabled = true;
int n_alarms = 2;
int alarm_hours[] = {0, 1};
int alarm_minutes[] = {1, 10};
bool alarm_triggered[] = {false, false};


int n_notes = 8;
int C = 262;
int D = 294;
int E = 330;
int F = 349;
int G = 392;
int A = 440;
int B = 494;
int C_H = 523;
int notes[] = {C, D, E, F, G, A, B, C_H};



Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void print_line(String text, int text_size, int row, int column);
void print_time_now(void);
void update_time();
void update_time_with_check_alarm();
void ring_alarm();



void setup() {
  // put your setup code here, to run once:
  pinMode(LED_1, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)){
    Serial.println(F("SSD1306 allocation failed!"));
    for(;;);
  }

  display.display();
  delay(2000);


  print_line("Welcome to MediBox!", 2, 10, 20);
  delay(2000);
  display.clearDisplay();
  
}

void loop() {
  // put your main code here, to run repeatedly:
  update_time_with_check_alarm();
  
  
}


void print_line(String text, int text_size, int row, int column){
  display.setTextSize(text_size);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(column, row);
  display.println(text);
  display.display();
}

void print_time_now(void){
  display.clearDisplay();
  print_line(String(days), 2, 0, 0);
  print_line(":", 2, 0, 30);
  print_line(String(hours), 2, 0, 30);
  print_line(":", 2, 0, 50);
  print_line(String(minutes), 2, 0, 60);
  print_line(":", 2, 0, 80);
  print_line(String(seconds), 2, 0, 90);
}

void update_time(){
  timeNow = millis() / 1000;

  seconds = timeNow - timeLast;

  if(seconds >= 60){
    minutes++;
    timeLast += 60;
  }

  if(minutes == 60){
    hours++;
    minutes = 0;
  }

  if(hours == 24){
    days++;
    hours = 0;
  }

}


void update_time_with_check_alarm(){
  update_time();
  print_time_now();

  if(alarm_enabled){
    for(int i =0; i < n_alarms;i++){
      if(!alarm_triggered[i] && alarm_hours[i] == hours && alarm_minutes[i] == minutes){
        ring_alarm();
      }
    }
  }
}

void ring_alarm(){
  display.clearDisplay();
  print_line("Medicine Time!", 2, 0, 2);

  digitalWrite(LED_1, HIGH);
  
  for(int i = 0; i < n_notes; i++){
    tone(BUZZER, notes[i]);
    delay(500);
    noTone(BUZZER);
    delay(2);
  }
  digitalWrite(LED_1, LOW);
  display.clearDisplay();
}