#include<NewTone.h>
#include<Adafruit_NeoPixel.h>
#define Neo_PIN  A0  //Define tunnel light pins
#define NUMPIXELS  11 //Define the number of beads
Adafruit_NeoPixel pixels(NUMPIXELS, Neo_PIN, NEO_GRB + NEO_KHZ800);

#include<Wire.h>

#include<Adafruit_GFX.h>
#include<Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int tunnel_count;

#include<OneButton.h>
// Setup a new OneButton on pin 2
OneButton button1(2, true);
// Setup a new OneButton on pin 13 
OneButton button2(13, true);

String passward = ".._.";  //passward
String E_passward = "";  //Store the entered password

#define door_servo 11  //servo PIN
#define window_servo 4  
int count1 = 0;
int count2 = 0;
int pulsewidth = 0;

#define LED_pin A3
#define btn_pin1 2
#define btn_pin2 13

int mode_count = 0;
boolean OLED_LED = 1;  //The first mode is LED, so its display variable is 1 
boolean OLED_servo = 0;
boolean OLED_tunnel = 0;
boolean OLED_music = 0;
boolean OLED_follow_car = 0;
boolean OLED_avoid_car = 0;
boolean OLED_tracking_car = 0;
boolean OLED_find_car_light = 0;

#define lightPin A1  //Light sensor pin A1 on the left

float distance = 0;
float ds[3];
int distance1;
int distance2;
int distance3;
#define head_servo 12

#define INA 7  //Define motor drive pins
#define ENA 6
#define INB 8
#define ENB 5

#define wayPin1 9
#define wayPin2 10
boolean L_tval = 0;
boolean R_tval = 0;

#define buzzerPin A3   //buzzer Pin

#define L_light A1
#define R_light A2
char bleVal;


void setup() {
  Serial.begin(9600);
  //IrReceiver.enableIRIn();  // Start the receiver
  pinMode(head_servo, OUTPUT);
  pinMode(door_servo, OUTPUT);
  procedure(door_servo, 46); //close the door
  delay(500);
  pinMode(window_servo, OUTPUT);
  procedure(window_servo, 112); //close the window
  delay(500);
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  // Clear the buffer
  display.clearDisplay();
  display.display();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,0);             // Start at top-left corner
  display.println(F("LED"));
  display.println(F("Door and window"));
  display.println(F("Tunnel light"));
  display.println(F("Music"));
  display.println(F("Follow Car"));
  display.println(F("Avoid Car"));
  display.println(F("Tracking Car"));
  display.println(F("Find car light"));
  display.display();
  delay(1000);
  
  // link the button 1 functions.
  button1.attachClick(click1);
  button1.attachDoubleClick(doubleclick1);
  
  display.setTextColor(SSD1306_WHITE);        // Draw white text
  display.setCursor(0,10);             // Start at top-left corner

  pixels.begin(); // init NeoPixel
  pixels.clear(); //clear
  pixels.show();//display

  pinMode(INA, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(INB, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(wayPin1, INPUT);
  pinMode(wayPin2, INPUT);

}

void loop() {
  //APP control
  if(Serial.available() > 0)
  {
    String bleVal1 = Serial.readStringUntil('*');
    if(bleVal1[0] == '$')
    {
      bleVal = bleVal1[1];
      Serial.println(bleVal);
    }
    switch(bleVal)
    {
      case 'F': car_forward(); break;
      case 'B': car_back(); break;
      case 'L': car_left(); break;
      case 'R': car_right(); break;
      case 'S': car_stop(); break;
    }
  }

  // keep watching the push buttons:
  button1.tick();
  button2.tick();
}

// ----- button 1 callback functions

// This function will be called when the button1 was pressed 1 time (and no 2. button press followed).
void click1() {
  mode_count++;
  if(mode_count >= 9)
  {
    mode_count = 0;
  }
  Serial.println(mode_count);
  switch(mode_count)
  {
    case 1: select_LED_mode(); break;
    case 2: select_Door_and_window_mode(); break;
    case 3: select_Tunnel_light_mode(); break;
    case 4: select_Music_mode(); break;
    case 5: select_Follow_car_mode(); break;
    case 6: select_Avoid_car_mode(); break;
    case 7: select_Tracking_car_mode(); break;
    case 8: select_Find_car_light_mode(); break; 
  }
} // click1




// This function will be called when the button1 was pressed 2 times in a short timeframe.
void doubleclick1() {
  Serial.println("Button 1 doubleclick.");
  Serial.println(mode_count);
  switch(mode_count)
  {
    case 1: LED_mode(); break;
    case 2: Door_and_window_mode(); break;
    case 3: Tunnel_light_mode(); break;
    case 4: Music_mode(); break;
    case 5: Follow_car_mode(); break;
    case 6: Avoid_car_mode(); break;
    case 7: Tracking_car_mode(); break;
    case 8: Find_car_light_mode(); break; 
  }
} // doubleclick1


void LED_mode()
{
  boolean LED_flag = 1;
  boolean i = 1;
  while(LED_flag == 1)
  {
    while(i == 1)
    {
      display.clearDisplay();
      display.display();
      display.setTextSize(2);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0,0);             // Start at top-left corner
      display.println(F("LED mode"));
      display.setTextSize(1);
      display.println(F("Btn1 turn off the LED"));
      display.println(F("Btn2 turn on the LED"));
      display.display();
      i = 0;
    }
    pinMode(btn_pin1,INPUT);
    pinMode(btn_pin2,INPUT);
    boolean btn1_val = digitalRead(btn_pin1);
    boolean btn2_val = digitalRead(btn_pin2);
    if(btn1_val == 0)
    {
      digitalWrite(LED_pin, LOW);
    }
    if(btn2_val == 0)
    {
      digitalWrite(LED_pin, HIGH);
    }
    if((btn1_val == 0) && (btn2_val == 0))
    {
      Serial.println("Exit the mode");
      boolean L = 1;
      while(L == 1)
      {
        display.clearDisplay();
        display.display();
        display.setTextSize(1);             // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        display.setCursor(0,0);             // Start at top-left corner
        display.println(F("^  LED"));
        display.println(F("Door and window"));
        display.println(F("Tunnel light"));
        display.println(F("Music"));
        display.println(F("Follow Car"));
        display.println(F("Avoid Car"));
        display.println(F("Tracking Car"));
        display.println(F("Find car light"));
        display.display();
        L = 0;
      }
      digitalWrite(LED_pin, LOW);
      LED_flag = 0;
      mode_count = 0;
      OLED_LED = 1;
    }
  }
  
}

