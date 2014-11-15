/**************************************************************************/
/*!
    @file     Adafruit_MMA8451.h
    @author   K. Townsend (Adafruit Industries)
    @license  BSD (see license.txt)

    This is an example for the Adafruit MMA8451 Accel breakout board
    ----> https://www.adafruit.com/products/2019

    Adafruit invests time and resources providing this open source code,
    please support Adafruit and open-source hardware by purchasing
    products from Adafruit!

    @section  HISTORY

    v1.0  - First release
*/
/**************************************************************************/

#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20.h>

// Comment this next line to use SPI
//#define USE_I2C

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

const int GYRO_OFFSET_X = -6.556;
const int GYRO_OFFSET_Y = 8.544;
const int GYRO_OFFSET_Z = 0.105;

//instantiate accel object
Adafruit_MMA8451 mma = Adafruit_MMA8451();

void setup(void) 
{
  Serial.begin(9600);
  
  Serial.println("Accelerometer and Gyro Data");
  
  //check both sensors for correct instantiation
  while (! mma.begin()) 
  {
    Serial.println("Couldnt start MMA8451 Accel!");
  }
  //comment/uncomment for different senitivities (DPS = degrees/second)
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS))
  //while (!gyro.begin(gyro.L3DS20_RANGE_500DPS))
  //while (!gyro.begin(gyro.L3DS20_RANGE_2000DPS))
  {
    Serial.println("Unable to initialize the L3GD20 Gyro. Check your wiring!");
  }
  Serial.println("MMA8451 found!");
  Serial.println("L3DG20 found!");
  
  mma.setRange(MMA8451_RANGE_8_G);
  
  Serial.print("Range = "); Serial.print(2 << mma.getRange());  
  Serial.println("G");
  
  Serial.print("X: ");Serial.print("Y: ");Serial.print("Z: ");Serial.print(" ");
  
  Serial.print("X: ");Serial.print("Y: ");Serial.print("Z: ");
  Serial.println("");
}


void loop()
{
  // Read the 'raw' data in 14-bit counts
  mma.read();
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);
  
  gyro.read();

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print(event.acceleration.x);Serial.print("\t");
  Serial.print(event.acceleration.y);Serial.print("\t");
  Serial.print(event.acceleration.z);Serial.print("\t");
  
  Serial.print(gyro.data.x+GYRO_OFFSET_X);Serial.print("\t");
  Serial.print(gyro.data.y+GYRO_OFFSET_Y);Serial.print("\t");
  Serial.print(gyro.data.z+GYRO_OFFSET_Z);Serial.print("\t");
  Serial.println("");
  
  delay(200);
}
