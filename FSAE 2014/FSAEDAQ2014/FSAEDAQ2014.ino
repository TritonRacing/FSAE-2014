/**************************************************************************
 * File name:     FSAEDAQ20124.ino
 * Written By:    Aman Azad (2014-2015 FSAE DAQ Team)
 * Description:   Uses MMA8451 Accelerometer, L3GD20 Gyroscope, Hall
                  Effect analog sensor, and a potentiometer to gather data
                  from cars. Data gathered: accleration (MMA8451), 
                  velocity (Hall Effect), slip angle (L3DG20 and potentiometer).
 
                  Data is gathered in one string, exported to Excel document,
                  formulas run to turn values into meaningful data.
                   
                  Runs on the Mega
                  
                  NOTE: Serial outputs and instantiations are not required 
                  and used for debugging.
**************************************************************************/

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


//delay time in hertz
//Set Baud Rate
const int hertz = 20;
const int DELAY_TIME = 1000/hertz;

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


boolean same_magnet = false;
/*
 * Name: setup()
 * Purpose: instantiate serial port, check for proper
 * instantiation of sensors, display information
 */
void setup(void) 
{
  //Serial.begin(9600);

  //Serial.println("FSAE TRITON RACING DAQ 2014");
  //Serial.println("Acceleration, Velocity, Slip Angle");

  //check sensors for correct instantiation
  if (! mma.begin()) 
  {
    //Serial.println("Couldnt start MMA8451 Accel!");
  }
  //comment/uncomment for different senitivities (DPS = degrees/second)
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS))
    //if (!gyro.begin(gyro.L3DS20_RANGE_500DPS))
    //if (!gyro.begin(gyro.L3DS20_RANGE_2000DPS))
  {
    //Serial.println("Unable to initialize the L3GD20 Gyro. Check your wiring!");
  }
  else
  {
    //Serial.println("MMA8451 found!");
    //Serial.println("L3DG20 found!");
    //Serial.println("Hall Effect Sensor found!");  
  }
  //range can take values of 2G, 4G, and 8G
  mma.setRange(MMA8451_RANGE_8_G);

  pinMode(SS, OUTPUT);
  if (!SD.begin(10,11, 12, 13))
  {
    //Serial.println("Card failed to instantiate");
    return;
  }

  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (! dataFile)
  {
    //Serial.println("error opening datalog.txt");
    //cannot write data
  }

  DataString = String("Acceleration (g's) | Velocity (ft/s)| Gyroscope (deg/s)");
  dataFile.println(DataString);
  dataFile.flush();
  
  DataString = String("X"+"\t"+"Y"+"\t"+"Z"+"\t"+"FT/S"+"\t"+"X"+"\t"+"Y"+"\t"+"Z"+"\t"+"ms");
  dataFile.println(DataString);
  dataFile.flush();
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

  if ((hallEffectReading == 1) && (!same_magnet)
  {
    //take initial time stamp
    time_init = millis();
    same_magnet = true;
    
    if (hallEffectReading == 0)
    {
      same_magnet = false;
    }
    else if (same_magnet)
    {
      same_magnet = true;
    }
    
    if (!same_magnet)
    {
      if (hallEffectReading == 1)
      {
        time = millis();
        
        velocity = WHEEL_DISTANE/(time-time_init);
        
        same_magnet = false;
      }
    }
    
  }

  /******************************************
  /* ACCELERATION
   ******************************************/
  // Read the 'raw' data in 14-bit counts
  mma.read();
  /* Get a new sensor event */
  sensors_event_t event; 
  mma.getEvent(&event);

  /******************************************
  /* SLIP ANGLE
   ******************************************/
  gyro.read();

  //some calculation for slip angle....

  /******************************************
  /* WRITE TO SD CARD
   ******************************************/
  //concatenate all data into one string to print to SD
  DataString = event.acceleration.x + "\t" + event.acceleration.y + "\t" + event.acceleration.z + "\t" +
               "\t" + velocity + "\t" + gyro.data.x + "\t" + gyro.data.y + "\t" + gyro.data.z + "\t" +
               "\t" + millis();
               
  dataFile.println(DataString);
  dataFile.flush(); //save after every line

  //Delay based off of max RPM (2000RPM at 80mph)
  delay(DELAY_TIME);
}

