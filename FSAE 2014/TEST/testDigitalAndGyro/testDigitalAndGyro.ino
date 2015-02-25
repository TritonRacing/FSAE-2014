/*
  Analog Input
 Demonstrates analog input by reading an analog sensor on analog pin 0 and
 turning on and off a light emitting diode(LED)  connected to digital pin 13. 
 The amount of time the LED will be on and off depends on
 the value obtained by analogRead(). 
 
 The circuit:
 * Potentiometer attached to analog input 0
 * center pin of the potentiometer to the analog pin
 * one side pin (either one) to ground
 * the other side pin to +5V
 * LED anode (long leg) attached to digital output 13
 * LED cathode (short leg) attached to ground
 
 * Note: because most Arduinos have a built-in LED attached 
 to pin 13 on the board, the LED is optional.
 
 
 Created by David Cuartielles
 modified 30 Aug 2011
 By Tom Igoe
 
 This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/AnalogInput
 
 */
 
 #include <Wire.h>
 #include <Adafruit_L3GD20.h>
 
 
 //wired on 4, 5, 6, 7 on digital pins on Arduino mega
 #ifdef USE_I2C
  // The default constructor uses I2C
  Adafruit_L3GD20 gyro;
#else
  // To use SPI, you have to define the pins
  #define GYRO_CS 4 // labeled CS
  #define GYRO_DO 5 // labeled SA0
  #define GYRO_DI 6  // labeled SDA
  #define GYRO_CLK 7 // labeled SCL
  Adafruit_L3GD20 gyro(GYRO_CS, GYRO_DO, GYRO_DI, GYRO_CLK);
#endif
 
int sensorPin = 3;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor

void setup() {
  // declare the ledPin as an OUTPUT:
  Serial.begin(9600);
  //pinMode(sensorPin, INPUT);
  
    //comment/uncomment for different senitivities (DPS = degrees/second)
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS))
  //if (!gyro.begin(gyro.L3DS20_RANGE_500DPS))
  //if (!gyro.begin(gyro.L3DS20_RANGE_2000DPS))
  {
    Serial.println("Unable to initialize the L3GD20 Gyro. Check your wiring!");
  }
}

void loop() {
  // read the value from the sensor:
  //Serial.println("Printing data!");
  sensorValue = digitalRead(sensorPin);    
  Serial.println("Sensor Value: ");
  Serial.print(sensorValue);
  
  Serial.println("");
  Serial.println("");
  
    gyro.read();
  //add vars, convert to int (look at accel)
  Serial.print("X: "); Serial.print((int)gyro.data.x);   Serial.print(" ");
  Serial.print("Y: "); Serial.print((int)gyro.data.y);   Serial.print(" ");
  Serial.print("Z: "); Serial.print((int)gyro.data.z); Serial.print(" ");
  Serial.println("degrees/second");
  //Assign variables to hold 
  int XAxisRotation = (int)gyro.data.x;
  int YAxisRotation = (int)gyro.data.y;
  int ZAxisRotation = (int)gyro.data.z;
  
  
  
  delay(100);  
}
