/** 
 * This is the concrete implementation of the process control
 * for
 * Generator ATS Controller
 */
#ifndef _ARDUINO_PROCESS
#define _ARDUINO_PROCESS
#pragma once

#include "Arduino.h"
#include "ProcessBase.h"
#include "ProcessBase.cpp"


/** Default variable settings **/
// IO
#define INPUT_PIN_RUN_STOP                      A1
#define INPUT_PIN_RUN_INDICATOR                 A2
#define INPUT_PIN_ANALOG_TEMPERATURE            A3
#define INPUT_PIN_ANALOG_FUEL_LEVEL             A4
#define OUTPUT_PIN_GENERATOR_IGNITION           4
#define OUTPUT_PIN_GENERATOR_STARTER_CONTACTOR  5
#define OUTPUT_PIN_GENERATOR_OUTPUT_ENABLED     6
#define OUTPUT_PIN_INDICATOR_ERROR              7
#define OUTPUT_PIN_INDICATOR_WARNING            8
#define OUTPUT_PIN_INDICATOR_TEMPERATURE        9
#define OUTPUT_PIN_INDICATOR_FUEL               10


// Generator states
#define GENERATOR_STATE_OFF                 0
#define GENERATOR_STATE_STARTING            2
#define GENERATOR_STATE_START_CHECK         3
#define GENERATOR_STATE_STOPPING            4
#define GENERATOR_STATE_RUNNING             8
#define GENERATOR_STATE_ON                  16
#define GENERATOR_STATE_RETRY_WAIT          32
#define GENERATOR_STATE_START_ERROR         64
#define GENERATOR_STATE_STOP_ERROR          128

// // 8 bit error register
// #define WARNING_TEMPERATURE                 0
// #define WARNING_FUEL                        1
// #define ERROR_GENERATOR_START               2
// #define ERROR_GENERATOR_STOP                3
// #define ERROR_TEMPERATURE                   4
// #define ERROR_FUEL                          5
// #define WARNING_THRESHOLD                   3 // If error reg over this is warning


/**
 * A struct containing all the important settable and gettable program vars
 * With serial string prefix
 */
struct ProgramVars {
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
  // Generator state vars
  uint16_t generatorState;                              // r
  bool generatorOn;                                     // o
  // Error and warning is bitwise
  // start | stop | temperature | fuel | 4 | 5 | 6| 7
  uint8_t generatorWarning;                             // w
  uint8_t generatorError;                               // e
  // Generator start vars
  uint8_t maxRetries;                                   // S
  uint8_t numRetries;                                   // s
  uint16_t starterContactorTimeSeconds;                 // C
  uint16_t starterContactorTimeCurrentTicks;            // 
  uint16_t checkTimeSeconds;                            // c
  uint16_t checkTimeCurrentTicks;                       //
  uint16_t generatorRunningDebounceCount;               // D
  uint16_t startToOnTimeSeconds;                        // V
  uint16_t startToOnTimeCurrentTicks;                   // v
  uint16_t startSwitchDebounceCount;                    // d
  uint16_t startSwitchDebounceCurrentTicks;             //
  uint16_t retryWaitTimeSeconds;                        // N
  uint16_t retryWaitTimeCurrentTicks;                   //
  // Environmental conditions
  float checkLevelTemperatureMax;                       // T
  float checkLevelTemperatureWarning;                   // t
  float checkLevelFuelLevelMin;                         // F
  float checkLevelFuelLevelWarning;                     // f
  // Inputs
  bool inputRunStop;                                    // R
  bool inpputRunIndicator;                              // i
  float   inputTemperature;                             // y
  float   inputFuelLevel;                               // u
  // Outputs
  bool outputGeneratorIgnition;                         // b
  bool outputGeneratorStarterContactor;                 // n
  bool outputGeneratorOutputEnable;                     // m
  bool outputIndicatorWarning;                          // ,
  bool outputIndicatorError;                            // .
  bool outputIndicatorTemperature;                      // /
  bool outputIndicatorFuelLevel;                        // ?
};


class Process: public ProcessBase<ProgramVars> {
  public:
    // Process(ProgramVars *progVars);
    Process() {};
    ~Process(void) {};

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
  private:

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

    /**************************************************************************
     * Helper functions
     *************************************************************************/

    /**
     * Read the 'run/stop' input
     */
    bool read_run_stop(uint8_t digitalInputPin, uint16_t debounceNum);

    /**
     * Read the 'generator running' input
     */
    bool read_generator_running(uint8_t digitalInputPin, uint16_t debounceNum);

    /**
     * Convert seconds to ticks (or program loop cycles) based on current 
     * ticks per cycle
     */
    uint16_t secondsToTicks(uint16_t seconds, uint16_t ticksPerCycle);

    /**
     * Calculate an output state to flash it asymetrically
     */
    bool flash_error_light(bool indicatorState, uint8_t onTicks, uint8_t offTicks);
};

#endif