//servo function
void procedure(int serPin, int myangle)
{
  int i=0;
  while(i<15)
  {
    pulsewidth = myangle * 11 + 600;  
    digitalWrite(serPin,HIGH);
    delayMicroseconds(pulsewidth);
    digitalWrite(serPin,LOW);
    delay((20 - pulsewidth / 1000));
    i++;
  }
}
void Door_and_window_mode()
{
  boolean servo_flag = 1;
  boolean i = 1;
  while(servo_flag == 1)
  {
    while(i == 1)
    {
      pinMode(door_servo, OUTPUT);
      pinMode(window_servo, OUTPUT);
      display.clearDisplay();
      display.display();
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0,0);             // Start at top-left corner
      display.println(F("door_window mode"));
      display.println(F(""));
      display.println(F("Btn2 open and close the door"));
      display.display();
      i = 0;
    }
    pinMode(btn_pin1,INPUT);
    pinMode(btn_pin2,INPUT);
    boolean btn1_val = digitalRead(btn_pin1);
    boolean btn2_val = digitalRead(btn_pin2);
   
    if((btn1_val == 0) && (btn2_val == 0))
    {
      Serial.println("Exit the mode");
      boolean L = 1;
      while(L == 1)
      {
        display.clearDisplay();
        display.display();
        display.setTextSize(1);             // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        display.setCursor(0,0);             // Start at top-left corner
        display.println(F("LED"));
        display.println(F("^  Door and window"));
        display.println(F("Tunnel light"));
        display.println(F("Music"));
        display.println(F("Follow Car"));
        display.println(F("Avoid Car"));
        display.println(F("Tracking Car"));
        display.println(F("Find car light"));
        display.display();
        L = 0;
      }
      procedure(door_servo, 46);  //close the door
      procedure(window_servo, 110);  // close the window
      delay(200);
      servo_flag = 0;
      mode_count = 1;
      OLED_servo = 1;
    }
    else
    {
      if(btn2_val == 0)
      {
        delay(10);
        boolean ser2 = 1;
        while(ser2 == 1)
        {
          boolean btn2_val = digitalRead(btn_pin2);
          if(btn2_val == 1)
          {
            count2++;
            Serial.print("button2 = ");
            Serial.println(count2);
            ser2 = 0;
          }
          boolean val = count2 % 2;
          if(val == 1)
          {
            procedure(door_servo, 140);
            procedure(window_servo, 50);
            delay(200);
          }
          else
          {
            procedure(door_servo, 46);
            procedure(window_servo, 110);
            delay(200);
          }
        }
      }
    }
  }
}

