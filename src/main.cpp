#include <Arduino.h>


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
#define LOGGING_ENABLED_DEFAULT             true
#define PROCESS_CONTROL_ENABLED_DEFAULT     true
#define READ_INPUTS_ENABLED_DEFAULT         true
#define CALCULATE_STATE_ENABLED_DEFAULT     true
#define CHECK_LIMITS_ENABLED_DEFAULT        true
#define SET_OUTPUTS_ENABLED_DEFAULT         true
#define MAX_RETRIES_DEFAULT                 3
#define RETRY_WAIT_TIME_SECONDS             5 // Make realistic
#define STARTER_CONTACTOR_TIME_SECONDS      5 // Make realistic
#define START_TO_ON_TIME_SECONDS            7 // Make realistic
#define GENERATOR_CHECK_TIME_SECONDS        3 // Make realistic
#define GENERATOR_RUNNING_DEBOUNCE_COUNT    10 // Make realistic, heavily dependent on GENERATOR_CHECK_TIME_SECONDS and possible ticks
#define START_SWITCH_DEBOUNCE_COUNT         10 //
#define TEMPERATURE_THRESHOLD_WARNING       70.0
#define TEMPERATURE_THRESHOLD_MAX           88.8
#define FUEL_LEVEL_THRESHOLD_WARNING        10.0
#define FUEL_LEVEL_THRESHOLD_MIN            2.2


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
 * Timer things
 */
// The number of milliseconds in each time period
#define FAST_TIME_MILLIS 10
#define MEDIUM_TIME_MILLIS 100
#define LONG_TIME_MILLIS 1000

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

/*************************************************
 * Serial Console
 *  - Starting with OUTPUT
 *  - No idea how we do INPUT 
 *************************************************/
#define TERMINAL_SERIAL_BAUD          115200

// Argument type defines
#define ARGUMENT_TYPE_NONE            0
#define ARGUMENT_TYPE_LONG            1
#define ARGUMENT_TYPE_DOUBLE          2
#define ARGUMENT_TYPE_STRING          3

// Serial buffer and message
// TODO: Removee these when the function is added to MyTerminal
String serialBuffer = "";
String messages = "";


String strSep = ",";
// A 'struct' to hold parsed commands and arguments
struct CommandAndArguments {
    char    command;
    int     argType;
    long    argLong;
    String  argString;
    boolean parseState;
};

int getCommandAndArgument(String inputString, char *command, String *argument) {
    // Check that the String is long enough to process and return fail otherwise
    if (inputString.length() <= 1) {
        return EXIT_FAILURE;
    }

    *command = inputString.charAt(0);
    *argument = inputString.substring(1);
    return EXIT_SUCCESS;
}

int stringToLong(String inputString, long *targetInt) {
    // convert the input string to an integer
    int32_t intTemp = inputString.toInt();
    // If the resulting integer is not 0 then no problem
    if (intTemp != 0) {
    *targetInt = intTemp;
    return EXIT_SUCCESS;
    // If the input string is literally "0" no problem
    } else if (inputString == "0") {
    *targetInt = 0;
    return EXIT_SUCCESS;
    // Otherwise there was a problem
    } else {
    return EXIT_FAILURE;
    }    
}

CommandAndArguments parseCommandArgs(String commandArgs) {
    char comChar = 'h';
    int argType = ARGUMENT_TYPE_NONE;
    long argLong = 0;
    String argString = "";


    // Trim the result, include removing the trailing '/n'
    commandArgs.trim();

    // Check that the String is long enough to process and return fail otherwise
    if (commandArgs.length() == 0) {
    return CommandAndArguments{
        comChar, argType, argLong, argString, EXIT_FAILURE
    };
    } 
    // Get the command
    comChar = commandArgs.charAt(0);

    // If there are enough characters in 'commandArgs' get and parse them
    if (commandArgs.length() > 1) {
    // Separate the argument from the command
    argString = commandArgs.substring(1);
    // If we can convert the argString to a number we do
    if (stringToLong(argString, &argLong) == EXIT_SUCCESS) {
        argType = ARGUMENT_TYPE_LONG;
    } else {
        argType = ARGUMENT_TYPE_STRING;
    }
    }

    // Return all the things
    return CommandAndArguments{
    comChar, argType, argLong, argString, EXIT_SUCCESS
    };
}


// Boolean version
boolean argDisplayOrSetBoolean(String argName, CommandAndArguments comAndArg, boolean *var, String *message) {
    if (comAndArg.argType == ARGUMENT_TYPE_NONE) {
    *message = argName + " is : '" + String(*var) + "'";
    return false;
    }
    // Check if true both string and Long
    comAndArg.argString.toLowerCase();
    if (
        // String and equals 'true'
        (comAndArg.argType == ARGUMENT_TYPE_STRING && comAndArg.argString == "true") ||
        // Long and equals 1
        (comAndArg.argType == ARGUMENT_TYPE_LONG && comAndArg.argLong == 1)
    ) {
        *var = true;
        *message = "Set '" + argName + "' to : 'true'";
        return true;
    }
    // Check if false both string and Long
    if (
        // String and equals 'true'
        (comAndArg.argType == ARGUMENT_TYPE_STRING && comAndArg.argString == "false") ||
        // Long and equals 1
        (comAndArg.argType == ARGUMENT_TYPE_LONG && comAndArg.argLong == 0)
    ) {
        *var = false;
        *message = "Set '" + argName + "' to : 'false'";
        return true;
    }
    return false;
}

