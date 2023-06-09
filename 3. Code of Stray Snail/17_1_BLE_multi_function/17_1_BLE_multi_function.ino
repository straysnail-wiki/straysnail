#include <NewTone.h>
#include<Adafruit_NeoPixel.h>
#define Neo_PIN  A0
#define NUMPIXELS  11
Adafruit_NeoPixel pixels(NUMPIXELS, Neo_PIN, NEO_GRB + NEO_KHZ800);
#include<Wire.h>

//Define motor drive pins
#define INA 7
#define ENA 6
#define INB 8
#define ENB 5
#define door_servo 11  //door servo
#define window_servo 4  //window servo 
#define head_servo 12  //head servo
#define LED_pin A3  //LED
#define buzzerPin A3   //buzzer
#define L_light A1 //photosensitive sensor
#define R_light A2
int distance = 0;
float ds[3];
int distance1;
int distance2;
int distance3;
int distance_val;  //The final filtered ultrasonic value
#define wayPin1 9
#define wayPin2 10
boolean L_tval = 0;
boolean R_tval = 0;

int seek_r,seek_g,seek_b;
int seek_ml = 150;
int seek_mr = 150;

char bleVal;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(head_servo, OUTPUT);
  pinMode(door_servo, OUTPUT);
  procedure(door_servo, 42);
  delay(200);
  pinMode(window_servo, OUTPUT);
  procedure(window_servo, 114);
  delay(200);
  pinMode(INA, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(wayPin1, INPUT);
  pinMode(wayPin2, INPUT);
  pinMode(L_light, INPUT);
  pinMode(R_light, INPUT);
  pinMode(Neo_PIN, OUTPUT);
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(150, 0, 0));
  } 
  pixels.show();
  delay(300);
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 150, 0));
  } 
  pixels.show();
  delay(300);
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 150));
  } 
  pixels.show();
  delay(1000);
  for(int i=0; i<NUMPIXELS; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 0));
  } 
  pixels.show();
}

void loop() {
  SerialRead();
  parseData();
}

void SerialRead()
{
  if(Serial.available() > 0)
  {
    String bleVal1 = Serial.readStringUntil('*');
    if(bleVal1[0] == '$')
    {
      bleVal = bleVal1[1];
      Serial.println(bleVal);
      if(bleVal1[1] == 'l')
      {
        seek_r = String(String(bleVal1).substring(2,String(bleVal1).length())).toInt();
        Serial.println(seek_r);
        //Serial.println(String(bleVal1).substring(2,String(bleVal1).length()));
        displayPiexls(NUMPIXELS, seek_r, seek_g, seek_b);pixels.show();
      }
      if(bleVal1[1] == 'm')
      {
        seek_g = String(String(bleVal1).substring(2,String(bleVal1).length())).toInt();
        Serial.println(seek_g);
        displayPiexls(NUMPIXELS, seek_r, seek_g, seek_b);pixels.show();
      }
      if(bleVal1[1] == 'n')
      {
        seek_b = String(String(bleVal1).substring(2,String(bleVal1).length())).toInt();
        Serial.println(seek_b);
        displayPiexls(NUMPIXELS, seek_r, seek_g, seek_b);pixels.show();
      }
      if(bleVal1[1] == 'u')
      {
        seek_ml = String(String(bleVal1).substring(2,String(bleVal1).length())).toInt();
        Serial.println(seek_ml);
      }
      if(bleVal1[1] == 'v')
      {
        seek_mr = String(String(bleVal1).substring(2,String(bleVal1).length())).toInt();
        Serial.println(seek_mr);
      }
    }
  }
}