//lamp
int displayPiexls(int num, int redVal, int greenVal, int blueVal)
{
  for(int i=0; i<num; i++) {
    pixels.setPixelColor(i, pixels.Color(redVal, greenVal, blueVal));
  }
}
void Tunnel_light_mode()
{
  boolean tunnel_flag = 1;
  boolean i = 1;
  while(tunnel_flag == 1)
  {
    while(i == 1)
    {
      display.clearDisplay();
      display.display();
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0,0);             // Start at top-left corner
      display.println(F("Tunnel mode"));
      display.println(F("Btn1 turn off tunnel"));
      display.println(F("Btn2 turn on tunnel"));
      display.println(F("touch waterSensor tunnel breath"));
      display.display();
      i = 0;
    }
    pinMode(btn_pin1,INPUT);
    //pinMode(btn_pin2,INPUT);
    pinMode(lightPin,INPUT);
    boolean btn1_val = digitalRead(btn_pin1);
    boolean btn2_val = digitalRead(btn_pin2);
    int light_val_ss = analogRead(lightPin);
    if(light_val_ss < 100)
    {
      for(int bright_val=0;bright_val<256;bright_val++)
      {
        displayPiexls(NUMPIXELS, bright_val, 0, bright_val);
        pixels.show();
        delay(10);
      }
      for(int bright_val=255;bright_val>=0;bright_val--)
      {
        displayPiexls(NUMPIXELS, bright_val, 0, bright_val);
        pixels.show();
        delay(10);
      }
    }
    if(btn1_val == 0)
    {
      displayPiexls(NUMPIXELS, 0, 0, 0);  //close the lamp
      pixels.show();
    }
    if(btn2_val == 0)
    {
      delay(10);
      boolean i=1;
      while(i == 1)
      {
        boolean btn2_val = digitalRead(btn_pin2);
        if(btn2_val == 1)
        {
          tunnel_count++;
          if(tunnel_count >= 6)
          {
            tunnel_count = 0;
          }
          Serial.print("tunnel_count = ");
          Serial.println(tunnel_count);
          i= 0;
        }
        switch(tunnel_count)
        {
          case 0: displayPiexls(NUMPIXELS, 200, 0, 0); pixels.show(); break;
          case 1: displayPiexls(NUMPIXELS, 0, 200, 0); pixels.show(); break;
          case 2: displayPiexls(NUMPIXELS, 0, 0, 200); pixels.show(); break;
          case 3: displayPiexls(NUMPIXELS, 200, 200, 0); pixels.show(); break;
          case 4: displayPiexls(NUMPIXELS, 0, 200, 200); pixels.show(); break;
          case 5: displayPiexls(NUMPIXELS, 200, 200, 200); pixels.show(); break;
          case 6: displayPiexls(NUMPIXELS, 0, 0, 0); pixels.show(); break;
        }
      }
    }
    if((btn1_val == 0) && (btn2_val == 0))
    {
      Serial.println("Exit the mode");
      boolean L = 1;
      while(L == 1)
      {
        display.clearDisplay();
        display.display();
        display.setTextSize(1);             // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        display.setCursor(0,0);             // Start at top-left corner
        display.println(F("LED"));
        display.println(F("Door and window"));
        display.println(F("^  Tunnel light"));
        display.println(F("Music"));
        display.println(F("Follow Car"));
        display.println(F("Avoid Car"));
        display.println(F("Tracking Car"));
        display.println(F("Find car light"));
        display.display();
        L = 0;
      }
      tunnel_flag = 0;
      mode_count = 2;
      OLED_tunnel = 1;
      displayPiexls(NUMPIXELS, 0, 0, 0); 
      pixels.show();
    }
  }
}
void Music_mode()
{
  boolean music_flag = 1;
  boolean i = 1;
  while(music_flag == 1)
  {
    while(i == 1)
    {
      display.clearDisplay();
      display.display();
      display.setTextSize(2);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0,0);             // Start at top-left corner
      display.println(F("music mode"));
      display.setTextSize(1);
      display.println(F("Play Happy Birthday"));
      display.display();
      i = 0;
    }
    pinMode(btn_pin1,INPUT);
    pinMode(btn_pin2,INPUT);
    boolean btn1_val = digitalRead(btn_pin1);
    boolean btn2_val = digitalRead(btn_pin2);
    if((btn1_val == 0) && (btn2_val == 0))
    {
      Serial.println("Exit the mode");
      boolean L = 1;
      while(L == 1)
      {
        display.clearDisplay();
        display.display();
        display.setTextSize(1);             // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        display.setCursor(0,0);             // Start at top-left corner
        display.println(F("LED"));
        display.println(F("Door and window"));
        display.println(F("Tunnel light"));
        display.println(F("^  Music"));
        display.println(F("Follow Car"));
        display.println(F("Avoid Car"));
        display.println(F("Tracking Car"));
        display.println(F("Find car light"));
        display.display();
        L = 0;
      }
      music_flag = 0;
      mode_count = 3;
      OLED_LED = 1;
      break;
    }
    if(btn2_val == 0)
    {
      pinMode(buzzerPin, OUTPUT);
      play_music(buzzerPin);
    }
  }
  
}

