/*
 * create by straysnail
 * 2022/2/26
 */

#define buzPin  A3  //Define the pin of the buzzer as A3

void setup() {
  // put your setup code here, to run once:
  pinMode(buzPin, OUTPUT); //Set to output state
}

void loop() {
  //Set the frequency of the buzzer by delay
  digitalWrite(buzPin, HIGH);
  delayMicroseconds(500);  //Microsecond delay. Within a certain range, adjust the delay, and the buzzer will make different sounds
  digitalWrite(buzPin, LOW);
  delayMicroseconds(500);
}
