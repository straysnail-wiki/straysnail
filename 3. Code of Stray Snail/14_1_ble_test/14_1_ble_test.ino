char bleStr = "";

void setup() {
  Serial.begin(9600);

}

void loop() {
  while(Serial.available() > 0)  //Judge whether the serial port has received the value
  {
    bleStr = Serial.read();  //Read the value received by serial port
    Serial.println(bleStr);
    delay(10);
  }
}
