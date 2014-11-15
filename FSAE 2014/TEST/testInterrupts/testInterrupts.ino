/*
 * Name: testInterrupts.ino
 * Purpose: Test hall effect sensors using interrupts
*/

const int HERTZ= 20;
const int DELAY_TIME = 1000/HERTZ;

const int HALL_EFFECT_PIN = 3;
const int WHEEL_DISTANCE = 12;

volatile boolean take_first_time = false;
volatile boolean take_second_time = false;
volatile int time;
volatile int time_init;

int velocity;

/*
 * name: Setup
 * Purpose: Instantiate interrupt, attach function 
 * "trackHallEffect"
*/
void setup()
{
  Serial.begin(9600);
  
  //pin, interrupt service routine(ISR), interrupt sequence?
  attachInterrupt(0, trackHallEffect, RISING);
  
}

/*
 * Name: loop
 * Purpose: use booleans changed by interrupt function to take 
 * appropriate time stamps. Calculate velocity using 
 * rate = distance/time 
*/
void loop()
{
  if (take_first_time)
  {
    time_init = millis();
  }
  else if (take_second_time)
  {
    time = millis();
  }
  
  velocity = WHEEL_DISTANCE/(time-time_init);
  
  delay(DELAY_TIME);
}

/*
 * Name: trackHallEffect
 * Purpose: Change boolean flags dependant on 1st and 2nd
 * pickup of hall effect sensor
*/
void trackHallEffect()
{
  if (take_first_time)
  {
    take_second_time = true;
    take_first_time = false;
  }
  else {
    take_first_time = true;
    take_second_time = false;
  } 
}
