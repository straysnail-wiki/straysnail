/*
create by straysnail
2022/2/26
*/
void setup() {
  pinMode(A3,OUTPUT); //Set A3 pin to output state
}

void loop() {
  digitalWrite(A3,HIGH); //Digital output function, controlling A3 pin output high level
  delay(300); //Delay 300ms
  digitalWrite(A3,LOW);  //Control A3 pin output low level
  delay(300);
}
