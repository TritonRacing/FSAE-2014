int val = 0;
int val1 = 0;
void setup() {
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
  //int val = 0;
  Serial.println("CLEARDATA");
}

void loop() {
  Serial.print("DATA,TIME,");
  Serial.println(val1);
  val1 = random(0, 100);
  delay(3000);
}
