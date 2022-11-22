#include <Servo.h>  //Import the library files of steering gear
Servo head_servo;  //Create a steering gear instance to control a steering gear
#include <Wire.h> 
float distance = 0;  //Decimal distance value
float ds[3];  //Create an array of float numbers to store the values read
int distance1; //Variable 1 for storing ranging values
int distance2; //Variable 2 for storing ranging values
int distance3; //Variable 3 for storing ranging values

#define INA 7  //Define pin 7 to control the direction of left motor
#define ENA 6  //Define pin 6 to control the speed of left motor
#define INB 8  //Define pin 8 to control the direction of right motor
#define ENB 5  //Define pin 5 to control the speed of right motor

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(INA, OUTPUT);  //Set the pins controlling the motors to output
  pinMode(ENA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(ENB, OUTPUT);
  head_servo.attach(12,500,2500);  //Set the steering gear pin as A2, and the range is 500-2500ms
  head_servo.write(90);  //90 degrees
  delay(200);
}

void loop() {
  func_avoid(); // The sub function of obstacle avoidance
}

void func_avoid()
{
  distance1 = checkdistance(); //Get the value returned by ultrasonic function
  Serial.print("distance1 = ");
  Serial.println(distance1);  //Print the values detected by ultrasonic sensor with new line
  if(distance1 > 15)
  {
    car_forward();
  }
  else
  {
    car_stop();
    head_servo.write(180);  //The head of the snail turns left
    delay(300);
    distance2 = checkdistance(); //Get the left distance value
    delay(100);
    head_servo.write(10);   //The head of the snail turns right
    delay(500);
    distance3 = checkdistance(); //Get the left distance value
    delay(100);
    if(distance2 > distance3)
    {
      car_left();  //Turn left
      head_servo.write(90);
      delay(300);  //The delay time determines the rotation angle
    }
    else
    {
      car_right();
      head_servo.write(90);
      delay(300);
    }
  }
}

//Ultrasonic ranging function
float checkdistance() {
  char i = 0;
  ds[0]=0;
  ds[1]=0;
  ds[2]=0;
  Wire.beginTransmission(0x57);   //Initial Address is 0X57
  Wire.write(1);                  //Writing command 0X01: 0X01 is the command to start measuring 
  Wire.endTransmission();            
  delay(100);                     //Measure cycle delay: One cycle is 100ms, set 120ms to keep some allowance    
  Wire.requestFrom(0x57,3);       //The address is 0X57, and read 3 8-bit distance values      
  while (Wire.available())
  {
   ds[i++] = Wire.read();
  }          
  distance=(ds[0]*65536+ds[1]*256+ds[2])/10000; //Calculated as cm value     
  Serial.print("distance : "); 
  if ((1<=distance)&&(600>=distance))           //Value between 1cm-6m
  {
  Serial.print(distance);
   Serial.print(" CM ");  
  }
  else 
  {
  Serial.print(" - - - - ");                   //Invalid value is displayed as- - - -
  }
  Serial.println();    
  delay(10);                                  //Adjust the measure cycle  
  return distance;
}

//The little snail moves forward
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
