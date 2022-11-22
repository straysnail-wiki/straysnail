/*
 * create by straysnail
 * 2022/2/26
 */
#define INA 7  //Define pin 7 to control the direction of left motor
#define ENA 6  //Define pin 6 to Control the speed of left motor
#define INB 8  //Define pin 8 to control the direction of right motor
#define ENB 5  //Define pin 5 to Control the Define pin 6 to Control the speed of left motorspeed of right motor

void setup() {
  pinMode(INA, OUTPUT);  //Set the pins controlling the motor to output
  pinMode(ENA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  car_forward();  //Forward
  delay(2000);    //2s
  car_back();     //Backward
  delay(2000);    //2s
  car_left();     //Turn left
  delay(2000);    //2s
  car_right();    //Turn right
  delay(2000);    //2s
  car_stop();     //Stop
  delay(1000);    //1s
}

//The little snail goes forward
void car_forward()
{
  digitalWrite(INA, HIGH);  //Output high level and control the left motor forward
  analogWrite(ENA, 100);    //PWM output to control the speed of left motor
  digitalWrite(INB, HIGH);  //Output high level and control the right motor forward
  analogWrite(ENB, 100);    //PWM output to control the speed of right motor
}
//The little snail moves backward
void car_back()
{
  digitalWrite(INA, LOW);  //Output low level and control the left motor backward
  analogWrite(ENA, 100);   //PWM output to control the speed of left motor
  digitalWrite(INB, LOW);  //Output low level and control the right motor backward
  analogWrite(ENB, 100);   //PWM output to control the speed of right motor
}
//The little snail turns left
void car_left()
{
  digitalWrite(INA, LOW);
  analogWrite(ENA, 100);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 100);
}
//The little snail turns right
void car_right()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 100);
  digitalWrite(INB, LOW);
  analogWrite(ENB, 100);
}
//Stop
void car_stop()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 0);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 0);
}
