#include "buzzer.h"



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

void ring_alarm(){
  bool break_happened = false;

  //Ring the Buzzer
  while (break_happened==false && digitalRead(BTN_CANCEL)==HIGH){
    for (int i=0; i<n_notes; i++ ){
    if (digitalRead(BTN_CANCEL)==LOW){
      delay(200);
      break_happened= true;
      break;
    }
    tone(BUZZER , notes[i]);
    delay(500);
    noTone(BUZZER);
    delay(2);
    }
  } 
}