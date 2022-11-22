/*
 * create by straysnail
 * 2022/2/26
 */
#define waterPin A0  //Define the IO port of the raindrop sensor as A0
int waterVal = 0;

void setup() {
  Serial.begin(9600);  //Set the baud rate of serial communication to 9600
  pinMode(waterPin,INPUT);  //Set as input
}

void loop() {
  waterVal = analogRead(waterPin);  //Read the analog value detected by the raindrop sensor
  Serial.print("waterVal = ");
  Serial.println(waterVal);  //Print the value detected by the raindrop sensor with new line
  //delay(200); //Delay to adjust the printing speed of serial port
}