boolean argDisplayOrSetUint8(String argName,CommandAndArguments comAndArg, uint8_t *var, String *message) {
    if (comAndArg.argType == ARGUMENT_TYPE_NONE) {
    *message = argName + " is : " + String(*var);
    return false;
    }
    if (comAndArg.argType == ARGUMENT_TYPE_LONG) {
    *var = comAndArg.argLong;
    *message = "Set '" + argName + "' to : " + String(*var);
    return true;
    }
    return false;
}

boolean argDisplayOrSetUint16(String argName, CommandAndArguments comAndArg, uint16_t *var, String *message) {
    if (comAndArg.argType == ARGUMENT_TYPE_NONE) {
    *message = argName + " is : " + String(*var);
    return false;
    }
    if (comAndArg.argType == ARGUMENT_TYPE_LONG) {
    *var = comAndArg.argLong;
    *message = "Set '" + argName + "' to : " + String(*var);
    return true;
    }
    return false;
}

boolean argDisplayOrSetFloatFromLong(String argName, CommandAndArguments comAndArg, float *var, uint16_t denominator, String *message) {
    if (comAndArg.argType == ARGUMENT_TYPE_NONE) {
    *message = argName + " is : " + String(*var);
    return false;
    }
    if (comAndArg.argType == ARGUMENT_TYPE_LONG) {
    *var = 1.0 * comAndArg.argLong / denominator;
    *message = "Set '" + argName + "' to : " + String(*var);
    return true;
    }
    return false;
}

/**
 * Process the UART commands, updating the variables or producing a string message
 */