void parseData()
{
  switch(bleVal)
  {
    case 'F': car_forward(); break;
    case 'B': car_back(); break;
    case 'L': car_left(); break;
    case 'R': car_right(); break;
    case 'S': car_stop(); break;
    case 'a': digitalWrite(LED_pin, HIGH); break;
    case 'b': digitalWrite(LED_pin, LOW); break;
    case 'c': NewTone(buzzerPin,532,250); break;
    case 'd': NewNoTone(buzzerPin); break;
    case '1': displayPiexls(NUMPIXELS, 150, 0, 0);pixels.show(); break;
    case '2': displayPiexls(NUMPIXELS, 255, 128, 0);pixels.show(); break;
    case '3': displayPiexls(NUMPIXELS, 255, 255, 0);pixels.show(); break;
    case '4': displayPiexls(NUMPIXELS, 0, 150, 0);pixels.show(); break;
    case '5': displayPiexls(NUMPIXELS, 0, 255, 255);pixels.show(); break;
    case '6': displayPiexls(NUMPIXELS, 0, 0, 150);pixels.show(); break;
    case '7': displayPiexls(NUMPIXELS, 138, 43, 226);pixels.show(); break;
    case '8': displayPiexls(NUMPIXELS, 150, 150, 150);pixels.show(); break;
    case '9': displayPiexls(NUMPIXELS, 0, 0, 0);pixels.show(); break;
    case 'z': breath_rgb(); break;
    case 'h': colorWipe(pixels.Color(255,0,0),100); colorWipe(pixels.Color(0,255,0),100);colorWipe(pixels.Color(0,0,255),100);break;
    case 'i': theaterChase(pixels.Color(127,127,127),50);theaterChase(pixels.Color(127,0,0),50);theaterChase(pixels.Color(0,0,127),50);break;
    case 'j': rainbow(10);break;
    case 'k': theaterChaseRainbow(50);break;
    case 'l': break;
    case 'm': break;
    case 'n': break;

    case 't': procedure(head_servo, 166); break;
    case 'y': procedure(head_servo, 80); break;
    case 'x': procedure(head_servo, 7); break;
    case 'A': procedure(door_servo, 110);break;
    case 'C': procedure(door_servo, 42); break;
    case 'D': procedure(window_servo, 50);break;
    case 'E': procedure(window_servo, 114);break;
    
    case 'G': car_tracking(); break;
    case 'H': car_avoid(); break;
    case 'I': car_circle(); break;
    case 'J': car_follow(); break;
    case 'K': car_find_light(); break;
    case 'M': play_music(buzzerPin); break;

    
    case 'f': int distance1 = checkdistance(); delay(100); break;
    case 'g': Serial.println("000"); break;
                               
  }
}

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

void car_find_light()
{
  boolean flag = 1;
  while(flag == 1)
  {
    int L_lightVal = analogRead(L_light);
    int R_lightVal = analogRead(R_light);
    Serial.print(L_lightVal);
    Serial.print("  ");
    Serial.println(R_lightVal);
    delay(100);
    if((L_lightVal >= 800) && (R_lightVal >= 800))
    {
      digitalWrite(INA, HIGH);
      analogWrite(ENA, 155);
      digitalWrite(INB, HIGH);
      analogWrite(ENB, 155);
    }
    else if((L_lightVal >= 800) && (R_lightVal < 800))
    {
      digitalWrite(INA, LOW);
      analogWrite(ENA, 150);
      digitalWrite(INB, HIGH);
      analogWrite(ENB, 150);
    }
    else if((L_lightVal < 800) && (R_lightVal >= 800))
    {
      digitalWrite(INA, HIGH);
      analogWrite(ENA, 150);
      digitalWrite(INB, LOW);
      analogWrite(ENB, 150);
    }
    else if((L_lightVal < 800) && (R_lightVal < 800))
    {
      car_stop();
    }
    while(Serial.available() > 0)
    {
      bleVal = Serial.read();
      Serial.println(bleVal);
      if(bleVal == 'S')
      {
        car_stop();
        flag = 0;
      }
    }
  }
}

void car_follow()
{
  boolean flag = 1;
  while(flag == 1)
  {
    procedure(head_servo, 80);
    distance = checkdistance(); //Obtain the ultrasonic value
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
    else if((distance > 16) && (distance <= 55))
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
    while(Serial.available() > 0)
    {
      bleVal = Serial.read();
      Serial.println(bleVal);
      if(bleVal == 'S')
      {
        car_stop();
        flag = 0;
      }
    }
  }
}

void car_avoid()
{
  boolean flag = 1;
  while(flag == 1)
  {
    distance1 = checkdistance();
    Serial.print("distance1 = ");
    Serial.println(distance1);
    if(distance1 > 15)
    {
      digitalWrite(INA, HIGH);
      analogWrite(ENA, 100);
      digitalWrite(INB, HIGH);
      analogWrite(ENB, 100);
    }
    else
    {
      car_stop();
      procedure(head_servo, 170);
      delay(300);
      distance2 = checkdistance();
      delay(100);
      procedure(head_servo, 15);
      delay(500);
      distance3 = checkdistance();
      delay(100);
      if(distance2 > distance3)
      {
        digitalWrite(INA, LOW);
        analogWrite(ENA, 200);
        digitalWrite(INB, HIGH);
        analogWrite(ENB, 200);
        procedure(head_servo, 90);
        delay(300);
      }
      else
      {
        digitalWrite(INA, HIGH);
        analogWrite(ENA, 200);
        digitalWrite(INB, LOW);
        analogWrite(ENB, 200);
        procedure(head_servo, 90);
        delay(300);
      }
    }
    while(Serial.available() > 0)
    {
      bleVal = Serial.read();
      Serial.println(bleVal);
      if(bleVal == 'S')
      {
        car_stop();
        flag = 0;
      }
    }
  }
}

