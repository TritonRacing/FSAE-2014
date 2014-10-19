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
#include <SD.h>
#include <SPI.h>

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


//instantiate accel object
Adafruit_MMA8451 mma = Adafruit_MMA8451();

//Analog pin for hall effect magnetic sensor
const int HALL_EFFECT_PIN = A2;

//Analog pin for steering angle
const int STEER_POT_PIN = A3;

//sensor holding values
int hallEffectReading = 0;
int steerPotReading = 0;

//velocity variables
//WHEEL_DISTANCE based off of distance between magnets on wheel
const int WHEEL_DISTANCE = 12;
int time = 0;
int time_init = 0;
double velocity = 0;

//slip angle variables
//////////////////////

File dataFile;
const int chipSelect = 10;
/*
 * Name: setup()
 * Purpose: instantiate serial port, check for proper
 * instantiation of sensors, display information
*/
void setup(void) 
{
  Serial.begin(9600);
  
  Serial.println("FSAE TRITON RACING DAQ 2014");
  Serial.println("Acceleration, Velocity, Slip Angle");
  
  //check sensors for correct instantiation
  if (! mma.begin()) 
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
  //gather readings from hall effect sensor to check calibration
  hallEffectReading = analogRead(HALL_EFFECT_PIN);
  if ((hallEffectReading<1000) || (hallEffectReading>200))
  {
    Serial.println("Hall Effect Sensor is not calibrated correctly");
  }
  
  Serial.println("MMA8451 found!");
  Serial.println("L3DG20 found!");
  Serial.println("Hall Effect Sensor found!");
  
  //range can take values of 2G, 4G, and 8G
  mma.setRange(MMA8451_RANGE_4_G);
  
  pinMode(SS, OUTPUT);
  if (!SD.begin(chipSelect))
  {
    Serial.println("Card failed to instantiate");
    return;
  }
  
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (! datafile)
  {
    Serial.println("error opening datalog.txt");
    //cannot write data
  }
  
  Serial.println("Card initialized! Outputs written to 'datalog.txt'");
}

/*
 * Name: Velocity
 * Hall effect sensors are 1022 or 1023 when idle.
 * Once a magnet passes, sensor reads around 0 or 1. Check
 * time between magnet passes, use d=r/t formula to calculate
 * speed.
*/

/*
 * Name: Acceleration
 *
 *
 *
*/

/*
 * Name: Slip Angle
 *
 *
 *
 *
*/

/*
 * Name: Write to SD
 *
 *
 *
*/

void loop()
{
  /******************************************
  /* VELOCITY
  ******************************************/
  hallEffectReading = analogRead(HALL_EFFECT_PIN);
  if (hallEffectReading < 200)
  {
   time_init = millis();
   //wait for magnet to reach sensor, but wait to check count isn't taken
   //from same magnet
   while (hallEffectReading < 200)
   {
     hallEffectReading = analogRead(HALL_EFFECT_PIN);
   }
   while (hallEffectReading > 1000)
   {
     hallEffectReading = analogRead(HALL_EFFECT_PIN);
   }
   time = millis();
   //rate = distance/time
   velocity = WHEEL_DISTANCE/(time-time_init);
  }
  
  /******************************************
  /* ACCELERATION
  ******************************************/  
  // Read the 'raw' data in 14-bit counts
  mma.read();
  /* Get a new sensor event */ 
  sensors_event_t event; 
  mma.getEvent(&event);

  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("X: \t"); Serial.print(event.acceleration.x); Serial.print("\t");
  Serial.print("Y: \t"); Serial.print(event.acceleration.y); Serial.print("\t");
  Serial.print("Z: \t"); Serial.print(event.acceleration.z); Serial.print("\t");
  Serial.println("m/s^2 ");
  Serial.println();
  
  //assign accleration to variables
  int XAccel;
  if(event.acceleration.x <= 0) 
  {
    XAccel = (event.acceleration.x + .005) * 100;
  } 
  else
  {
    XAccel = (event.acceleration.x - .005) * 100;
  }
   
  int YAccel;
  if(event.acceleration.y <= 0) 
  {
    YAccel = (event.acceleration.y + .005) * 100;
  } 
  else
  {
    YAccel = (event.acceleration.y - .005) * 100;
  }
  
  int ZAccel;
  if(event.acceleration.x >= 0)
  {
    ZAccel = (event.acceleration.z + .005) * 100;
  } 
  else
  {
    ZAccel = (event.acceleration.z - .005) * 100;
  }
  
  /******************************************
  /* SLIP ANGLE
  ******************************************/
  gyro.read();
  steerPotReading = analogRead(STEER_POT_PIN);
  //add vars, convert to int (look at accel)
  Serial.print("X: "); Serial.print((int)gyro.data.x);   Serial.print(" ");
  Serial.print("Y: "); Serial.print((int)gyro.data.y);   Serial.print(" ");
  Serial.print("Z: "); Serial.print((int)gyro.data.z); Serial.print(" ");
  Serial.println("degrees/second");

  //some calculation for slip angle....
  
  /******************************************
  /* WRITE TO SD CARD
  ******************************************/  
  DataString = String(XAccel) + " \t" + String(YAccel) + " /t" + String(ZAccel) +
                              " \t" + velocity;
                              
  if (dataFile)
  {
    dataFile.println(DataString);
    dataFile.close();
  }
  else {
    Serial.println("SD Card not working");
  }
    
  //Delay based off of max RPM (2000RPM at 80mph)
  delay(33);
}
