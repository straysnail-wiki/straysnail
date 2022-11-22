/*
 * create by straysnail
 * 2022/2/26
 */
 
#include <NewTone.h> //Import library files for sound frequencies(no timer required)
#define buzzerPin A3   //Set the pin of the buzzer as A3
#define btnPin 2

void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  pinMode(btnPin, INPUT);
}

void loop() {
  boolean val = digitalRead(btnPin);
  if(val == 0)
  {
    play_music(buzzerPin);
  }
  else
  {
    NewNoTone(buzzerPin);  //Stop making sound
  }
}

void play_music(int buzzer_pin)
{
  NewTone(buzzer_pin,294,250);   //3 parameters of NewTone: IO port, frequency, duration
  NewTone(buzzer_pin,440,250);
  NewTone(buzzer_pin,392,250);
  NewTone(buzzer_pin,532,250);
  NewTone(buzzer_pin,494,500);
  NewTone(buzzer_pin,392,250);
  NewTone(buzzer_pin,440,250);
  NewTone(buzzer_pin,392,250);
  NewTone(buzzer_pin,587,250);
  NewTone(buzzer_pin,532,500);
  NewTone(buzzer_pin,392,250);
  NewTone(buzzer_pin,784,250);
  NewTone(buzzer_pin,659,250);
  NewTone(buzzer_pin,532,250);
  NewTone(buzzer_pin,494,250);
  NewTone(buzzer_pin,440,250);
  NewTone(buzzer_pin,698,375);
  NewTone(buzzer_pin,659,250);
  NewTone(buzzer_pin,532,250);
  NewTone(buzzer_pin,587,250);
  NewTone(buzzer_pin,532,500);
  NewNoTone(buzzer_pin);
}
