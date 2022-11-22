/*
 * create by straysnail
 * 2022/2/28
 */
#define L_light A1
#define R_light A2
int L_lightVal;
int R_lightVal;

//Define the pin of motors
#define INA 7
#define ENA 6
#define INB 8
#define ENB 5

void setup() {
  Serial.begin(9600);
  //Set the pin of the photosensitive sensors to input
  pinMode(L_light, INPUT);
  pinMode(R_light, INPUT);
  
  //Set the pin of the motors to output
  pinMode(INA, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  L_lightVal = analogRead(L_light);
  R_lightVal = analogRead(R_light);
  Serial.print(L_lightVal);
  Serial.print("  ");
  Serial.println(R_lightVal);
  delay(100);
  if((L_lightVal >= 800) && (R_lightVal >= 800))
  {
    car_forward();
  }
  else if((L_lightVal >= 800) && (R_lightVal < 800))
  {
    car_left();
  }
  else if((L_lightVal < 800) && (R_lightVal >= 800))
  {
    
    car_right();
  }
  else if((L_lightVal < 800) && (R_lightVal < 800))
  {
    car_stop();
  }
}

//The little snail moves forward
void car_forward()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 155);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 155);
}

//The little snail turns left
void car_left()
{
  digitalWrite(INA, LOW);
  analogWrite(ENA, 150);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 150);
}

//The little snail turns right
void car_right()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 150);
  digitalWrite(INB, LOW);
  analogWrite(ENB, 150);
}

//Stop
void car_stop()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 0);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 0);
}
