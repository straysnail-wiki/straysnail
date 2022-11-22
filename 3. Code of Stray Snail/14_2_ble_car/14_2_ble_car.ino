/*
 * create by straysnail
 * 2022/3/6
 */
char bleVal;

#define INA 7  //Define pin 7 to control the direction of left motor
#define ENA 6  //Define pin 6 to control the speed of left motor
#define INB 8  //Define pin 8 to control the direction of right motor
#define ENB 5  //Define pin 5 to control the speed of right motor

void setup() {
  Serial.begin(9600);
  pinMode(INA, OUTPUT);  //Set the pins controlling the motors to output
  pinMode(ENA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  if(Serial.available() > 0)
  {
    String bleVal1 = Serial.readStringUntil('*');
    if(bleVal1[0] == '$')
    {
      bleVal = bleVal1[1];
      Serial.println(bleVal);
    }
    switch(bleVal)
    {
      case 'F': car_forward(); break;
      case 'B': car_back(); break;
      case 'L': car_left(); break;
      case 'R': car_right(); break;
      case 'S': car_stop(); break;
    }
  }
}

//The little snail moves forward
void car_forward()
{
  digitalWrite(INA, HIGH);  //Output high level and control the left motor forward
  analogWrite(ENA, 200);    //PWM output to control the speed of left motor
  digitalWrite(INB, HIGH);  //Output high level and control the right motor forward
  analogWrite(ENB, 200);    //PWM output to control the speed of right motor
}
//The little snail moves backward
void car_back()
{
  digitalWrite(INA, LOW);  //Output low level and control the left motor backward
  analogWrite(ENA, 200);   //PWM output to control the speed of left motor
  digitalWrite(INB, LOW);  //Output low level and control the right motor backward
  analogWrite(ENB, 200);   //PWM output to control the speed of right motor
}
//The little snail turns left
void car_left()
{
  digitalWrite(INA, LOW);
  analogWrite(ENA, 200);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 200);
}
//The little snail turns right
void car_right()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 200);
  digitalWrite(INB, LOW);
  analogWrite(ENB, 200);
}
//Stop
void car_stop()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 0);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 0);
}
