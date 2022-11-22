#include <Servo.h>  //Import the library files of steering gear
Servo head_servo;  //Create a steering gear instance to control a steering gear

void setup() {
  head_servo.attach(12,500,2500);  //Set the pin of the steering gear as A2, and the pulse range is 500-2500ms
}

void loop() {
  head_servo.write(10);   //The steering gear turns to 10 degree
  delay(500);   //Delay for the steering gear to turn
  head_servo.write(90);  //90 degree
  delay(500);
  head_servo.write(180);
  delay(500);
  head_servo.write(90);
  delay(500);
}
