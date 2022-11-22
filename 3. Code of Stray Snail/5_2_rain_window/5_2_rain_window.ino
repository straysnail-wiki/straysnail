#include <Servo.h>  //Import the library files of steering gear
Servo window_servo;  //Create a steering gear instance to control the window steering gear
#define waterPin A0  //Define the IO port of the raindrop sensor as A0
int waterVal = 0;

void setup() {
  Serial.begin(9600);  //Set the baud rate of serial communication to 9600
  pinMode(4, OUTPUT);
  pinMode(waterPin,INPUT);  //Set as input
  window_servo.attach(4,500,2500);  //Set the pin of the steering gear as A0, and the pulse range is 500-2500ms
  window_servo.write(116);
  delay(300);
}

void loop() {
  waterVal = analogRead(waterPin);  //Read the analog value detected by the raindrop sensor
  Serial.print("waterVal = ");
  Serial.println(waterVal);  //Print the value detected by the raindrop sensor with new line
  delay(200); //Delay to adjust the printing speed of serial port
  if(waterVal > 100)
  {
    window_servo.write(116); //Close the window
    delay(300);
  }
  else
  {
    window_servo.write(50); //Open the window
    delay(300);
  }
}
