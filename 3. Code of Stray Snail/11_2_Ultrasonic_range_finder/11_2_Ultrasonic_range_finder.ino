#include <Wire.h>
float distance = 0;  //Decimal distance value
float ds[3];  //Create an array of float numbers to store the values read
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define btnPin1 2  //Define button pin
//The variable is used to receive the value detected by the button
boolean btnVal1;
int count;
boolean ranging_state = 0;  //Used to decide whether to start ranging

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(btnPin1,INPUT);  //Set pin 11 to output

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

   // Clear the buffer
  display.clearDisplay();
  display.display();
}

void loop() {
  btnVal1 = digitalRead(btnPin1);  //Read the button value and assign it to btnVal1
  if(btnVal1 == 0)  //If button 1 is pressed
  {
    delay(10);
    if(btnVal1 == 0)
    {
      boolean i = 1;
      while(i == 1)
      {
        btnVal1 = digitalRead(btnPin1);  //Read the button value and assign it to btnVal1
        if(btnVal1 == 1)
        {
          count++;
          Serial.println(count);
          i = 0;
        }
      }
      ranging_state = count % 2; //Calculate the remainder. If it is singular, the value is equal to 1, and if it is even, the value is equal to 0
    }
    
  }
  if(ranging_state == 1)
  {
    distance = checkdistance();  //Read the value detected by ultrasonic sensor
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Distance : "));
    display.println(distance);
    display.display();
    display.clearDisplay();
  }
  else
  { 
    display.setTextSize(2);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("Distance : "));
    display.println(distance);
    display.display();
    display.clearDisplay();
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
  delay(120);                     //Measure cycle delay: One cycle is 100ms, set 120ms to keep some allowance   
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
