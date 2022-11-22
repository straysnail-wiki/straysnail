#define lightPin_L A1
#define LEDPin A3

int light_L_val;

void setup() {
  Serial.begin(9600);
  pinMode(lightPin_L, INPUT);
  pinMode(LEDPin, OUTPUT);
}

void loop() {
  light_L_val = analogRead(lightPin_L);
  Serial.print("light_L_val = ");
  Serial.println(light_L_val);
  delay(100);
  if(light_L_val < 500)  //When it is lower than the set value, the LED will be on
  {
    digitalWrite(LEDPin, HIGH);
  }
  else
  {
    digitalWrite(LEDPin, LOW);
  }
}
