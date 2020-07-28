#include <Arduino.h>
#include "Timer.h"
#include "Process.h"
#include "MyTerminal.h"

/** Default variable settings **/
// IO
#define INPUT_PIN_RUN_STOP                      A0
#define INPUT_PIN_RUN_INDICATOR                 A1
#define INPUT_PIN_ANALOG_TEMPERATURE            A2
#define INPUT_PIN_ANALOG_FUEL_LEVEL             A3
#define OUTPUT_PIN_GENERATOR_IGNITION           7
#define OUTPUT_PIN_GENERATOR_STARTER_CONTACTOR  8
#define OUTPUT_PIN_GENERATOR_OUTPUT_ENABLED     9
#define OUTPUT_PIN_INDICATOR_ERROR              10
#define OUTPUT_PIN_INDICATOR_WARNING            11
#define OUTPUT_PIN_INDICATOR_TEMPERATURE        12
#define OUTPUT_PIN_INDICATOR_FUEL               13


/**
 * Program VARS 
 */
// Program state varse
#define LOGGING_ENABLED_DEFAULT                 true
#define PROCESS_CONTROL_ENABLED_DEFAULT         true
#define READ_INPUTS_ENABLED_DEFAULT             true
#define CALCULATE_STATE_ENABLED_DEFAULT         true
#define CHECK_LIMITS_ENABLED_DEFAULT            true
#define SET_OUTPUTS_ENABLED_DEFAULT             true
#define MAX_RETRIES_DEFAULT                     3
#define RETRY_WAIT_TIME_SECONDS                 5    // Make realistic
#define STARTER_CONTACTOR_TIME_SECONDS          5    // Make realistic
#define START_TO_ON_TIME_SECONDS                7    // Make realistic
#define GENERATOR_CHECK_TIME_SECONDS            3    // Make realistic
#define GENERATOR_RUNNING_DEBOUNCE_COUNT        10   // Make realistic, heavily dependent on GENERATOR_CHECK_TIME_SECONDS and possible ticks
#define START_SWITCH_DEBOUNCE_COUNT             10   //
#define TEMPERATURE_THRESHOLD_WARNING           70.0
#define TEMPERATURE_THRESHOLD_MAX               88.8
#define FUEL_LEVEL_THRESHOLD_WARNING            10.0
#define FUEL_LEVEL_THRESHOLD_MIN                2.2


// Insantiate an empty version
ProgramVars programVars = {
  // has a state changed?
  .stateChange = false,
  // is logging enabled
  .loggingEnabled = LOGGING_ENABLED_DEFAULT,
  // Process control stages - Maybe not needed in this struct...
  .processControlEnabled = PROCESS_CONTROL_ENABLED_DEFAULT,
  .readInputsEnabled = READ_INPUTS_ENABLED_DEFAULT,
  .calculateStateEnabled = CALCULATE_STATE_ENABLED_DEFAULT,
  .checkLimitsEnabled = CHECK_LIMITS_ENABLED_DEFAULT,
  .setOutputsEnabled = SET_OUTPUTS_ENABLED_DEFAULT,
  // Generator state vars
  .generatorState = false,
  .generatorOn = false,
  // Error and warning is bitwise
  // start | stop | temperature | fuel | 4 | 5 | 6| 7
  .generatorWarning = 0,
  .generatorError = 0,
  // Generator start vars
  .maxRetries = MAX_RETRIES_DEFAULT,
  .numRetries = 0,
  .starterContactorTimeSeconds = STARTER_CONTACTOR_TIME_SECONDS,
  .starterContactorTimeCurrentTicks = 0,
  .checkTimeSeconds = GENERATOR_CHECK_TIME_SECONDS,
  .checkTimeCurrentTicks = 0,
  .generatorRunningDebounceCount = GENERATOR_RUNNING_DEBOUNCE_COUNT,
  .startToOnTimeSeconds = START_TO_ON_TIME_SECONDS,
  .startToOnTimeCurrentTicks = 0,
  .startSwitchDebounceCount = START_SWITCH_DEBOUNCE_COUNT,
  .startSwitchDebounceCurrentTicks = 0,
  .retryWaitTimeSeconds = RETRY_WAIT_TIME_SECONDS,
  .retryWaitTimeCurrentTicks = 0,
  // Environmental conditions
  .checkLevelTemperatureMax = TEMPERATURE_THRESHOLD_MAX,
  .checkLevelTemperatureWarning = TEMPERATURE_THRESHOLD_WARNING,
  .checkLevelFuelLevelMin =FUEL_LEVEL_THRESHOLD_MIN,
  .checkLevelFuelLevelWarning = FUEL_LEVEL_THRESHOLD_WARNING,
  // Inputs
  .inputRunStop = false,
  .inpputRunIndicator = false,
  .inputTemperature = 0.0,
  .inputFuelLevel = 0.0,
  // Outputs
  .outputGeneratorIgnition = false,
  .outputGeneratorStarterContactor = false,
  .outputGeneratorOutputEnable = false,
  .outputIndicatorWarning = false,
  .outputIndicatorError = false,
  .outputIndicatorTemperature = false,
  .outputIndicatorFuelLevel = false
};




/**
 * Program Control Things
 */
Process process;

/**
 * Terminal
 */
MyTerminal terminal(Serial, &programVars);
/*****************************************************
 * Bringing it together closer to the main loop
 * 
 * 
 ****************************************************/


/**
 * This function needs to be incorporated into MyTerminal
 * We may have issues wiht Serial and where it comes from etc...
 * design is by shotgun here
 * 
 * Gross, using in scope variables... yuk
 */
void run_terminal_toggle_led(){

  // Handle the serial input and process 
  terminal.handle_serial_input();

  // Def once per second
  // print logging info if enabled
  if (programVars.loggingEnabled == true) {
  // if (programVars.inputRunStop == true) {
  // if (digitalRead(INPUT_PIN_RUN_STOP) == true) {
    String logMessage = terminal.formatProgVars(timestamp);    
    Serial.println(logMessage);
  }

  // Toggle the LED
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

// We have to wrap the non-static member function to call in our
// callback. Just 'cause 8(
void run_process(void){
  process.process(&programVars, MEDIUM_TIME_MILLIS);
}

// Set the timer callback functions, i.e the functions called
// every short, medium, and long time
// timer_set_fast_time_callback(&medium_timestep);
// timer_set_medium_time_callback(&fast_timestep);

void setup() {
  // Initialise the serial hardware at baude 115200
  Serial.begin(115200);

  // put your setup code here, to run once:
  timer_setup();

  // Set the input and output pin modes, using internal pullup resistors for digital inputs
  process.begin();
  
  // Dummy LED thingee
  pinMode(LED_BUILTIN, OUTPUT);

  // Register the timed functions
  timer_set_long_time_callback(&run_terminal_toggle_led);
  timer_set_medium_time_callback(&run_process);
}

void loop() {
  // Run timed processes
  timer_run_timed_functions();
}