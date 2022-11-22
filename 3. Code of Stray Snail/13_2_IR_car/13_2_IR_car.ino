#include <IRremote.h>
#define IR_RECEIVE_PIN  3  //Define the pin of infrared receiving module 
IRrecv IrReceiver(IR_RECEIVE_PIN);
decode_results results;
int ir_val;
int ir_FFFF;
int ir_val_data;
#define INA 7  //Define pin 7 to control the direction of left motor
#define ENA 6  //Define pin 6 to control the speed of left motor
#define INB 8  //Define pin 8 to control the direction of right motor
#define ENB 5  //Define pin 5 to control the speed of right motor

void setup() {
  Serial.begin(115200);  //Define the baud rate to 115200
  Serial.println("Enabling IRin");
  IrReceiver.enableIRIn();  // Start the receiver
  //IrReceiver.blink13(true); // Enable feedback LED

  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(IR_RECEIVE_PIN);
  pinMode(INA, OUTPUT);  //Set the pins controlling the motors to output
  pinMode(ENA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(ENB, OUTPUT);
}

void loop() {
  if(IrReceiver.decode(&results))
  {
    ir_val = results.value;
    //IrReceiver.printResultShort(&Serial);
    Serial.println(ir_val,HEX);
    if(ir_val == 0xFFFF)
    {
      ir_FFFF = ir_val;
    }
    else
    {
      ir_val_data = ir_val;
    }
    switch(ir_val_data)
    {
      case 0x18E7: car_forward(); break;  //The little moves forward
      case 0x4AB5: car_back(); break;     //The little moves backward
      case 0x10EF: car_left(); break;     //The little turns left
      case 0x5AA5: car_right(); break;    //The little turns right
    }
    IrReceiver.resume(); // Receive the next value
  }
  else
  {
    car_stop(); //Stop
  }
  delay(100);
}

//The little moves forward
void car_forward()
{
  digitalWrite(INA, HIGH);  //Output high level and control the left motor forward
  analogWrite(ENA, 200);    //PMW output to control the speed of left motor
  digitalWrite(INB, HIGH);  //Output high level and control the right motor forward
  analogWrite(ENB, 200);    //PMW output to control the speed of right motor
}
//The little snail moves backward
void car_back()
{
  digitalWrite(INA, LOW);  //Output low level and control the left motor backward
  analogWrite(ENA, 200);   //PMW output to control the speed of left motor
  digitalWrite(INB, LOW);  //Output low level and control the right motor backward
  analogWrite(ENB, 200);   //PMW output to control the speed of right motor
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
