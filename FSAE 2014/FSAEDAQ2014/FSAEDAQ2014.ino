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

/**
 * PINOUT for Custom Header
 * Left to right, assuming SD port is pointing to the LEFT
 * 0 - Ground for Accelerometer
 * 1 - 3.3V for both Accelerometer and Gyroscope (left empty)
 * 2 - SCL port for A.
 * 3 - SDA port for A.
 * GAP
 * 4 - SAO port for G.
 * 5 - SDA port for G.
 * 6 - CS port for G.
 * 7 - SCL port for G.
 * 8 - VIN port (left empty)
 * 9 - Ground for G.
**/

#include <Wire.h>
#include <Adafruit_MMA8451.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_L3GD20.h>
#include <SD.h>
#include <SPI.h>

// Comment this next line to use SPI
#define USE_I2C

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
const int hertz = 10;
const int DELAY_TIME = 1000/hertz;

//instantiate accel object
Adafruit_MMA8451 mma = Adafruit_MMA8451();

//SD card variables
File dataFile;
String DataString;
const int chipSelect = 10;

double time_init;

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
  
  /*
  char filename[16]; /* string used to hold name of file log data
                        is being sent to */
  /*strcpy(filename, "DATALOG00.TXT");
  for (uint8_t index = 0; index < 100; index++)
  {
    /* update new log file names to avoid overwrites */
   // filename[6] = '0' + index/10;
  //  filename[7] = '0' + index%10;
    
    /* create if does not exist */
   // if (!SD.exists(filename))
  // {
  //    break;
  //  }
  //}
  
  dataFile = SD.open("DATALOG.txt", FILE_WRITE);
  if (! dataFile)
  {
    //Serial.println("error opening datalog.txt");
    //cannot write data
  }

  DataString = String("Acceleration (g's) | Gyroscope (deg/s)");
  dataFile.println(DataString);
  dataFile.flush();
  
  DataString = String("X")+"\t"+"Y"+"\t"+"Z"+"\t"+"X"+"\t"+"Y"+"\t"+"Z"+"\t"+"ms";
  dataFile.println(DataString);
  dataFile.flush();
}

/*
 * Name: Acceleration
 * MMA8451 Accelerometer returns values based on code
 * written in libraries. Values are returned as doubles
 * and are convertd to ints to be concatenated into strings.
 */

/*
 * Name: Write to SD
 * Interact with SD card libraries to print out a 
 * concatenated string with data types. String concatenation
 * is timed stamped with time of Arduino startup.
 */

void loop()
{
  time_init = millis();
  /******************************************
  /* ACCELERATION
   ******************************************/
  // Read the 'raw' data in 14-bit counts
  mma.read();
  /* Get a new sensor event */
  sensors_event_t event; 
  mma.getEvent(&event);

  /******************************************
  /* GYROSCOPE
   ******************************************/
  gyro.read();

  /******************************************
  /* WRITE TO SD CARD
   ******************************************/
  //concatenate all data into one string to print to SD
  DataString = String(event.acceleration.x) + "\t" + event.acceleration.y + "\t" + event.acceleration.z + 
                "\t" + gyro.data.x + "\t" + gyro.data.y + "\t" + gyro.data.z + "\t" + millis();
               
  dataFile.println(DataString);
  dataFile.flush(); //save after every line

  /* Wait for the time */
  while( millis() < (time_init+DELAY_TIME))
  {
    /*Delay...*/
    if (millis() > (time_init+DELAY_TIME))
    {
      break;
    }
  }
}

