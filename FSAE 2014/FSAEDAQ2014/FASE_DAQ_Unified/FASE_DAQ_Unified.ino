char inGPSString [100];
int i = 0;

void setup() {
  Serial1.begin(9600);  //for the GPS
}
void loop()  {
  i = 0;
  while (Serial1.available() > 0)  {
    inGPSString[i] = Serial1.read();
    i++;
  }
    
}
