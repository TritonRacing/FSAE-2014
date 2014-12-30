// Test code for Adafruit GPS modules using MTK3329/MTK3339 driver
//
// This code just echos whatever is coming from the GPS unit to the
// serial monitor, handy for debugging!
//
// Tested and works great with the Adafruit Ultimate GPS module
// using MTK33x9 chipset
//    ------> http://www.adafruit.com/products/746
// Pick one up today at the Adafruit electronics shop 
// and help support open source hardware & software! -ada

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(8, 7);
Adafruit_GPS GPS(&mySerial);
boolean serial_echo = true;


void initGPS()
{
  GPS.begin(9600);
  
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);
  
  GPS.sendCommand(PGCMD_ANTENNA);
}

void setup()
{
  Serial.begin(115200);
  initGPS();
}

void initInterrupt()
{
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

SIGNAL(TIMER0_COMPA_vect)
{
  char c = GPS.read();
}

boolean GPS_Check()
{
  if (GPS.newNMEAreceived())
  {
    if (!GPS.parse(GPS.lastNMEA()))
    {
      return false;
    }
    return true;
  }
}

void loop()
{
  float latitude;
  float longitude;
  float gps_alt;
  /* Check for GPS data and put it into more readable variables*/
  boolean GPS_data = GPS_Check();
  if(GPS_data){
    if(GPS.fix){
      latitude = GPS.latitudeDegrees;
      longitude= GPS.longitudeDegrees;
      gps_alt = GPS.altitude;
    }
    else {
      Serial.println("No GPS fix");
    }
  }

  /* Display values though the Serial Port for now */
  if(serial_echo){
    if(GPS_data){
      Serial.print("\nTime: ");
      Serial.print(GPS.hour, DEC); Serial.print(':');
      Serial.print(GPS.minute, DEC); Serial.print(':');
      Serial.print(GPS.seconds, DEC); Serial.print('.');
      Serial.println(GPS.milliseconds);
      Serial.print("Date: ");
      Serial.print(GPS.day, DEC); Serial.print("/");
      Serial.print(GPS.month, DEC); Serial.print("/20");
      Serial.println(GPS.year, DEC);
      Serial.print("Fix: "); Serial.print((int)GPS.fix);
      Serial.print(" quality: "); Serial.println((int)GPS.fixquality);
      if(GPS.fix) {
        Serial.print("Satellites: "); Serial.println((int)GPS.satellites);
        Serial.print("Location: ");
        Serial.print(latitude,6); Serial.print(", ");
        Serial.println(longitude,6); 
        /* Serial.print("GPS Alititude: "); Serial.print(gps_alt,6); 
        Serial.println(" m"); */
      } 
    }
  }
  else{
    Serial.println("\nBetween GPS Updates, only printing new sensor data");
  } 
  delay(500);
}