int processCommands(String inputString, ProgramVars *progVars, String *message) {
  // Parse the 'inputString'
  CommandAndArguments comArgState = parseCommandArgs(inputString);

  // Exit with message if no command
  if (comArgState.parseState == EXIT_FAILURE) {
    *message = F("Input string is not a valid command/argument");
    return EXIT_FAILURE;
  }

  // Let us process the commands
  switch (comArgState.command)
  {
  case 'h':
    progVars->stateChange = false;
    *message = String("Help: See source code, Strings too large for Arduino");
  //      *message = String(F("Help: \n")) + 
  //         String(F("'L': loggingEnabled\n")) +
  // // Process control stages
  //         String(F("'P': processControlEnabled\n")) +
  //         String(F("'I': readInputsEnabled\n")) +
  //         String(F("'Y': calculateStateEnabled\n")) +
  //         String(F("'U': checkLimitsEnabled\n")) +
  //         String(F("'O': setOutputsEnabled\n")) +
  // // Generator state vars
  //         String(F("'r': generatorState\n")) +
  //         String(F("'o': generatorOn\n")) +
  // // Error and warning is bitwise
  //         String(F("'w': generatorWarning\n")) +
  //         String(F("'e': generatorError\n")) +
  // // Generator start vars
  //         String(F("'S': maxRetries\n")) +
  //         String(F("'s': numRetries\n")) +
  //         String(F("'C': starterContactorTimeSeconds\n")) +
  //         String(F("'c': checkTimeSeconds\n")) +
  //         String(F("'V': startToOnTimeSeconds\n")) +
  //         String(F("'v': startToOnTimeCurrentTicks\n")) +
  // // Environmental conditions
  //         String(F("'T': checkLevelTemperatureMax\n")) +
  //         String(F("'t': checkLevelTemperatureWarning\n")) +
  //         String(F("'F': checkLevelFuelLevelMin\n")) +
  //         String(F("'f': checkLevelFuelLevelWarning\n")) +
  // // Inputs
  //         String(F("'R': inputRunStop\n")) +
  //         String(F("'i': inpputRunIndicator\n")) +
  //         String(F("'y': inputTemperature\n")) +
  //         String(F("'u': inputFuelLevel\n")) +
  // // Outputs
  //         String(F("'b': outputGeneratorIgnition\n")) +
  //         String(F("'n': outputGeneratorStarterContactor\n")) +
  //         String(F("'m': outputGeneratorOutputEnable\n")) +
  //         String(F("',': outputIndicatorWarning\n")) +
  //         String(F("'.': outputIndicatorError\n")) +
  //         String(F("'/': outputIndicatorTemperature\n")) +
  //         String(F("'?': outputIndicatorFuelLevel\n"));
    break;
  case 'L':
    progVars->stateChange = argDisplayOrSetBoolean("loggingEnabled", comArgState, &progVars->loggingEnabled, message);
    break;
  // Process control stages
  case 'P':
    progVars->stateChange = argDisplayOrSetBoolean("processControlEnabled", comArgState, &progVars->processControlEnabled, message);
    break;
  case 'I':
    progVars->stateChange = argDisplayOrSetBoolean("readInputsEnabled", comArgState, &progVars->readInputsEnabled, message);
    break;
  case 'Y':
    progVars->stateChange = argDisplayOrSetBoolean("calculateStateEnabled", comArgState, &progVars->calculateStateEnabled, message);
    break;
  case 'U':
    progVars->stateChange = argDisplayOrSetBoolean("checkLimitsEnabled", comArgState, &progVars->checkLimitsEnabled, message);
    break;
  case 'O':
    progVars->stateChange = argDisplayOrSetBoolean("setOutputsEnabled", comArgState, &progVars->setOutputsEnabled, message);
    break;
  // Generator state vars
  case 'r':
    progVars->stateChange = argDisplayOrSetUint16("generatorState", comArgState, &progVars->generatorState, message);
    break;
  // case 'o':
  //   progVars->stateChange = argDisplayOrSetBoolean("generatorOn", comArgState, &progVars->generatorOn, message);
  //   break;
  // Error and warning is bitwise
  // start | stop | temperature | fuel | 4 | 5 | 6| 7
  // case 'w':
  //   progVars->stateChange = argDisplayOrSetUint8("generatorWarning", comArgState, &progVars->generatorWarning, message);
  //   break;
  // case 'e':
  //   progVars->stateChange = argDisplayOrSetUint8("generatorError", comArgState, &progVars->generatorError, message);
  //   break;
  // Generator start vars
  case 'S':
    progVars->stateChange = argDisplayOrSetUint8("maxRetries", comArgState, &progVars->maxRetries, message);
    break;
  case 's':
    progVars->stateChange = argDisplayOrSetUint8("numRetries", comArgState, &progVars->numRetries, message);
    break;
  case 'C':
    progVars->stateChange = argDisplayOrSetUint16("starterContactorTimeSeconds", comArgState, &progVars->starterContactorTimeSeconds, message);
    break;
  // case 'XX':
  //   progVars->stateChange = argDisplayOrSetUint16("starterContactorTimeCurrentTicks", comArgState, &progVars->starterContactorTimeCurrentTicks, message);
  //   break;
  case 'c':
    progVars->stateChange = argDisplayOrSetUint16("checkTimeSeconds", comArgState, &progVars->checkTimeSeconds, message);
    break;
  case 'D':
    progVars->stateChange = argDisplayOrSetUint16("generatorRunningDebounceCount", comArgState, &progVars->generatorRunningDebounceCount, message);
    break;
  case 'd':
    progVars->stateChange = argDisplayOrSetUint16("startSwitchDebounceCount", comArgState, &progVars->startSwitchDebounceCount, message);
    break;
  case 'V':
    progVars->stateChange = argDisplayOrSetUint16("startToOnTimeSeconds", comArgState, &progVars->startToOnTimeSeconds, message);
    break;
  case 'N':
    progVars->stateChange = argDisplayOrSetUint16("retryWaitTimeSeconds", comArgState, &progVars->retryWaitTimeSeconds, message);
    break;
  // case 'v':
  //   progVars->stateChange = argDisplayOrSetUint16("startToOnTimeCurrentTicks", comArgState, &progVars->startToOnTimeCurrentTicks, message);
  //   break;
  // // Environmental conditions
  // case 'T':
  //   progVars->stateChange = argDisplayOrSetFloatFromLong("checkLevelTemperatureMax", comArgState, &progVars->checkLevelTemperatureMax, 100, message);
  //   break;
  // case 't':
  //   progVars->stateChange = argDisplayOrSetFloatFromLong("checkLevelTemperatureWarning", comArgState, &progVars->checkLevelTemperatureWarning, 100, message);
  //   break;
  // case 'F':
  //   progVars->stateChange = argDisplayOrSetFloatFromLong("checkLevelFuelLevelMin", comArgState, &progVars->checkLevelFuelLevelMin, 100, message);
  //   break;
  // case 'f':
  //   progVars->stateChange = argDisplayOrSetFloatFromLong("checkLevelFuelLevelWarning", comArgState, &progVars->checkLevelFuelLevelWarning, 100, message);
  //   break;
  // Inputs
  case 'R':
    progVars->stateChange = argDisplayOrSetBoolean("inputRunStop", comArgState, &progVars->inputRunStop, message);
    break;
  case 'i':
    progVars->stateChange = argDisplayOrSetBoolean("inpputRunIndicator", comArgState, &progVars->inpputRunIndicator, message);
    break;
  // case 'y':
  //   progVars->stateChange = argDisplayOrSetFloatFromLong("inputTemperature", comArgState, &progVars->inputTemperature, 100, message);
  //   break;
  // case 'u':
  //   progVars->stateChange = argDisplayOrSetFloatFromLong("inputFuelLevel", comArgState, &progVars->inputFuelLevel, 100, message);
  //   break;
  // Outputs
  // case 'b':
  //   progVars->stateChange = argDisplayOrSetBoolean("outputGeneratorIgnition", comArgState, &progVars->outputGeneratorIgnition, message);
  //   break;
  // case 'n':
  //   progVars->stateChange = argDisplayOrSetBoolean("outputGeneratorStarterContactor", comArgState, &progVars->outputGeneratorStarterContactor, message);
  //   break;
  // case 'm':
  //   progVars->stateChange = argDisplayOrSetBoolean("outputGeneratorOutputEnable", comArgState, &progVars->outputGeneratorOutputEnable, message);
  //   break;
  // case ',':
  //   progVars->stateChange = argDisplayOrSetBoolean("outputIndicatorWarning", comArgState, &progVars->outputIndicatorWarning, message);
  //   break;
  case '.':
    progVars->stateChange = argDisplayOrSetBoolean("outputIndicatorError", comArgState, &progVars->outputIndicatorError, message);
    break;
  // case '/':
  //   progVars->stateChange = argDisplayOrSetBoolean("outputIndicatorTemperature", comArgState, &progVars->outputIndicatorTemperature, message);
  //   break;
  // case '?':
  //   progVars->stateChange = argDisplayOrSetBoolean("outputIndicatorFuelLevel", comArgState, &progVars->outputIndicatorFuelLevel, message);
  //   break;
  default:
    progVars->stateChange = false;
    *message = "No recognised command";
    break;
  }
  return EXIT_SUCCESS;
};


