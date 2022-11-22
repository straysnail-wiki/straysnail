/*
 * create by straysnail
 * 2022/2/28
 */
#define lightPin_L A1  //Define the pin of photosensitive sensor as A1
#define lightPin_R A2

int light_L_val;
int light_R_val;

void setup() {
  Serial.begin(9600);
  pinMode(lightPin_L, INPUT);  //Set to input
  pinMode(lightPin_R, INPUT);
}

void loop() {
  light_L_val = analogRead(lightPin_L);  //Read the analog value
  light_R_val = analogRead(lightPin_R);
  Serial.print("light_L_val = ");
  Serial.print(light_L_val);
  Serial.print("  ");
  Serial.print("light_R_val = ");
  Serial.println(light_R_val);
  delay(100);
}
