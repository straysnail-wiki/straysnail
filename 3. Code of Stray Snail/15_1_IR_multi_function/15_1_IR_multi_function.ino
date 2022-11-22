#include <IRremote.h>
#define IR_RECEIVE_PIN  3  //Define the pins of the infrared receiver module 
IRrecv IrReceiver(IR_RECEIVE_PIN);
decode_results results;
int ir_val;
int ir_FFFF;
int ir_val_data;
#define INA 7  //Define the pins of the motor
#define ENA 6
#define INB 8 
#define ENB 5  

#define ledPin A3 //LED PIN
int ledState = 0;

#include <NewTone.h> //Import library file of sound frequency (no timer required)
#define buzzerPin A3   //buzzer PIN

//Tunnel light
#include <Adafruit_NeoPixel.h>
#define Neo_PIN  A0
#define NUMPIXELS 11
Adafruit_NeoPixel pixels(NUMPIXELS, Neo_PIN, NEO_GRB + NEO_KHZ800);
#define DELAYVAL 100
int neoFlag = 0;

#include <Servo.h>
Servo door_servo;  //Create an instance
Servo window_servo;
Servo head_servo;
int pulsewidth = 0; //Variable, used to calculate the steering gear pulse value
#include <Wire.h> 
float distance = 0;  //Value of distance
float ds[3];  //Creates an array of floating-point numbers to hold the values read

#define wayPin1 9
#define wayPin2 10

#define L_light A1 //Define the pins of the photosensitive sensor
#define R_light A2


void setup() {
  Serial.begin(115200);  //The baud rate of the serial port is defined as 115200
  Serial.println("Enabling IRin");
  IrReceiver.enableIRIn();  // Start the receiver
  IrReceiver.blink13(true); // Enable feedback LED

  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(IR_RECEIVE_PIN);
  pinMode(INA, OUTPUT);  //Set the pin of the control motor to output mode
  pinMode(ENA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin,OUTPUT);
  door_servo.attach(11,500,2500);  //servo PIN
  window_servo.attach(4,500,2500);
  head_servo.attach(12,500,2500);

  //Example Initialize the tunnel light
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)

  Wire.begin();
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
      case 0x18E7: car_forward(); break;  //forward
      case 0x4AB5: car_back(); break;     //back
      case 0x10EF: car_left(); break;     //left
      case 0x5AA5: car_right(); break;    //right
      case 0xFFFFA25D: LED_mode(); break;
      case 0x629D: play_music(buzzerPin); break;
      case 0xFFFFE21D: neoPiexl(); break;
      case 0x22DD: Serial.println("open the door");door_servo.attach(11,500,2500);door_servo.write(140);delay(300); break;
      case 0x2FD: door_servo.attach(11,500,2500);door_servo.write(50);delay(300); break;
      case 0xFFFFC23D: window_servo.attach(4,500,2500);window_servo.write(50);delay(300); break;
      case 0xFFFFE01F: window_servo.attach(4,500,2500);window_servo.write(116);delay(300); break;
      case 0xFFFFA857: car_tracking(); break;
      case 0xFFFF906F: follow_car_mode(); break;
      case 0x6897: avoid_car_mode(); break;
      case 0xFFFF9867: car_findLight_mode(); break;
      case 0xFFFFB04F:  ; break;
    }
    IrReceiver.resume(); // Receive the next value
  }
  else
  {
    car_stop(); //Stop
  }
  delay(100);
}

//Light control car function
void car_findLight_mode()
{
  boolean findLight_flag = 1;
  while(findLight_flag == 1)
  {
    pinMode(L_light, INPUT);
    pinMode(R_light, INPUT);
    int L_lightVal = analogRead(L_light);
    int R_lightVal = analogRead(R_light);
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

    if(IrReceiver.decode(&results)) {
      ir_val=results.value;
      String type="UNKNOWN";
      String typelist[18]={"UNUSED", "RC5", "RC6", "NEC", "SONY", "PANASONIC", "JVC", "SAMSUNG", "WHYNTER", "AIWA_RC_T501", "LG", "SANYO", "MITSUBISHI", "DISH", "SHARP", "DENON", "PRONTO", "LEGO_PF"};
      if(results.decode_type>=1&&results.decode_type<=17){
        type=typelist[results.decode_type];
      }
      //Serial.println("IR TYPE:"+type+"  ");
      Serial.println(ir_val,HEX);
      IrReceiver.resume();
      if(ir_val == 0X38C7)
      {
        findLight_flag = 0;
        car_stop();
        Serial.println("exit mode");
        //break;
      }
    }
  }
  
}

