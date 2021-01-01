#include "TimeLib.h"
//#include <avr/io.h>
//#include <avr/interrupt.h>
//#include <avr/wdt.h>
//#include <avr/iom328p.h>

//ISR(BADISR_vect)
//{
//  while(true);
//}

void setup() {
//  sei();
//if(MCUCSR & (1<<PORF ));// myprintf0P(PSTR("Power-on reset.\n"));
//if(MCUCSR & (1<<EXTRF));// myprintf0P(PSTR("External reset!\n"));
//if(MCUCSR & (1<<BORF ));// myprintf0P(PSTR("Brownout reset!\n"));
//if(MCUCSR & (1<<WDRF ));// myprintf0P(PSTR("Watchdog reset!\n"));
//if(MCUCSR & (1<<JTRF ));// myprintf0P(PSTR("JTAG reset!\n"));
//MCUCSR = 0;
  pinMode(2,OUTPUT);  // pompa 1
  pinMode(3,OUTPUT);  // pompa 2
  pinMode(4,OUTPUT);  // luce piccola
  pinMode(5,OUTPUT);  // luce grnde
  pinMode(7,INPUT_PULLUP);  // freccia in su (MODE)
  pinMode(8,INPUT_PULLUP);  // freccia in giu (COUNT)
  pinMode(13,OUTPUT);   // led verde
  pinMode(12,OUTPUT);   // led rosso
//  Serial.begin(9600);
  digitalWrite(12, HIGH); // segnalazione di mancanza rete
  digitalWrite(13, LOW);
  digitalWrite(2, HIGH);
  digitalWrite(3, HIGH);
  digitalWrite(4, HIGH);
  digitalWrite(5, HIGH);
}

byte program=0;
byte toogle=0;
int roundRobin=0;
byte cntOra=0;
byte cnt5Min=0;

void loop() {
  if(!(roundRobin%600))
    acquarioRun();
  // gestione riprogrammazione tempo
  if(program!=7) {
    if(program==0) {
      if(digitalRead(7)==LOW) {
        program=1;
        delay(200);
      }
    }
    else if(program==1) {
      if(digitalRead(7)==HIGH) {
        program=2;
        delay(200);
      }
    }
    else if(program==2) {
      if((roundRobin/10)%2==0)
        digitalWrite(12, HIGH);
      else if((roundRobin/10)%2==1)
        digitalWrite(12, LOW);
      if(digitalRead(8)==LOW) {
        if(toogle==0) {
          digitalWrite(13, HIGH);
          cntOra++;
          toogle=1;
          if(cntOra>23)
            cntOra=0;
        }
      }
      else {
        digitalWrite(13, LOW);
        toogle=0;
      }
      if(digitalRead(7)==LOW) {
        program=3;
        delay(400);
      }
    }
    else if(program==3) {
      digitalWrite(12, LOW);
      if(digitalRead(7)==HIGH) {
        for(int i=0;i<cntOra;i++) {
          digitalWrite(13, HIGH);
          delay(400);
          digitalWrite(13, LOW);
          delay(400);
        }
        program=4;
        delay(200);
      }
    }
    else if(program==4) {
      if((roundRobin/2)%2==0)
        digitalWrite(12, HIGH);
      else if((roundRobin/2)%2==1)
        digitalWrite(12, LOW);
      if(digitalRead(8)==LOW) {
        if(toogle==0) {
          digitalWrite(13, HIGH);
          cnt5Min++;
          toogle=1;
          if(cnt5Min>11)
            cnt5Min=0;
        }
      }
      else {
        digitalWrite(13, LOW);
        toogle=0;
      }
      if(digitalRead(7)==LOW) {
        program=5;
        delay(400);
      }
    }
    else if(program==5) {
      digitalWrite(12, LOW);
      if(digitalRead(7)==HIGH) {
        for(int i=0;i<cnt5Min;i++) {
          digitalWrite(13, HIGH);
          delay(400);
          digitalWrite(13, LOW);
          delay(400);
        }
        program=6;
        delay(200);
      }
    }
    else if(program==6) {
      digitalWrite(12, HIGH);
      delay(500);
      digitalWrite(13, HIGH);
      delay(1000);
      digitalWrite(12, LOW);
      delay(500);
      digitalWrite(13, LOW);
      delay(500);
      setTime(cntOra,cnt5Min*5,0,0,0,0);
      program=7;
//      int ora=hour();
//      int minuti=minute();
//      Serial.print("ora:");
//      Serial.println(ora);
//      Serial.print("min:");
//      Serial.println(minuti);
//      Serial.println("END program!");
      acquarioRun();
    }
  }
  else {
    if(digitalRead(8)==LOW) {
      digitalWrite(12, LOW);
      int ore=hour();
      for(int i=0;i<ore;i++) {
        digitalWrite(13, HIGH);
        delay(400);
        digitalWrite(13, LOW);
        delay(400);
      }
      delay(200);
    }
    if(digitalRead(7)==LOW) {
      digitalWrite(13, LOW);
      int minu=minute();
      for(int i=0;i<minu/5;i++) {
        digitalWrite(12, HIGH);
        delay(400);
        digitalWrite(12, LOW);
        delay(400);
      }
      delay(200);
    }
  }
//  if(!(roundRobin%10)) {
//    Serial.print("program=");
//    Serial.println(program);
//  }
  roundRobin++;
  // now sleep
  delay(100); // round-robin a 100ms
}


void acquarioRun() {
  int ora=hour()%24;
  int minuti=minute()%60;
  int minutiTotali=ora*60+minuti;
//  Serial.print("ora:");
//  Serial.println(ora);
//  Serial.print("min:");
//  Serial.println(minuti);
  // Pompa 1-2
  if((minutiTotali>=0 && minutiTotali<370) || (minutiTotali>=720 && minutiTotali<1090))
    digitalWrite(2, LOW);
  else
    digitalWrite(2, HIGH);
  // Pompa 2
  if((minutiTotali>=0 && minutiTotali<10) || (minutiTotali>=360 && minutiTotali<730) || (minutiTotali>=1080))
    digitalWrite(3, LOW);
  else
    digitalWrite(3, HIGH);
  // Luce piccola
  if(ora>=13 && ora<22)
    digitalWrite(4, LOW);
  else
    digitalWrite(4, HIGH);
  // Luce grossa
  if(ora>=14 && ora<21)
    digitalWrite(5, LOW);
  else
    digitalWrite(5, HIGH);
}

