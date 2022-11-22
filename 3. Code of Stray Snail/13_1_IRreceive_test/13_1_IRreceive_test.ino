#include <IRremote.h>
#define IR_RECEIVE_PIN  3  //Define the pin of infrared receiving module 
IRrecv IrReceiver(IR_RECEIVE_PIN);
decode_results results;
int ir_val;

void setup() {
  Serial.begin(115200);  //Define the baud rate to 115200
  Serial.println("Enabling IRin");
  IrReceiver.enableIRIn();  // Start the receiver
  IrReceiver.blink13(true); // Enable feedback LED

  Serial.print(F("Ready to receive IR signals at pin "));
  Serial.println(IR_RECEIVE_PIN);
}

void loop() {
  if (IrReceiver.decode(&results)) 
  {
    ir_val = results.value;
    //IrReceiver.printResultShort(&Serial);
    Serial.println(ir_val,HEX);

    IrReceiver.resume(); // Receive the next value
  }
  delay(100);
}
