#define wayPin1 9
#define wayPin2 10

boolean val1 = 0;
boolean val2 = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(wayPin1, INPUT);
  pinMode(wayPin2, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  val1 = digitalRead(wayPin1);
  val2 = digitalRead(wayPin2);
  Serial.print("wayPin1 = ");
  Serial.print(val1);
  Serial.print("  ");
  Serial.print("wayPin2 = ");
  Serial.println(val2);
  delay(100);
}