void car_circle()
{
  boolean flag = 1;
  while(flag == 1)
  {
    L_tval = digitalRead(wayPin1);
    R_tval = digitalRead(wayPin2);
    Serial.print(L_tval);
    Serial.print("  ");
    Serial.println(R_tval);
    delay(100);
    if((L_tval == 1) && (R_tval == 1))
    {
      car_back();
      delay(100);
      car_left();
      delay(200);
    }
    else if((L_tval == 1) && (R_tval == 0))
    {
      car_right();
      delay(100);
    }
    else if((L_tval == 0) && (R_tval == 1))
    {
      car_left();
      delay(100);
    }
    else if((L_tval == 0) && (R_tval == 0))
    {
      car_forward();
    }
    while(Serial.available() > 0)
    {
      bleVal = Serial.read();
      Serial.println(bleVal);
      if(bleVal == 'S')
      {
        car_stop();
        flag = 0;
      }
    }
  }
}

void car_tracking()
{
  boolean flag = 1;
  while(flag == 1)
  {
    L_tval = digitalRead(wayPin1);
    R_tval = digitalRead(wayPin2);
    Serial.print(L_tval);
    Serial.print("  ");
    Serial.println(R_tval);
    delay(100);
    if((L_tval == 1) && (R_tval == 1))
    {
      digitalWrite(INA, HIGH);
      analogWrite(ENA, 56);
      digitalWrite(INB, HIGH);
      analogWrite(ENB, 56);
    }
    else if((L_tval == 1) && (R_tval == 0))
    {
      digitalWrite(INA, LOW);
      analogWrite(ENA, 90);
      digitalWrite(INB, HIGH);
      analogWrite(ENB, 100);      
    }
    else if((L_tval == 0) && (R_tval == 1))
    {
      digitalWrite(INA, HIGH);
      analogWrite(ENA, 100);
      digitalWrite(INB, LOW);
      analogWrite(ENB, 90);
    }
    else if((L_tval == 0) && (R_tval == 0))
    {
      car_stop();
    }
    while(Serial.available() > 0)
    {
      bleVal = Serial.read();
      Serial.println(bleVal);
      if(bleVal == 'S')
      {
        car_stop();
        flag = 0;
      }
    }
  }
}


int displayPiexls(int num, int redVal, int greenVal, int blueVal)
{
  for(int i=0; i<num; i++) {
    pixels.setPixelColor(i, pixels.Color(redVal, greenVal, blueVal));
  }
}

void breath_rgb()
{
  for(int bright_val=0;bright_val<256;bright_val++)
  {
    displayPiexls(NUMPIXELS, 0, bright_val, bright_val);
    pixels.show();
    delay(10);
  }
  for(int bright_val=255;bright_val>0;bright_val--)
  {
    displayPiexls(NUMPIXELS, 0, bright_val, bright_val);
    pixels.show();
    delay(10);
  }
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
    pixels.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    pixels.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void theaterChase(uint32_t color, int wait) {
  for(int a=0; a<10; a++) {  // Repeat 10 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for(int c=b; c<pixels.numPixels(); c += 3) {
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      pixels.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void rainbow(int wait) {
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<pixels.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / pixels.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      pixels.setPixelColor(i, pixels.gamma32(pixels.ColorHSV(pixelHue)));
    }
    pixels.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      pixels.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<pixels.numPixels(); c += 3) {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int      hue   = firstPixelHue + c * 65536L / pixels.numPixels();
        uint32_t color = pixels.gamma32(pixels.ColorHSV(hue)); // hue -> RGB
        pixels.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      pixels.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}

void car_forward()
{
  digitalWrite(INA, HIGH); 
  analogWrite(ENA, seek_ml);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, seek_mr);
}

void car_back()
{
  digitalWrite(INA, LOW);
  analogWrite(ENA, seek_ml);
  digitalWrite(INB, LOW);
  analogWrite(ENB, seek_mr);
}

void car_left()
{
  digitalWrite(INA, LOW);
  analogWrite(ENA, seek_ml);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, seek_mr);
}

void car_right()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, seek_ml);
  digitalWrite(INB, LOW);
  analogWrite(ENB, seek_mr);
}

void car_stop()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 0);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 0);
}

float checkdistance() {
  char i = 0;
  ds[0]=0;
  ds[1]=0;
  ds[2]=0;
  Wire.beginTransmission(0x57);
  Wire.write(1);
  Wire.endTransmission();            
  delay(10);
  Wire.requestFrom(0x57,3);
  while (Wire.available())
  {
   ds[i++] = Wire.read();
  }          
  distance=(ds[0]*65536+ds[1]*256+ds[2])/10000;
  //Serial.print("distance : "); 
  if ((1<=distance)&&(600>=distance))
  {
   Serial.println(distance);
   distance_val = distance;
   //Serial.print(" CM ");  
  }
  else 
  {
  Serial.print(" - - - - ");
  }
  //Serial.println();    
  delay(10);
  return distance_val;
}

void procedure(int serPin, int myangle)
{
  int i=0;
  while(i<15)
  {
    int pulsewidth = myangle * 11 + 600;  
    digitalWrite(serPin,HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(serPin,LOW);
    delay((20 - pulsewidth / 1000));
    i++;
  }
}