void handle_serial_input(void) {
  // terminal.readSerialAndProcessCommands(&programVars);
  // While there are characters in the Serial buffer
  // read them in one at a time into sBuffer
  // We need to go via char probably due to implicit type conversions
  while(Serial.available() > 0){
    char inChar = Serial.read();
    serialBuffer += inChar;
  }

  // If the buffers end in newline, try to parse the command an arguments
  if(serialBuffer.endsWith("\n")) {
    // Print out the buffer - for fun
    Serial.println(serialBuffer);
    // Process the commands
    processCommands(serialBuffer, &programVars, &messages);
    // Print the message
    Serial.println(messages);
    // Reset the buffer to empty
    serialBuffer = "";
  }
}

String formatProgVars(long time, ProgramVars progVars) {
  // Mode specific parts
  String modeString =  String("");
  switch(progVars.generatorState) {
    case GENERATOR_STATE_OFF:
      // modeString = String(", m: off, timer: 0");
      modeString = String(", m: off") +
      // modeString = String(", m: ") + progVars.generatorState +
        String(", t: 0/0");
      break;
    case GENERATOR_STATE_ON:
      // modeString = String(", m:  on, timer: 0");
      modeString = String(", m: on") +
      // modeString = String(", m: ") + progVars.generatorState +
        String(", t: 0/0");
      break;
    case GENERATOR_STATE_STARTING:
      // modeString = String(", m:  starting, timer: ") + String(progVars.starterContactorTimeCurrentTicks/MEDIUM_TIME_MILLIS);
      modeString = String(", m: starting") +
      // modeString = String(", m: ") + progVars.generatorState + 
        String(", t: ") + 
        progVars.starterContactorTimeCurrentTicks/MEDIUM_TIME_MILLIS + "/" + progVars.starterContactorTimeSeconds;
      break;
    case GENERATOR_STATE_STOPPING:
      // modeString = String(", m:  stopping, timer: ") + String(progVars.starterContactorTimeCurrentTicks/MEDIUM_TIME_MILLIS);
      modeString = String(", m: stopping") +
      // modeString = String(", m: ") + progVars.generatorState +
        String(", t: ") + 
        String(progVars.starterContactorTimeCurrentTicks/MEDIUM_TIME_MILLIS) + "/" + progVars.starterContactorTimeSeconds;
      break;
    case GENERATOR_STATE_START_CHECK:
      // modeString = String(", m:  start-check, timer: ") + String(progVars.checkTimeCurrentTicks/MEDIUM_TIME_MILLIS);
      modeString = String(", m: start-check") +
      // modeString = String(", m: ") + progVars.generatorState +
        String(", t: ") +
        String(progVars.checkTimeCurrentTicks/MEDIUM_TIME_MILLIS) + "/" + progVars.checkTimeSeconds;
      break;
    case GENERATOR_STATE_RUNNING:
      // modeString = String(", m:  running, timer: ") + String(progVars.startToOnTimeCurrentTicks/MEDIUM_TIME_MILLIS);
      modeString = String(", m: running") +
      // modeString = String(", m: ") + progVars.generatorState +
        String(", t: ") + 
        String(progVars.startToOnTimeCurrentTicks/MEDIUM_TIME_MILLIS)+ "/" + progVars.startToOnTimeSeconds;
      break;
    case GENERATOR_STATE_RETRY_WAIT:
      // modeString = String(", m:  retrying, timer: ") + String(progVars.retryWaitTimeCurrentTicks/MEDIUM_TIME_MILLIS);
      modeString = String(", m: retry-wait") +
      // modeString = String(", m: ") + progVars.generatorState +
        String(", t: ") +
        String(progVars.retryWaitTimeCurrentTicks/MEDIUM_TIME_MILLIS) + "/" + progVars.retryWaitTimeSeconds;
      break;
    case GENERATOR_STATE_START_ERROR:
      // modeString = String(", m:  start-error, timer: 0");
      modeString = String(", m: start-error") +
      // modeString = String(", m: ") + progVars.generatorState +
        String(", t: 0/0");
      break;
    case GENERATOR_STATE_STOP_ERROR:
      // modeString = String(", m:  stop-error, timer: 0");
      modeString = String(", m: stop-error") +
      // modeString = String(", m: ") + progVars.generatorState +
        String(", t: 0/0");
      break;
    default:
      // modeString = String(", m: unknown, time: 0");
      modeString = String(", m: ") + progVars.generatorState + String(", t: 0/0");
      break;
  };
  return String(timestamp) +
    // // Generator state
      modeString +
      // retries
      String(", R: ") +
      String(progVars.numRetries) +
      String("/") +
      String(progVars.maxRetries) +
    // // F(" loggingEnabled: ") + 
    //   String(strSep) +
    //   String(progVars.loggingEnabled) +
  // Process control stages
      // String(F(", Process: ")) +
      String(F(", P: ")) +
    // F(" processControlEnabled: ") + 
      // String(strSep) +
      String(progVars.processControlEnabled) +
    // F(" readInputsEnabled: ") + 
      String(strSep) +
      String(progVars.readInputsEnabled) +
    // F(" calculateStateEnabled: ") + 
      String(strSep) +
      String(progVars.calculateStateEnabled) +
    // F(" checkLimitsEnabled: ") + 
      String(strSep) +
      String(progVars.checkLimitsEnabled) +
    // F(" setOutputsEnabled: ") + 
      String(strSep) +
      String(progVars.setOutputsEnabled) +
  // Generator state vars
      // String(F(", Gen State: ")) +
      String(F(", WE: ")) +
    // // F(" generatorState: ") + 
    //   // String(strSep) +
    //   String(progVars.generatorState) +
    // // F(" generatorOn: ") + 
    //   String(strSep) +
    //   String(progVars.generatorOn) +
  // Error and warning is bitwise
  // start | stop | temperature | fuel | 4 | 5 | 6| 7
    // F(" generatorWarning: ") + 
      // String(strSep) +
      String(progVars.generatorWarning) +
    // F(" generatorError: ") + 
      String(strSep) +
      String(progVars.generatorError) +
  // // Generator start vars
  //   // F(" maxRetries: ") + 
  //     String(strSep) +
  //     String(progVars.maxRetries) +
  //   // F(" numRetries: ") + 
  //     String(strSep) +
  //    String(progVars.numRetries) +
  //   // F(" starterContactorTimeSeconds: ") + 
  //     String(strSep) +
  //    String(progVars.starterContactorTimeSeconds) +
  //   // F(" starterContactorTimeCurrentTicks: ") + 
  //     String(strSep) +
  //    String(progVars.starterContactorTimeCurrentTicks) +
  //   // F(" startToOnTimeSeconds: ") + 
  //     String(strSep) +
  //    String(progVars.startToOnTimeSeconds) +
  //   // F(" startToOnTimeCurrentTicks: ") + 
  //     String(strSep) +
  //     String(progVars.startToOnTimeCurrentTicks) +
  // // Environmental conditions
  //   // F(" checkLevelTemperatureMax: ") + 
  //     String(strSep) +
  //     String(progVars.checkLevelTemperatureMax) +
  //   // F(" checkLevelTemperatureWarning: ") + 
  //     String(strSep) +
  //     String(progVars.checkLevelTemperatureWarning) +
  //   // F(" checkLevelFuelLevelMin: ") + 
  //     String(strSep) +
  //     String(progVars.checkLevelFuelLevelMin) +
  //   // F(" checkLevelFuelLevelWarning: ") + 
  //     String(strSep) +
  //     String(progVars.checkLevelFuelLevelWarning) +
  // Inputs
      // String(F(", Inputs: ")) +
      String(F(", I: ")) +
    // F(" inputRunStop: ") + 
      // String(strSep) +
      String(progVars.inputRunStop) +
    // F(" inpputRunIndicator: ") + 
      String(strSep) +
      String(progVars.inpputRunIndicator) +
    // // F(" inputTemperature: ") + 
    //   String(strSep) +
    //   String(progVars.inputTemperature) +
    // // F(" inputFuelLevel: ") + 
    //   String(strSep) +
    //   String(progVars.inputFuelLevel) +
  // Outputs
      // String(F(", Outputs: ")) +
      String(F(", O: ")) +
    // F(" outputGeneratorIgnition: ") + 
      String(progVars.outputGeneratorIgnition) +
    // F(" outputGeneratorStarterContactor: ") + 
      String(strSep) +
      String(progVars.outputGeneratorStarterContactor) +
    // F(" outputGeneratorOutputEnable: ") + 
      String(strSep) +
      String(progVars.outputGeneratorOutputEnable) +
    // F(" outputGeneratorOutputEnable: ") + 
      String(strSep) +
      String(progVars.outputIndicatorError) ;// +
    // // F(" outputIndicatorWarning: ") + 
    //   String(strSep) +
    //   String(progVars.outputIndicatorWarning) +
    // // F(" outputIndicatorError: ") + 
    //   String(strSep) +
    //   String(progVars.outputIndicatorError) +
    // // F(" outputIndicatorTemperature: ") + 
    //   String(strSep) +
    //   String(progVars.outputIndicatorTemperature) +
    // // F(" outputIndicatorFuelLevel: ") + 
    //   String(strSep) +
    //   String(progVars.outputIndicatorFuelLevel);
    // String(F(",IO: A0: ")) +
    // String(digitalRead(A0));
};


