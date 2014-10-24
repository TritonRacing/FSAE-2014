/**************************************************************************/
/*!
    @file     FSAEDAQ20124.ino
    @author   Triton Racing Research and Development 2014
    
    Uses MMA8451 Accelerometer, L3GD20 Gyroscope, Hall Effect analog sensor,
    and a potentiometer to gather data from cars. Data gathered: accleration
    (MMA8451), velocity (Hall Effect), slip angle (L3DG20 and potentiometer).
    
    Data is gathered in one string, exported to Excel document, formulas
    run to turn values into meaningful data.
    
    NOTE: Serial outputs and instantiations are not required and used
    for debugging.
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

//Digital pin for hall effect magnetic sensor
const int HALL_EFFECT_PIN = 3;

//Analog pin for steering angle
const int STEER_POT_PIN = A3;

//sensor holding values
int hallEffectReading = 0;
int steerPotReading = 0;

//velocity variables
//WHEEL_DISTANCE based off of distance between magnets on wheel
const double WHEEL_DISTANCE = 14.25;
int time = 0;
int time_init = 0;
double velocity = 0;

//slip angle variables
//////////////////////

//SD card variables
File dataFile;
String DataString;
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
  //if (!gyro.begin(gyro.L3DS20_RANGE_500DPS))
  //if (!gyro.begin(gyro.L3DS20_RANGE_2000DPS))
  {
    Serial.println("Unable to initialize the L3GD20 Gyro. Check your wiring!");
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
  if (! dataFile)
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
 * MMA8451 Accelerometer returns values based on code
 * written in libraries. Values are returned as doubles
 * and are convertd to ints to be concatenated into strings.
*/

/*
 * Name: Slip Angle
 * Potentiometer connected to detect steering angle
 * is coupled with data from the L3DG20 Gyroscope
 * to calculate slip angle of vehicle. Calculations are
 * done in post-processing Excell formulas.
*/

/*
 * Name: Write to SD
 * Interact with SD card libraries to print out a 
 * concatenated string with data types. String concatenation
 * is timed stamped with time of Arduino startup.
*/

void loop()
{
  /******************************************
  /* VELOCITY
  ******************************************/
  hallEffectReading = digitalRead(HALL_EFFECT_PIN);
  if (hallEffectReading == 1)
  {
   time_init = millis();
   //wait for magnet to reach sensor, but wait to check count isn't taken
   //from same magnet twice
   while (hallEffectReading == 1)
   {
     hallEffectReading = digitalRead(HALL_EFFECT_PIN);
   }
   while (hallEffectReading == 0)
   {
     hallEffectReading = digitalRead(HALL_EFFECT_PIN);
   }
   //second magnet has passed, take timestamp
   time = millis();
   //rate = distance/time
   velocity = WHEEL_DISTANCE/(time-time_init);
  }
  
  //integer required to print to datastream
  int velocity_rounded = int(velocity);
  
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
  
  //assign accleration to variables, convert doubles to ints
  int XAccel;
  if(event.acceleration.x <= 0) 
  {
    XAccel = (event.acceleration.x + .0005) * 1000;
  } 
  else
  {
    XAccel = (event.acceleration.x - .0005) * 1000;
  }
   
  int YAccel;
  if(event.acceleration.y <= 0) 
  {
    YAccel = (event.acceleration.y + .0005) * 1000;
  } 
  else
  {
    YAccel = (event.acceleration.y - .0005) * 1000;
  }
  
  int ZAccel;
  if(event.acceleration.x >= 0)
  {
    ZAccel = (event.acceleration.z + .0005) * 1000;
  } 
  else
  {
    ZAccel = (event.acceleration.z - .0005) * 1000;
  }
  
  /******************************************
  /* SLIP ANGLE
  ******************************************/
  gyro.read();
  //steerPotReading = analogRead(STEER_POT_PIN);
  //add vars, convert to int (look at accel)
  Serial.print("X: "); Serial.print((int)gyro.data.x);   Serial.print(" ");
  Serial.print("Y: "); Serial.print((int)gyro.data.y);   Serial.print(" ");
  Serial.print("Z: "); Serial.print((int)gyro.data.z); Serial.print(" ");
  Serial.println("degrees/second");
  
  //Assign variables to hold 
  int XAxisRotation = (int)gyro.data.x;
  int YAxisRotation = (int)gyro.data.y;
  int ZAxisRotation = (int)gyro.data.z;
  
  
  
  //some calculation for slip angle....
  
  /******************************************
  /* WRITE TO SD CARD
  ******************************************/  
  //concatenate all data into one string to print to SD
  DataString = String("Acceleration ") + String(XAccel) + " \t" + String(YAccel) + " /t" + String(ZAccel) +
                              " \t" + String("Velocity ") + velocity + " \t" + String("Gyro ") +
                              String(XAxisRotation) + " \t" + String(YAxisRotation) + " \t" + 
                              String(ZAxisRotation) + " \t" + "Time: " + millis()/*String("Steering Angle ") + String(steerPotReading) + 
                              " \t"*/;
                              
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
