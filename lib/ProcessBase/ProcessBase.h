/** 
 * This is the concrete implementation of the process control
 * for
 * Generator ATS Controller
 */
#ifndef _ARDUINO_PROCESS_BASE
#define _ARDUINO_PROCESS_BASE
#pragma once

#include "Arduino.h"

// Define the LED_BUILTIN if not defined
#ifndef LED_BUILTIN
#ifdef ARDUINO_ARCH_ESP32
// The LED is on pin 2 for my node32s knockoff
#define LED_BUILTIN       2
#endif
#endif

/**
 * A struct for containing all the important settable and gettable program vars
 */
struct ProgramVarsBase {
  // has a state changed?
  bool stateChange;
  // is logging enabled
  bool loggingEnabled;                                  // L
  // Process control stages - Maybe not needed in this struct...
  bool processControlEnabled;                           // P
  bool readInputsEnabled;                               // I
  bool calculateStateEnabled;                           // Y
  bool checkLimitsEnabled;                              // U
  bool setOutputsEnabled;                               // O
};


template <typename ProgramVars>
class ProcessBase {
  public:
    // Process(ProgramVars *progVars);
    ProcessBase() {};
    ~ProcessBase() {};

    /**
     * Setup the hardware, inputs and outputs
     */
    void begin(void);

    /**
     * Run the complete process control loop
     * Ideally this should be registered to a timer
     * on a 'medium' or 'fast' time interval
     */
    void process(ProgramVars *progVars, uint16_t periodTimeMillis);

    /**
     * Flash an LED to show the uC is alive
     * Ideally this should be registered to a timer
     * on a 'long' time interval
     * 
     * This should likely be overwridden
     */
    void flash_led(ProgramVars *progVars, uint16_t periodTimeMillis);
  protected:
    ProgramVars _programVars;

    /**************************************************************************
     * The process control stages
     *************************************************************************/

    /**
     * Read the uC inputs
     */
    void read_inputs(ProgramVars *progVars, uint16_t periodTimeMillis);

    /**
     * Calculate the new program states based on inputs etc.
     */
    void calculate_state(ProgramVars *progVars, uint16_t periodTimeMillis);

    /**
     * Check for limit conditions, and handle warnings and errors
     */
    void check_limits(ProgramVars *progVars, uint16_t periodTimeMillis);

    /**
     * Set the uC outputs
     */
    void set_outputs(ProgramVars *progVars, uint16_t periodTimeMillis);
};

#endif
