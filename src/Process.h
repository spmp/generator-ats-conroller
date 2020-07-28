/** 
 * This is the concrete implementation of the process control
 * for
 * Generator ATS Controller
 */
#ifndef _ARDUINO_PROCESS
#define _ARDUINO_PROCESS
#pragma once

#include "Arduino.h"

/** Default variable settings **/
// IO
#ifndef INPUT_PIN_RUN_STOP
#define INPUT_PIN_RUN_STOP                      A0
#endif
#ifndef INPUT_PIN_RUN_INDICATOR
#define INPUT_PIN_RUN_INDICATOR                 A1
#endif
#ifndef INPUT_PIN_ANALOG_TEMPERATURE
#define INPUT_PIN_ANALOG_TEMPERATURE            A2
#endif
#ifndef INPUT_PIN_ANALOG_FUEL_LEVEL
#define INPUT_PIN_ANALOG_FUEL_LEVEL             A3
#endif
#ifndef OUTPUT_PIN_GENERATOR_IGNITION
#define OUTPUT_PIN_GENERATOR_IGNITION           7
#endif
#ifndef OUTPUT_PIN_GENERATOR_STARTER_CONTACTOR
#define OUTPUT_PIN_GENERATOR_STARTER_CONTACTOR  8
#endif
#ifndef OUTPUT_PIN_GENERATOR_OUTPUT_ENABLED
#define OUTPUT_PIN_GENERATOR_OUTPUT_ENABLED     9
#endif
#ifndef OUTPUT_PIN_INDICATOR_ERROR
#define OUTPUT_PIN_INDICATOR_ERROR              10
#endif
#ifndef OUTPUT_PIN_INDICATOR_WARNING
#define OUTPUT_PIN_INDICATOR_WARNING            11
#endif
#ifndef OUTPUT_PIN_INDICATOR_TEMPERATURE
#define OUTPUT_PIN_INDICATOR_TEMPERATURE        12
#endif
#ifndef OUTPUT_PIN_INDICATOR_FUEL
#define OUTPUT_PIN_INDICATOR_FUEL               13
#endif


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


class Process {
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
        ProgramVars _programVars;

        /**
         * The process control stages
         */
        void read_inputs(ProgramVars *progVars, uint16_t periodTimeMillis);
        void calculate_state(ProgramVars *progVars, uint16_t periodTimeMillis);
        void check_limits(ProgramVars *progVars, uint16_t periodTimeMillis);
        void set_outputs(ProgramVars *progVars, uint16_t periodTimeMillis);

        /**
         * Helper functions
         */
        bool read_run_stop(uint8_t digitalInputPin, uint16_t debounceNum);
        bool read_generator_running(uint8_t digitalInputPin, uint16_t debounceNum);
        uint16_t secondsToTicks(uint16_t seconds, uint16_t ticksPerCycle);
};

#endif