/**
 * Program Control Things
 */

// 8 bit error register
#define WARNING_TEMPERATURE                 0
#define WARNING_FUEL                        1
#define ERROR_GENERATOR_START               2
#define ERROR_GENERATOR_STOP                3
#define ERROR_TEMPERATURE                   4
#define ERROR_FUEL                          5
#define WARNING_THRESHOLD                   3 // If error reg over this is warning

/*****************************************************
 * Process Control
 * 
 * 
 *****************************************************/
bool read_run_stop(uint8_t digitalInputPin, uint16_t debounceNum) {
  static int16_t debounceCount = 0;
  static bool pinState = false;
  static bool returnState = false;

  // Pin is inverting, so we invert at read
  pinState = ! digitalRead(digitalInputPin);
  
  //We debounce checking the debounceCount against the debounceNum,
  // If we are at +debounce number we return true, and if at -debounceNum 
  // then return false, otherwise add for true or subtract if false from
  // debounce count.
  // But.. if we _were_ true and the debounce count falls we cannot
  // transition to false unless the count is at one rail or the other
  if(pinState == true) {
    if(debounceCount >= int16_t(debounceNum)) {
      returnState = true;
    } else {
      debounceCount++;
    }
  } else {
    if(debounceCount <= int16_t(-debounceCount)) {
      returnState = false;
    } else {
      debounceCount--;
    }
  }
  return returnState;
};


