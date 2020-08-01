#include "Timer.h"

// These variables are 'volatile' as it changes in an ISR
volatile uint32_t timestamp = 0;
volatile bool fastTimeFlag = false;
volatile bool mediumTimeFlag = false;
volatile bool longTimeFlag = false;

// void function pointers for timed function callbacks
void (* fastTimeCallback)();
void (* mediumTimeCallback)();
void (* longTimeCallback)();

/**
 * Timer0 is already used for millis() - we'll just interrupt somewhere
 * in the middle and call the "Compare A" function below
  */
void timer_setup(void){
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
}

/**
 * Call the registered callback functions at the fast, medium, and log times
 */
void timer_run_timed_functions(void){
  if ( fastTimeCallback && fastTimeFlag ) {
    fastTimeFlag = false;
    fastTimeCallback();
  }
  
  if (mediumTimeCallback && mediumTimeFlag ) {
    mediumTimeFlag = false;
    mediumTimeCallback();
  }
  
  if (longTimeCallback && longTimeFlag ) {
    longTimeFlag = false;
    longTimeCallback();
  }
}

/**
 * Set the time based callback functions
 */
void timer_set_fast_time_callback(void (* fast_callback)()) {
  fastTimeCallback = fast_callback;
}

void timer_set_medium_time_callback(void (* medium_callback)()) {
  mediumTimeCallback = medium_callback;
}

void timer_set_long_time_callback(void (* long_callback)()) {
  longTimeCallback = long_callback;
}

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
  static uint16_t milliCount = 0;
  static uint16_t fastTimeCountMillis = 0;
  static uint16_t mediumTimeCountMillis = 0;
  static uint16_t longTimeCountMillis = 0;


  // Increment the millis count
  milliCount++;
  fastTimeCountMillis++;
  mediumTimeCountMillis++;
  longTimeCountMillis++;

  // Check the time periods and set flags
  if(fastTimeCountMillis == FAST_TIME_MILLIS)
  {
    fastTimeCountMillis = 0;
    fastTimeFlag = true;
  }
  if(mediumTimeCountMillis == MEDIUM_TIME_MILLIS)
  {
    mediumTimeCountMillis = 0;
    mediumTimeFlag = true;
  }
  if(longTimeCountMillis == LONG_TIME_MILLIS)
  {
    longTimeCountMillis = 0;
    longTimeFlag = true;
  }

  // Check if we have had 1000 milliseconds and increment timestamp
  if(milliCount >= 1000)
  {
    // Increment the timestamp and reset milliCount
    timestamp++;
    milliCount = 0;
  }
}