//Function of tracking
void car_tracking()
{
  boolean track_flag = 1;
  while(track_flag == 1)
  {
    boolean L_tval = digitalRead(wayPin1);
    boolean R_tval = digitalRead(wayPin2);
    Serial.print(L_tval);
    Serial.print("  ");
    Serial.println(R_tval);
    delay(100);
    if((L_tval == 1) && (R_tval == 1))
    {
      track_forward();
    }
    else if((L_tval == 1) && (R_tval == 0))
    {
      track_left();
      
    }
    else if((L_tval == 0) && (R_tval == 1))
    {
      track_right();
    }
    else if((L_tval == 0) && (R_tval == 0))
    {
      car_stop();
    }

    if(IrReceiver.decode(&results)) {
      ir_val=results.value;
      String type="UNKNOWN";
      String typelist[18]={"UNUSED", "RC5", "RC6", "NEC", "SONY", "PANASONIC", "JVC", "SAMSUNG", "WHYNTER", "AIWA_RC_T501", "LG", "SANYO", "MITSUBISHI", "DISH", "SHARP", "DENON", "PRONTO", "LEGO_PF"};
      if(results.decode_type>=1&&results.decode_type<=17){
        type=typelist[results.decode_type];
      }
      //Serial.println("IR TYPE:"+type+"  ");
      Serial.println(ir_val,HEX);
      IrReceiver.resume();
      if(ir_val == 0X38C7)
      {
        track_flag = 0;
        car_stop();
        Serial.println("exit mode");
        //break;
      }
    }
  }
  
}

//Ultrasonic ranging function
float checkdistance() {
  char i = 0;
  ds[0]=0;
  ds[1]=0;
  ds[2]=0;
  Wire.beginTransmission(0x57);   //The address is 0X57
  Wire.write(1);                  //Write the command 0X01. 0X01 is the start measurement command
  Wire.endTransmission();            
  delay(10);                     //Measurement cycle delay
  Wire.requestFrom(0x57,3);       //The address is 0X57 to read three 8-bit distance data       
  while (Wire.available())
  {
   ds[i++] = Wire.read();
  }          
  distance=(ds[0]*65536+ds[1]*256+ds[2])/10000; //I'm going to compute it in CM     
  Serial.print("distance : "); 
  if ((1<=distance)&&(600>=distance))           //Values between 1CM and 6M are displayed
  {
  Serial.print(distance);
   Serial.print(" CM ");  
  }
  else 
  {
  Serial.print(" - - - - ");                   //Invalid values are displayed at ---- 
  }
  Serial.println();    
  delay(10);                                  //Adjust the measurement period 
  return distance;
}
//Follow the car function
void follow_car_mode()
{
  boolean follow_flag = 1;
  int distance = 0;
  head_servo.write(90);
  delay(200);
  while(follow_flag == 1)
  {
    distance = checkdistance(); //Gets the value returned by the ultrasonic function
    Serial.print("distance = ");
    Serial.println(distance);
    if(distance < 8)
    {
      digitalWrite(INA, LOW); 
      analogWrite(ENA, 100); 
      digitalWrite(INB, LOW);
      analogWrite(ENB, 100);
    }
    else if((distance >= 8) && (distance <= 16))
    {
      car_stop();
    }
    else if((distance > 16) && (distance <= 45))
    {
      digitalWrite(INA, HIGH);
      analogWrite(ENA, 100);
      digitalWrite(INB, HIGH);
      analogWrite(ENB, 100);
    }
    else
    {
      car_stop();
    }

    if(IrReceiver.decode(&results)) {
      ir_val=results.value;
      String type="UNKNOWN";
      String typelist[18]={"UNUSED", "RC5", "RC6", "NEC", "SONY", "PANASONIC", "JVC", "SAMSUNG", "WHYNTER", "AIWA_RC_T501", "LG", "SANYO", "MITSUBISHI", "DISH", "SHARP", "DENON", "PRONTO", "LEGO_PF"};
      if(results.decode_type>=1&&results.decode_type<=17){
        type=typelist[results.decode_type];
      }
      //Serial.println("IR TYPE:"+type+"  ");
      Serial.println(ir_val,HEX);
      IrReceiver.resume();
      if(ir_val == 0X38C7)
      {
        follow_flag = 0;
        car_stop();
        Serial.println("exit mode");
        //break;
      }
    }
  }
  
}

//Obstacle avoidance car function
void avoid_car_mode()
{
  boolean avoid_flag = 1;
  int distance1 = 0;
  int distance2 = 0;
  int distance3 = 0;
  while(avoid_flag == 1)
  {
    distance1 = checkdistance(); //Gets the value returned by the ultrasonic function
    Serial.print("distance1 = ");
    Serial.println(distance1);
    if((distance1 > 15) || (distance1 == 0))
    {
      digitalWrite(INA, HIGH);
      analogWrite(ENA, 100);
      digitalWrite(INB, HIGH);
      analogWrite(ENB, 100);
    }
    else
    {
      car_stop();
      //pinMode(head_servo, OUTPUT);
      //procedure(head_servo,90);
      head_servo.write(180);
      delay(300);
      distance2 = checkdistance();
      delay(100);
      //procedure(head_servo, 0);
      head_servo.write(15);
      delay(500);
      distance3 = checkdistance();
      delay(100);
      if(distance2 > distance3)
      {
        car_left();
        //procedure(head_servo,90);
        head_servo.write(90);
        delay(300);
      }
      else
      {
        car_right();
        //procedure(head_servo, 90);
        head_servo.write(90);
        delay(300);
      }
    }

    if(IrReceiver.decode(&results)) {
      ir_val=results.value;
      String type="UNKNOWN";
      String typelist[18]={"UNUSED", "RC5", "RC6", "NEC", "SONY", "PANASONIC", "JVC", "SAMSUNG", "WHYNTER", "AIWA_RC_T501", "LG", "SANYO", "MITSUBISHI", "DISH", "SHARP", "DENON", "PRONTO", "LEGO_PF"};
      if(results.decode_type>=1&&results.decode_type<=17){
        type=typelist[results.decode_type];
      }
      //Serial.println("IR TYPE:"+type+"  ");
      Serial.println(ir_val,HEX);
      IrReceiver.resume();
      if(ir_val == 0X38C7)
      {
        avoid_flag = 0;
        car_stop();
        Serial.println("exit mode");
        //break;
      }
    }
  }
  
}