bool read_generator_running(uint8_t digitalInputPin, uint16_t debounceNum) {
  static int16_t debounceCount = 0;
  static bool pinState = false;
  static bool returnState = false;

  // Pin is inverting, so we invert at read
  pinState = ! digitalRead(digitalInputPin);

  if(pinState == true) {
    if(debounceCount >= int16_t(debounceNum)) {
      returnState = true;
    } else {
      debounceCount++;
    }
  } else {
    if(debounceCount <= int16_t(-debounceCount)) {
      returnState = false;
    } else {
      debounceCount--;
    }
  }
  return returnState;
};

/**
 * Read inputs and process such that the 'input' parameters are
 * ready for use by the application. i.e witches are debounced,
 * and analogue signals are calibrated/corrected
 */
void read_inputs(void) {

    programVars.inputRunStop = read_run_stop(
      INPUT_PIN_RUN_STOP, programVars.startSwitchDebounceCount);
    programVars.inpputRunIndicator = read_generator_running(
      INPUT_PIN_RUN_INDICATOR, programVars.generatorRunningDebounceCount);
    programVars.inputTemperature = analogRead(INPUT_PIN_ANALOG_TEMPERATURE);
    programVars.inputFuelLevel = analogRead(INPUT_PIN_ANALOG_FUEL_LEVEL);
}

uint16_t secondsToTicks(uint16_t seconds, uint16_t ticksPerCycle) {
  return seconds*(1000.0/ticksPerCycle);
}

/**
 * Calculate the state - i.e start, stop, and manage the generator
 */