//ultrasonic ranging
float checkdistance() {
  char i = 0;
  ds[0]=0;
  ds[1]=0;
  ds[2]=0;
  Wire.beginTransmission(0x57);
  Wire.write(1);
  Wire.endTransmission();            
  delay(120);
  Wire.requestFrom(0x57,3);
  while (Wire.available())
  {
   ds[i++] = Wire.read();
  }          
  distance=(ds[0]*65536+ds[1]*256+ds[2])/10000;
  Serial.print("distance : "); 
  if ((1<=distance)&&(600>=distance))
  {
  Serial.print(distance);
   Serial.print(" CM ");  
  }
  else 
  {
  Serial.print(" - - - - ");
  }
  Serial.println();    
  delay(10);
  return distance;
}
void Follow_car_mode()
{
  boolean follow_flag = 1;
  boolean i = 1;
  while(follow_flag == 1)
  {
    while(i == 1)
    {
      display.clearDisplay();
      display.display();
      display.setTextSize(2);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0,0);             // Start at top-left corner
      display.println(F("follow car mode"));
      display.setTextSize(1);
      display.println(F("Follow the object in front"));  
      display.display();
      i = 0;
      procedure(head_servo, 90);
      delay(300);
    }

    distance = checkdistance(); //Get the measured distance value
    //Serial.print("distance = ");
    //Serial.println(distance);
    //delay(100);
    if(distance < 8)
    {
      follow_back();
    }
    else if((distance >= 8) && (distance <= 16))
    {
      car_stop();
    }
    else if((distance > 16) && (distance <= 25))
    {
      follow_forward();
    }
    else
    {
      car_stop();
    }
    pinMode(btn_pin1,INPUT);
    pinMode(btn_pin2,INPUT);
    boolean btn1_val = digitalRead(btn_pin1);
    boolean btn2_val = digitalRead(btn_pin2);
    if((btn1_val == 0) && (btn2_val == 0))
    {
      Serial.println("Exit the mode");
      boolean L = 1;
      while(L == 1)
      {
        display.clearDisplay();
        display.display();
        display.setTextSize(1);             // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        display.setCursor(0,0);             // Start at top-left corner
        display.println(F("LED"));
        display.println(F("Door and window"));
        display.println(F("Tunnel light"));
        display.println(F("Music"));
        display.println(F("^  Follow Car"));
        display.println(F("Avoid Car"));
        display.println(F("Tracking Car"));
        display.println(F("Find car light"));
        display.display();
        L = 0;
      }
      follow_flag = 0;
      mode_count = 4;
      OLED_LED = 1;
      car_stop();
    }
  }
}
void Avoid_car_mode()
{
  boolean avoid_flag = 1;
  boolean i = 1;
  while(avoid_flag == 1)
  {
    while(i == 1)
    {
      display.clearDisplay();
      display.display();
      display.setTextSize(2);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0,0);             // Start at top-left corner
      display.println(F("avoid mode"));
      display.setTextSize(1);
      display.println(F("Automatically avoid obstacles in front"));
      display.display();
      i = 0;
    }
    
    distance1 = checkdistance(); //Get the measured distance value
    Serial.print("distance1 = ");
    Serial.println(distance1);
    if((distance1 > 15) || (distance1 == 0))
    {
      follow_forward();
    }
    else
    {
      car_stop();
      pinMode(head_servo, OUTPUT);
      procedure(head_servo, 170);
      delay(300);
      distance2 = checkdistance(); //Get the measured distance value
      delay(100);
      procedure(head_servo, 10);
      delay(500);
      distance3 = checkdistance(); //Get the measured distance value
      delay(100);
      if(distance2 > distance3)
      {
        car_left();  //turn left
        procedure(head_servo,90);
        delay(300);
      }
      else
      {
        car_right();
        procedure(head_servo, 90);
        delay(300);
      }
    }
    pinMode(btn_pin1,INPUT);
    pinMode(btn_pin2,INPUT);
    boolean btn1_val = digitalRead(btn_pin1);
    boolean btn2_val = digitalRead(btn_pin2);
    if((btn1_val == 0) && (btn2_val == 0))
    {
      Serial.println("Exit the mode");
      boolean L = 1;
      while(L == 1)
      {
        display.clearDisplay();
        display.display();
        display.setTextSize(1);             // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        display.setCursor(0,0);             // Start at top-left corner
        display.println(F("LED"));
        display.println(F("Door and window"));
        display.println(F("Tunnel light"));
        display.println(F("Music"));
        display.println(F("Follow Car"));
        display.println(F("^  Avoid Car"));
        display.println(F("Tracking Car"));
        display.println(F("Find car light"));
        display.display();
        L = 0;
      }
      car_stop();
      avoid_flag = 0;
      mode_count = 5;
      OLED_LED = 1;
    }
  }
}
void Tracking_car_mode()
{
  boolean tracking_flag = 1;
  boolean i = 1;
  while(tracking_flag == 1)
  {
    while(i == 1)
    {
      display.clearDisplay();
      display.display();
      display.setTextSize(2);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0,0);             // Start at top-left corner
      display.println(F("Tracking mode"));
      display.setTextSize(1);
      display.println(F("Btn1 turn off the LED"));
      display.println(F("Btn2 turn on the LED"));
      display.display();
      i = 0;
    }

    L_tval = digitalRead(wayPin1);
    R_tval = digitalRead(wayPin2);
    Serial.print(L_tval);
    Serial.print("  ");
    Serial.println(R_tval);
    delay(100);
    if((L_tval == 1) && (R_tval == 1))
    {
      digitalWrite(INA, HIGH);
      analogWrite(ENA, 50); 
      digitalWrite(INB, HIGH);
      analogWrite(ENB, 50);
    }
    else if((L_tval == 1) && (R_tval == 0))
    {
      digitalWrite(INA, LOW);
      analogWrite(ENA, 70);
      digitalWrite(INB, HIGH);
      analogWrite(ENB, 80);
      
    }
    else if((L_tval == 0) && (R_tval == 1))
    {
      digitalWrite(INA, HIGH);
      analogWrite(ENA, 80);
      digitalWrite(INB, LOW);
      analogWrite(ENB, 70);
    }
    else if((L_tval == 0) && (R_tval == 0))
    {
      car_stop();
    }
    pinMode(btn_pin1,INPUT);
    pinMode(btn_pin2,INPUT);
    boolean btn1_val = digitalRead(btn_pin1);
    boolean btn2_val = digitalRead(btn_pin2);

    if((btn1_val == 0) && (btn2_val == 0))
    {
      Serial.println("Exit the mode");
      boolean L = 1;
      while(L == 1)
      {
        display.clearDisplay();
        display.display();
        display.setTextSize(1);             // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        display.setCursor(0,0);             // Start at top-left corner
        display.println(F("LED"));
        display.println(F("Door and window"));
        display.println(F("Tunnel light"));
        display.println(F("Music"));
        display.println(F("Follow Car"));
        display.println(F("Avoid Car"));
        display.println(F("^  Tracking Car"));
        display.println(F("Find car light"));
        display.display();
        L = 0;
      }
      tracking_flag = 0;
      mode_count = 6;
      OLED_LED = 1;
    }
  }
}
void Find_car_light_mode()
{
  boolean findLight_flag = 1;
  boolean i = 1;
  int L_lightVal = 0;
  int R_lightVal = 0;
  while(findLight_flag == 1)
  {
    while(i == 1)
    {
      display.clearDisplay();
      display.display();
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(SSD1306_WHITE);        // Draw white text
      display.setCursor(0,0);             // Start at top-left corner
      display.println(F("Find car light mode"));
      display.println(F(" "));
      display.println(F("Go toward the light"));
      display.display();
      i = 0;
    }
    pinMode(L_light, INPUT);
    pinMode(R_light, INPUT);
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
      car_right();
    }
    else if((L_lightVal < 800) && (R_lightVal >= 800))
    {
      
      car_left();
    }
    else if((L_lightVal < 800) && (R_lightVal < 800))
    {
      car_stop();
    }
    pinMode(btn_pin1,INPUT);
    pinMode(btn_pin2,INPUT);
    boolean btn1_val = digitalRead(btn_pin1);
    boolean btn2_val = digitalRead(btn_pin2);

    if((btn1_val == 0) && (btn2_val == 0))
    {
      Serial.println("Exit the mode");
      boolean L = 1;
      while(L == 1)
      {
        display.clearDisplay();
        display.display();
        display.setTextSize(1);             // Normal 1:1 pixel scale
        display.setTextColor(SSD1306_WHITE);        // Draw white text
        display.setCursor(0,0);             // Start at top-left corner
        display.println(F("LED"));
        display.println(F("Door and window"));
        display.println(F("Tunnel light"));
        display.println(F("Music"));
        display.println(F("Follow Car"));
        display.println(F("Avoid Car"));
        display.println(F("Tracking Car"));
        display.println(F("^  Find car light"));
        display.display();
        L = 0;
      }
      car_stop();
      findLight_flag = 0;
      mode_count = 7;
      OLED_LED = 1;
    }
  }
}

