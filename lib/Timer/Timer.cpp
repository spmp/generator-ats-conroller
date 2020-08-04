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
 * This function is run within the timer interrupt ISR
 * It is assumed that the interrupt occurs every millisecond.
 * 
 * Increment timestamp
 * Set fast, medium, and long time period flags
 * 
 * NOTE: We want to do as little processing as possible in an ISR
 */
void increment_time_periods_isr(void) {
  // This variable counts millis to inrement seconds
  // It is 'static' which means it's value is retained
  // between function calls, and is local to the function
  // The static variable is only initialised once
  volatile static uint16_t milliCount = 0;
  volatile static uint16_t fastTimeCountMillis = 0;
  volatile static uint16_t mediumTimeCountMillis = 0;
  volatile static uint16_t longTimeCountMillis = 0;


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

/**
 * Timer and timer ISR functions
 */
#ifdef ARDUINO_ARCH_ESP32
hw_timer_t * timer = NULL;
volatile SemaphoreHandle_t timerSemaphore;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

void IRAM_ATTR onTimer(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  increment_time_periods_isr();
  // increment_time_periods_isr();
  portEXIT_CRITICAL_ISR(&timerMux);
  // Give a semaphore that we can check in the loop
  xSemaphoreGiveFromISR(timerSemaphore, NULL);
}
#else
/** Repurposing/reusing timer0 for our own purposes
 * This is based on https://learn.adafruit.com/multi-tasking-the-arduino-part-2/timers
 * Basically compare timer0 wihch overflows every milliscond
 * with any value to trigger an interrup every millisecond
 * 
 * NOTE: timer0 at 16MHz is not quite 1millisecond, more like 0.97
 **/
/** ISR to handle timer increments
 **/
SIGNAL(TIMER0_COMPA_vect) {
  increment_time_periods_isr();
}
#endif

/**
 * Setup timer if required and ensure an interrupt is called
 * every millisecond
 * 
 * ATMega 328p:
 *   Timer0 is already used for millis() - we'll just interrupt somewhere
 *   in the middle and call the "Compare A" function below
 * ESP32:
 *  Setup a timer, add interrupt function, enable interrupt etc.
 *  TODO: Try to attach to the existing millisecond timer
  */
void timer_setup(void){
#ifdef ARDUINO_ARCH_ESP32
  // Setup Timer 1 on interrupt
  // Create semaphore to inform us when the timer has fired
  timerSemaphore = xSemaphoreCreateBinary();
  // Use 1st timer of 4 (counted from zero).
  // Set 80 divider for prescaler (see ESP32 Technical Reference Manual for more
  // info).
  // The prescalar divides the clock (80MHz), so for millisecond that is
  // 80e6/(1/1e-3) = 80000
  // BUT the divider is a 16bit number, so we must reduce by a power of ten
  // and increase the 'timerAlarmWrite' time to 10
  timer = timerBegin(0, 80, true);
  // Attach onTimer function to our timer.
  timerAttachInterrupt(timer, &onTimer, true);
  // Set alarm to call interrupt 1 millisecond
  // TODO: Check this against lowering prescalar and increasing this
  // The 'true' ensures this timer repeats
  timerAlarmWrite(timer, 1000, true);
  // Enable the interrupt alarm
  timerAlarmEnable(timer);
#else
  // Very similar to the above, but for ATMega328p
  // The timer is already initialised by Arduino,
  // We simply enable an ISR on arbitrary value (0xAF)
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);
#endif
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