void calculate_state(void) {
  // programVars.outputGeneratorStarterContactor = programVars.inputRunStop;
  // programVars.outputGeneratorOutputEnable = programVars.inpputRunIndicator;

  /**
   * Generator starting and stopping logic.
   * Probably need a few functions in here...
   */
  // Lets determine if there is a state change and which one
  // We need to debounce this input somehow. Maybe count up debounce max-> starting and down to 0-> stopping
  // Serial.print("calculating state. ");
  if(programVars.inputRunStop == true){
    // Serial.print("runStop true, ");
    if(programVars.generatorState == GENERATOR_STATE_ON) {
      // Serial.print("generator ON, ");
      // Check whether we are still in a 'running' state.
      // If not, move to retry
      if(programVars.inpputRunIndicator == false) {
        programVars.outputGeneratorOutputEnable = false;
        programVars.generatorState = GENERATOR_STATE_RETRY_WAIT;
      }
    } else if(programVars.generatorState == GENERATOR_STATE_RUNNING) {
      // Serial.print("Generator Running, ");
      // We need to move from Running to On, which means waiting the
      // required number of seconds
      // TODO: Can we set the X_TIME_MILLIS context so this is not hard coded to MEDIUM?
      if(programVars.startToOnTimeCurrentTicks >= secondsToTicks(programVars.startToOnTimeSeconds, MEDIUM_TIME_MILLIS)){
        // Sweet, the generator has been on long enough, can set the state to ON
        programVars.startToOnTimeCurrentTicks = 0;
        programVars.outputGeneratorOutputEnable = true;
        programVars.generatorState = GENERATOR_STATE_ON;
      } else {
        // We have not quite waited long enough
        programVars.startToOnTimeCurrentTicks++;
        // Let us be safe and ensure that the output is disabled
        programVars.outputGeneratorOutputEnable = false;
      }
    } else if(programVars.generatorState == GENERATOR_STATE_START_CHECK) {
      // We want to ensure that the generator is reporting a 'running' state for a while, and at least the right number of times
      if(programVars.checkTimeCurrentTicks >= secondsToTicks(programVars.checkTimeSeconds, MEDIUM_TIME_MILLIS)) {
        // If the generator is in reporting running, then let us move to running, otherwise retry/wait
        if(programVars.inpputRunIndicator == true){
          // Good, the generator is reporting a running state, we can begin counting down to enabling the output
          programVars.checkTimeCurrentTicks = 0;
          programVars.generatorState = GENERATOR_STATE_RUNNING;
        } else {
          // Oh dear, we need to retry starting the generator
          programVars.checkTimeCurrentTicks = 0;
          // We set the retry count here!
          programVars.numRetries++;
          programVars.outputGeneratorIgnition = false;
          programVars.generatorState = GENERATOR_STATE_RETRY_WAIT;
        }
      } else {
        // Incriment the count and check the output debounce
        programVars.checkTimeCurrentTicks++;
      }
    } else if(programVars.generatorState == GENERATOR_STATE_STARTING) {
      // We have got to keep that contactor on for the required time period
      if(programVars.starterContactorTimeCurrentTicks >= secondsToTicks(programVars.starterContactorTimeSeconds, MEDIUM_TIME_MILLIS)) {
        // Sweet, the contactor has been on long enough
        programVars.starterContactorTimeCurrentTicks = 0;
        programVars.outputGeneratorStarterContactor = false;
        programVars.outputGeneratorIgnition = true;
        programVars.generatorState = GENERATOR_STATE_START_CHECK;
      } else {
        // Lets keep the contactor on
        programVars.starterContactorTimeCurrentTicks++;
        programVars.outputGeneratorStarterContactor = true;
        programVars.outputGeneratorIgnition = true;
      }
    } else if(
      programVars.generatorState == GENERATOR_STATE_STOPPING ||
      programVars.generatorState == GENERATOR_STATE_OFF) {
        // Serial.print("setting to Starting, ");
        programVars.startSwitchDebounceCurrentTicks = 0;
        programVars.starterContactorTimeCurrentTicks = 0;
        programVars.startToOnTimeCurrentTicks = 0;
        programVars.outputGeneratorIgnition = true;
        programVars.generatorState = GENERATOR_STATE_STARTING;
    } else if(programVars.generatorState == GENERATOR_STATE_RETRY_WAIT) {
      // If the maximum number of retries is exceeded, then we shutdown and go to error state
      if(programVars.numRetries >= programVars.maxRetries) {
        programVars.numRetries = 0;
        programVars.generatorState = GENERATOR_STATE_START_ERROR;
        // Lets turn off the outputs just to be sure
        programVars.outputGeneratorStarterContactor = false;
        programVars.outputGeneratorOutputEnable = false;
      } else if(programVars.retryWaitTimeCurrentTicks >= secondsToTicks(programVars.retryWaitTimeSeconds, MEDIUM_TIME_MILLIS)){
        // Lets try starting again
        programVars.retryWaitTimeCurrentTicks = 0;
        programVars.generatorState = GENERATOR_STATE_STARTING;
      } else {
        // Incriment that timer
        programVars.retryWaitTimeCurrentTicks++;
      }
    } else if(
      programVars.generatorState == GENERATOR_STATE_START_ERROR ||
      programVars.generatorState == GENERATOR_STATE_STOP_ERROR) {
      // Reset all the counters, and... do nothing as we are in error and
      // it is not the role of this process to correct it.
      programVars.outputGeneratorIgnition = false;
      programVars.outputGeneratorStarterContactor = false;
      programVars.outputGeneratorOutputEnable = false;
      programVars.startSwitchDebounceCurrentTicks = 0;
      programVars.starterContactorTimeCurrentTicks = 0;
      programVars.startToOnTimeCurrentTicks = 0;
    }
  } else {
    // The run switch is off
    if (programVars.generatorState == GENERATOR_STATE_OFF) {
      // Do nothing
      // If the generator is running, issue an error
      if(programVars.inpputRunIndicator == true){
        programVars.generatorState = GENERATOR_STATE_STOP_ERROR;
      }
    } else if (
        programVars.generatorState == GENERATOR_STATE_RETRY_WAIT ||
        programVars.generatorState == GENERATOR_STATE_STARTING ||
        programVars.generatorState == GENERATOR_STATE_START_CHECK ||
        programVars.generatorState == GENERATOR_STATE_RUNNING ||
        programVars.generatorState == GENERATOR_STATE_ON) {
      // Let us stop the generator
      programVars.outputGeneratorIgnition = false;
      programVars.outputGeneratorStarterContactor = false;
      programVars.outputGeneratorOutputEnable = false;
      programVars.checkTimeCurrentTicks = 0;
      programVars.generatorState = GENERATOR_STATE_STOPPING;
    } else if(programVars.generatorState == GENERATOR_STATE_STOPPING) {
      // Use the check time and check if it is running or not given debounce
      if(programVars.checkTimeCurrentTicks >= secondsToTicks(programVars.checkTimeSeconds, MEDIUM_TIME_MILLIS)) {
        // Let us check how many times the input was set off, i.e debounced
        if(programVars.inpputRunIndicator == false){
          // Good, the generator is reporting a not running state
          programVars.checkTimeCurrentTicks = 0;
          programVars.generatorState = GENERATOR_STATE_OFF;
        } else {
          // Oh dear, the generator is not turning off! faark
          programVars.checkTimeCurrentTicks = 0;
          programVars.generatorState = GENERATOR_STATE_STOP_ERROR;
        }
      } else {
        // Incriment the count and check the output debounce
        programVars.checkTimeCurrentTicks++;
      }
    }
  }
}

