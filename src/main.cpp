#include <Arduino.h>

/** Default variable settings **/
// IO
#define RUN_STOP_INPUT_PIN 0
#define RUN_INDICATOR_INPUT_PIN 1
#define TEMPERATURE_INPUT_PIN 3
// #

// This variables are 'volatile' as it changes in an ISR
volatile uint32_t timestamp = 0;
volatile uint8_t  fastTimeFlag = 0;
volatile uint8_t mediumTimeFlag = 0;
volatile uint8_t longTimeFlag = 0;
// The number of milliseconds in each time period
#define FAST_TIME_MILLIS 10
#define MEDIUM_TIME_MILLIS 100
#define LONG_TIME_MILLIS 1000

/** Repurposing/reusing timer0 for our own purposes
 * This is based on https://learn.adafruit.com/multi-tasking-the-arduino-part-2/timers
 * Basically compare timer0 wihch overflows every milliscond
 * with any value to trigger an interrup every millisecond
 * 
 * NOTE: timer0 at 16MHz is not quite 1millisecond, more like 0.97
 **/
/** ISR to handle timer increments
 * Increment timestamp
 * Set fast, medium, and long time period flags
 * 
 * NOTE: We want to do as little processing as possible in an ISR
 **/
SIGNAL(TIMER0_COMPA_vect) 
{
  // This variable counts millis to inrement seconds
  // It is 'static' which means it's value is retained
  // between function calls, and is local to the function
  // The static variable is only initialised once
  static int16_t milliCount = 0;

  // Increment the millis count
  milliCount++;

  // Check the time periods and set flags
  if(milliCount == FAST_TIME_MILLIS)
  {
    fastTimeFlag = 1;
  }
  if(milliCount == MEDIUM_TIME_MILLIS)
  {
    mediumTimeFlag = 1;
  }
  if(milliCount == LONG_TIME_MILLIS)
  {
    longTimeFlag = 1;
  }

  // Check if we have had 1000 milliseconds and increment timestamp
  if(milliCount >= 1000)
  {
    // Increment the timestamp and reset milliCount
    timestamp++;
    milliCount = 0;
  }
}

void setup() {
  // put your setup code here, to run once:

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

void loop() {
  // put your main code here, to run repeatedly:
}