//forward
void car_forward()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 200);
  digitalWrite(INB, HIGH); 
  analogWrite(ENB, 200);
}

void car_back()
{
  digitalWrite(INA, LOW);
  analogWrite(ENA, 200);
  digitalWrite(INB, LOW);
  analogWrite(ENB, 200);
}

void car_left()
{
  digitalWrite(INA, LOW);
  analogWrite(ENA, 200);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 200);
}

void car_right()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 200);
  digitalWrite(INB, LOW);
  analogWrite(ENB, 200);
}

void track_forward()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 50);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 50);
}
void track_left()
{
  digitalWrite(INA, LOW);
  analogWrite(ENA, 70);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 80);
}
void track_right()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 80);
  digitalWrite(INB, LOW);
  analogWrite(ENB, 70);
}

void car_stop()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 0);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 0);
}

//LED mode
void LED_mode()
{
  ledState = ledState + 1;
  Serial.println(ledState);
  if(ledState == 1)
  {
    digitalWrite(ledPin, HIGH);
  }
  if(ledState >= 2)
  {
    digitalWrite(ledPin, LOW);
    ledState = 0;
  }
}

//barthday
void play_music(int buzzer_pin)
{
  NewTone(buzzer_pin,294,250);
  NewTone(buzzer_pin,440,250);
  NewTone(buzzer_pin,392,250);
  NewTone(buzzer_pin,532,250);
  NewTone(buzzer_pin,494,500);
  NewTone(buzzer_pin,392,250);
  NewTone(buzzer_pin,440,250);
  NewTone(buzzer_pin,392,250);
  NewTone(buzzer_pin,587,250);
  NewTone(buzzer_pin,532,500);
  NewTone(buzzer_pin,392,250);
  NewTone(buzzer_pin,784,250);
  NewTone(buzzer_pin,659,250);
  NewTone(buzzer_pin,532,250);
  NewTone(buzzer_pin,494,250);
  NewTone(buzzer_pin,440,250);
  NewTone(buzzer_pin,698,375);
  NewTone(buzzer_pin,659,250);
  NewTone(buzzer_pin,532,250);
  NewTone(buzzer_pin,587,250);
  NewTone(buzzer_pin,532,500);
  NewNoTone(buzzer_pin);
}

void neoPiexl()
{
  neoFlag = neoFlag + 1;
  Serial.println(neoFlag);
  switch(neoFlag)
  {
    case 1: 
      for(int i=0;i<11;i++)
      {
        pixels.setPixelColor(i, pixels.Color(150, 0, 0));
      }
      pixels.show();
      break;
    case 2: 
      for(int i=0;i<11;i++)
      {
        pixels.setPixelColor(i, pixels.Color(0, 150, 0));
      }
      pixels.show();
      break;
    case 3: 
      for(int i=0;i<11;i++)
      {
        pixels.setPixelColor(i, pixels.Color(0, 0, 150));
      }
      pixels.show();
      break;
    case 4: 
      for(int i=0;i<11;i++)
      {
        pixels.setPixelColor(i, pixels.Color(150, 150, 0));
      }
      pixels.show();
      break;
    case 5: 
      for(int i=0;i<11;i++)
      {
        pixels.setPixelColor(i, pixels.Color(0, 150, 150));
      }
      pixels.show();
      break;
    case 6: 
      for(int i=0;i<11;i++)
      {
        pixels.setPixelColor(i, pixels.Color(150, 150, 150));
      }
      pixels.show();
      break;
    case 7: 
      for(int i=0;i<11;i++)
      {
        pixels.setPixelColor(i, pixels.Color(150, 30, 50));
      }
      pixels.show();
      break;
    case 8: 
      pixels.clear();
      for(int i=0; i<NUMPIXELS; i++) {
        pixels.setPixelColor(i, pixels.Color(50, 150, 230));
        pixels.show();
        delay(DELAYVAL);
      }
      break;
     case 9: 
      for(int i=0;i<11;i++)
      {
        pixels.setPixelColor(i, pixels.Color(0, 0, 0));
        pixels.show();
      }
      break;
  }
  if(neoFlag >= 9)
  {
    neoFlag = 0;
  }
}

void stop_neoPiexl()
{
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    pixels.show();
  }
  pixels.clear();
}