void check_limits(void) {
  // Simply raise the error output for now.
  // You must reset to break out of error
  if(
      programVars.generatorState == GENERATOR_STATE_START_ERROR || 
      programVars.generatorState == GENERATOR_STATE_STOP_ERROR) {
    programVars.outputIndicatorError = true;
  }
}

void set_outputs(void) {
  digitalWrite(OUTPUT_PIN_GENERATOR_IGNITION, programVars.outputGeneratorIgnition);
  digitalWrite(OUTPUT_PIN_GENERATOR_STARTER_CONTACTOR, programVars.outputGeneratorStarterContactor);
  digitalWrite(OUTPUT_PIN_GENERATOR_OUTPUT_ENABLED, programVars.outputGeneratorOutputEnable);
  digitalWrite(OUTPUT_PIN_INDICATOR_ERROR, programVars.outputIndicatorError);
}


void process(void) {
  if(programVars.processControlEnabled == true) {
    // Read inputs
    if (programVars.readInputsEnabled) {
      read_inputs();
    }

    // Calculate state
    if (programVars.calculateStateEnabled) {
      calculate_state();
    }

    // Check Limits
    if (programVars.checkLimitsEnabled) {
      check_limits();
    }

    // Set outputs
    if (programVars.setOutputsEnabled) {
      set_outputs();
    }
  }
}



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
  handle_serial_input();

  // Def once per second
  // print logging info if enabled
  if (programVars.loggingEnabled == true) {
  // if (programVars.inputRunStop == true) {
  // if (digitalRead(INPUT_PIN_RUN_STOP) == true) {
    String logMessage = formatProgVars(timestamp, programVars);    
    Serial.println(logMessage);
  }

  // Toggle the LED
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

// We have to wrap the non-static member function to call in our
// callback. Just 'cause 8(
void run_process(void){
  // process.process();
  process();
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
  // process.begin();
  pinMode(INPUT_PIN_RUN_STOP, INPUT);
  pinMode(INPUT_PIN_RUN_INDICATOR, INPUT);
  pinMode(INPUT_PIN_ANALOG_TEMPERATURE, INPUT);
  pinMode(INPUT_PIN_ANALOG_FUEL_LEVEL, INPUT);
  //
  pinMode(OUTPUT_PIN_GENERATOR_IGNITION, OUTPUT);
  pinMode(OUTPUT_PIN_GENERATOR_STARTER_CONTACTOR, OUTPUT);
  pinMode(OUTPUT_PIN_GENERATOR_OUTPUT_ENABLED, OUTPUT);
  pinMode(OUTPUT_PIN_INDICATOR_ERROR, OUTPUT);
  pinMode(OUTPUT_PIN_INDICATOR_WARNING, OUTPUT);
  pinMode(OUTPUT_PIN_INDICATOR_ERROR, OUTPUT);
  pinMode(OUTPUT_PIN_INDICATOR_TEMPERATURE, OUTPUT);
  pinMode(OUTPUT_PIN_INDICATOR_FUEL, OUTPUT);

  
  // Dummy LED thingee
  pinMode(LED_BUILTIN, OUTPUT);

  // Register the timed functions
  // Maybe do this as part of class instantiation
  timer_set_long_time_callback(&run_terminal_toggle_led);
  timer_set_medium_time_callback(&run_process);
}

void loop() {
  // Run timed processes
  timer_run_timed_functions();
}