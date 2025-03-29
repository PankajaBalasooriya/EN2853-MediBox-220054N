#include "buzzer.h"
#include "clock.h"



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

void ring_alarm() {
  bool break_happened = false;

  //Ring the Buzzer
  while (break_happened == false && digitalRead(BTN_OK_PIN) == HIGH) {
    for (int i = 0; i < n_notes; i++) {
      if (digitalRead(BTN_OK_PIN) == LOW) {
        delay(200);
        display.clearDisplay();
        print_line("Alarm", 2, 5, 25);
        print_line("Dismissed", 2, 25, 15);
        display.display();
        delay(1000);
        break_happened = true;
        break;
      }

      if (digitalRead(BTN_CANCEL) == LOW) {
        delay(200);
        // Show snooze confirmation message
        display.clearDisplay();
        print_line("SNOOZING", 2, 5, 25);
        print_line("for 5 minutes", 1, 25, 20);
        display.display();
        delay(1500);
        
        // Call snooze function
        snooze_alarm(5); // Snooze for 5 minutes
        break_happened = true;
        break;
      }

      tone(BUZZER, notes[i]);
      delay(300);
      noTone(BUZZER);
      delay(2);
    }
  }
}