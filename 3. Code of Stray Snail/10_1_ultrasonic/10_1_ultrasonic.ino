#include <Wire.h> 
float distance = 0;  //Decimal distance value
float ds[3];  //Create an array of float numbers to store the values read

void setup()
{
  Serial.begin(9600);  //Define the baud rate of serial port to 9600, which is the factory default value
  Wire.begin();
  Serial.println("Ultrasonic-IIC start ranging："); 
}

void loop() 
{
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
   delay(100);                                  //Adjust the measure cycle
}
