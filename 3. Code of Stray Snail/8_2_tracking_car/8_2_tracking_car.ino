#define INA 7  //Define pin 7 to control the direction of left motor
#define ENA 6  //Define pin 6 to Control the speed of left motor
#define INB 8  //Define pin 8 to control the direction of right motor
#define ENB 5  //Define pin 5 to Control the speed of right motor

#define wayPin1 9
#define wayPin2 10
boolean L_tval = 0;
boolean R_tval = 0;

void setup() {
  Serial.begin(9600);   //Set the baud rate of serial port to 9600
  pinMode(INA, OUTPUT);  //Set the pin of controlling the motor to output
  pinMode(ENA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(wayPin1, INPUT);
  pinMode(wayPin2, INPUT);
}

void loop() {
  car_tracking();
}

void car_tracking()
{
  L_tval = digitalRead(wayPin1);
  R_tval = digitalRead(wayPin2);
  Serial.print(L_tval);
  Serial.print("  ");
  Serial.println(R_tval);
  delay(100);
  if((L_tval == 1) && (R_tval == 1))
  {
    car_forward();
  }
  else if((L_tval == 1) && (R_tval == 0))
  {
    car_left();
    
  }
  else if((L_tval == 0) && (R_tval == 1))
  {
    car_right();
  }
  else if((L_tval == 0) && (R_tval == 0))
  {
    car_stop();
  }
}

//The little snail moves forward
void car_forward()
{
  digitalWrite(INA, HIGH);  //Output high level and control the left motor forward
  analogWrite(ENA, 50);    //PWM output to control the speed of left motor
  digitalWrite(INB, HIGH);  //Output high level and control the right motor forward
  analogWrite(ENB, 50);    //PWM output to control the speed of right motor
}

//The little snail turns left
void car_left()
{
  digitalWrite(INA, LOW);
  analogWrite(ENA, 70);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 80);
}
//The little snail turns right
void car_right()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 80);
  digitalWrite(INB, LOW);
  analogWrite(ENB, 70);
}
//Stop
void car_stop()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 0);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 0);
}