void select_LED_mode()
{
  while(OLED_LED == 1)
  {
    display.clearDisplay();
    display.display();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("^  LED"));
    display.println(F("Door and window"));
    display.println(F("Tunnel light"));
    display.println(F("Music"));
    display.println(F("Follow Car"));
    display.println(F("Avoid Car"));
    display.println(F("Tracking Car"));
    display.println(F("Find car light"));
    display.display();
    OLED_LED = 0;
    OLED_servo = 1;
    OLED_tunnel = 1;
    OLED_music = 1;
    OLED_follow_car = 1;
    OLED_avoid_car = 1;
    OLED_tracking_car = 1;
    OLED_find_car_light = 1;
  }
}
void select_Door_and_window_mode()
{
  while(OLED_servo == 1)
  {
    display.clearDisplay();
    display.display();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("LED"));
    display.println(F("^  Door and window"));
    display.println(F("Tunnel light"));
    display.println(F("Music"));
    display.println(F("Follow Car"));
    display.println(F("Avoid Car"));
    display.println(F("Tracking Car"));
    display.println(F("Find car light"));
    display.display();
    
    OLED_LED = 1;
    OLED_servo = 0;
    OLED_tunnel = 1;
    OLED_music = 1;
    OLED_follow_car = 1;
    OLED_avoid_car = 1;
    OLED_tracking_car = 1;
    OLED_find_car_light = 1;
  }
}
void select_Tunnel_light_mode()
{
  while(OLED_tunnel == 1)
  {
    display.clearDisplay();
    display.display();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("LED"));
    display.println(F("Door and window"));
    display.println(F("^  Tunnel light"));
    display.println(F("Music"));
    display.println(F("Follow Car"));
    display.println(F("Avoid Car"));
    display.println(F("Tracking Car"));
    display.println(F("Find car light"));
    display.display();
    
    OLED_LED = 1;
    OLED_servo = 1;
    OLED_tunnel = 0;
    OLED_music = 1;
    OLED_follow_car = 1;
    OLED_avoid_car = 1;
    OLED_tracking_car = 1;
    OLED_find_car_light = 1;
  }
}
void select_Music_mode()
{
  while(OLED_music == 1)
  {
    display.clearDisplay();
    display.display();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("LED"));
    display.println(F("Door and window"));
    display.println(F("Tunnel light"));
    display.println(F("^  Music"));
    display.println(F("Follow Car"));
    display.println(F("Avoid Car"));
    display.println(F("Tracking Car"));
    display.println(F("Find car light"));
    display.display();
    
    OLED_LED = 1;
    OLED_servo = 1;
    OLED_tunnel = 1;
    OLED_music = 0;
    OLED_follow_car = 1;
    OLED_avoid_car = 1;
    OLED_tracking_car = 1;
    OLED_find_car_light = 1;
  }
}
void select_Follow_car_mode()
{
  while(OLED_follow_car == 1)
  {
    display.clearDisplay();
    display.display();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("LED"));
    display.println(F("Door and window"));
    display.println(F("Tunnel light"));
    display.println(F("Music"));
    display.println(F("^  Follow Car"));
    display.println(F("Avoid Car"));
    display.println(F("Tracking Car"));
    display.println(F("Find car light"));
    display.display();
    
    OLED_LED = 1;
    OLED_servo = 1;
    OLED_tunnel = 1;
    OLED_music = 1;
    OLED_follow_car = 0;
    OLED_avoid_car = 1;
    OLED_tracking_car = 1;
    OLED_find_car_light = 1;
  }
}
void select_Avoid_car_mode()
{
  while(OLED_avoid_car == 1)
  {
    display.clearDisplay();
    display.display();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("LED"));
    display.println(F("Door and window"));
    display.println(F("Tunnel light"));
    display.println(F("Music"));
    display.println(F("Follow Car"));
    display.println(F("^  Avoid Car"));
    display.println(F("Tracking Car"));
    display.println(F("Find car light"));
    display.display();
    
    OLED_LED = 1;
    OLED_servo = 1;
    OLED_tunnel = 1;
    OLED_music = 1;
    OLED_follow_car = 1;
    OLED_avoid_car = 0;
    OLED_tracking_car = 1;
    OLED_find_car_light = 1;
  }
}
void select_Tracking_car_mode()
{
  while(OLED_tracking_car == 1)
  {
    display.clearDisplay();
    display.display();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("LED"));
    display.println(F("Door and window"));
    display.println(F("Tunnel light"));
    display.println(F("Music"));
    display.println(F("Follow Car"));
    display.println(F("Avoid Car"));
    display.println(F("^  Tracking Car"));
    display.println(F("Find car light"));
    display.display();
    
    OLED_LED = 1;
    OLED_servo = 1;
    OLED_tunnel = 1;
    OLED_music = 1;
    OLED_follow_car = 1;
    OLED_avoid_car = 1;
    OLED_tracking_car = 0;
    OLED_find_car_light = 1;
  }
}
void select_Find_car_light_mode()
{
  while(OLED_find_car_light == 1)
  {
    display.clearDisplay();
    display.display();
    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.println(F("LED"));
    display.println(F("Door and window"));
    display.println(F("Tunnel light"));
    display.println(F("Music"));
    display.println(F("Follow Car"));
    display.println(F("Avoid Car"));
    display.println(F("Tracking Car"));
    display.println(F("^  Find car light"));
    display.display();
    
    OLED_LED = 1;
    OLED_servo = 1;
    OLED_tunnel = 1;
    OLED_music = 1;
    OLED_follow_car = 1;
    OLED_avoid_car = 1;
    OLED_tracking_car = 1;
    OLED_find_car_light = 0;
  }
}

//Speed of follow
void follow_forward()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 100);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 100);
}
void follow_back()
{
  digitalWrite(INA, LOW);
  analogWrite(ENA, 100);
  digitalWrite(INB, LOW); 
  analogWrite(ENB, 100); 
}

void car_forward()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 100); 
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 100);
}
void car_back()
{
  digitalWrite(INA, LOW);
  analogWrite(ENA, 100);
  digitalWrite(INB, LOW);
  analogWrite(ENB, 100);
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
void car_stop()
{
  digitalWrite(INA, HIGH);
  analogWrite(ENA, 0);
  digitalWrite(INB, HIGH);
  analogWrite(ENB, 0);
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
