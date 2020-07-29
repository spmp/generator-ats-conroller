/** 
 * This is a timer
 */
#ifndef _ARDUINO_TIMER
#define _ARDUINO_TIMER
#pragma once

#include "Arduino.h"

// The number of milliseconds in each time period
#define FAST_TIME_MILLIS 10
#define MEDIUM_TIME_MILLIS 100
#define LONG_TIME_MILLIS 1000

// This variables are 'volatile' as it changes in an ISR
volatile extern uint32_t timestamp;
volatile extern bool fastTimeFlag;
volatile extern bool mediumTimeFlag;
volatile extern bool longTimeFlag;

/**
 * Setup function to be run in setup(){...}
 * This sets the compare interrupt
 */
void timer_setup(void);

/**
 * Call the registered callback functions at the fast, medium, and log times
 */
void timer_run_timed_functions(void);


/**
 * @brief Set the name of the function that gets called on the fast interval. N.B. is called within an interrupt
 * @param  fast_Callback, the address of the function to run on the fast interval. i.e. &blahfunction
 * @return none
 **/
void timer_set_fast_time_callback(void (* fast_callback)());

/**
 * @brief Set the name of the function that gets called in the medium timeframe. N.B. is called within an interrupt
 * @param  medium_callback, the address of the function to run on the medium time interval. i.e. &blahfunction
 * @return none
 **/
void timer_set_medium_time_callback(void (* medium_callback)());

/**
 * @brief Set the name of the function that gets called once every second. N.B. is called within an interrupt
 * @param  long_callback, the address of the function to run every second. i.e. &blahfunction
 * @return none
 **/
void timer_set_long_time_callback(void (* long_callback)());

#endif