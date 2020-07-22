#include <Arduino.h>
#include "Terminal.h"
// We need to include the 'cpp' due to the templating
#include "Terminal.cpp"

/** Default variable settings **/
// IO
#define RUN_STOP_INPUT_PIN 0
#define RUN_INDICATOR_INPUT_PIN 1
#define TEMPERATURE_INPUT_PIN 3
// #
// Program state varse
#define LOGGING_ENABLED_DEFAULT 1

// This variables are 'volatile' as it changes in an ISR
volatile uint32_t timestamp = 0;
volatile bool fastTimeFlag = 0;
volatile bool mediumTimeFlag = 0;
volatile bool longTimeFlag = 0;

// The number of milliseconds in each time period
#define FAST_TIME_MILLIS 10
#define MEDIUM_TIME_MILLIS 100
#define LONG_TIME_MILLIS 1000

// Serial buffer and message
String serialBuffer = "";
String messages = "";

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
 * A struct containing all the important settable and gettable program vars
 * With serial string prefix
 */
struct ProgramVars {
  // has a state changed?
  bool stateChange;
  // is logging enabled
  bool loggingEnabled;                               // L
  // Generator state vars
  bool generatorRunning;                             // r
  bool generatorOn;                                  // o
  // Error and warning is bitwise
  // start | stop | temperature | fuel | 4 | 5 | 6| 7
  uint8_t generatorWarning;                             // w
  uint8_t generatorError;                               // e
  // Generator start vars
  uint8_t maxRetries;                                   // S
  uint8_t numRetries;                                   // s
  uint16_t starterContactorTimeSeconds;                 // C
  uint16_t starterContactorTimeSecondsCurrent;          // c
  uint16_t startToOnTimeSeconds;                        // V
  uint16_t startToOnTimeSecondsCurrent;                 // v
  // Environmental conditions
  float checkLevelTemperatureMax;                       // T
  float checkLevelTemperatureWarning;                   // t
  float checkLevelFuelLevelMin;                         // F
  float checkLevelFuelLevelWarning;                     // f
  // Inputs
  bool inputRunStop;                                 // R
  bool inpputRunIndicator;                           // i
  float   inputTemperature;                             // y
  float   inputFuelLevel;                               // u
  // Outputs
  bool outputGeneratorStarterContactor;              // b
  bool outputGeneratorOutputEnable;                  // n
  bool outputIndicatorWarning;                       // m
  bool outputIndicatorError;                         // ,
  bool outputIndicatorTemperature;                   // .
  bool outputIndicatorFuelLevel;                     // /
};
// Insantiate an empty version
ProgramVars programVars = {
  LOGGING_ENABLED_DEFAULT,	// loggingEnabled
  // Generator state vars
  0,	// generatorRunning
  0,	// generatorOn
  // Error and warning is bitwise
  0,	// generatorWarning
  0,	// generatorError
  // Generator start vars
  0,	// maxRetries
  0,	// numRetries
  0,	// starterContactorTimeSeconds
  0,	// starterContactorTimeSecondsCurrent
  0,	// startToOnTimeSeconds
  0,	// startToOnTimeSecondsCurrent
  // Environmental conditions
  0,	// checkLevelTemperatureMax
  0,	// checkLevelTemperatureWarning
  0,	// checkLevelFuelLevelMin
  0,	// checkLevelFuelLevelWarning
  // Inputs
  0,	// inputRunStop
  0,	// inpputRunIndicator
  0,	// inputTemperature
  0,	// inputFuelLevel
  // Outputs
  0,	// outputGeneratorStarterContactor
  0,	// outputGeneratorOutputEnable
  0,	// outputIndicatorWarning
  0,	// outputIndicatorError
  0,	// outputIndicatorTemperature
  0	// outputIndicatorFuelLevel
};


/**
 * The 'Terminal' requires us to write concrete methods for a concrete return type
 */
class CatFrogTerminal: Terminal<ProgramVars>{
  public:
    CatFrogTerminal(): Terminal()
    {
    }

    /**
     * Process the UART commands, updating the variables or producing a string message
     */
    int processCommands(String inputString, ProgramVars *progVars, String *message) {
      // Parse the 'inputString'
      CommandAndArguments comArgState = parseCommandArgs(inputString);

      // Exit with message if no command
      if (comArgState.parseState == EXIT_FAILURE) {
        *message = "Input string is not a valid command/argument";
        return EXIT_FAILURE;
      }

      // Let us process the commands
      switch (comArgState.command)
      {
      case 'h':
        progVars->stateChange = false;
  //       *message = String("Help: \n") + 
  //         String(F("'L': loggingEnabled\n")) +
  // // Generator state vars
  //         String("'r': generatorRunning\n") +
  //         String("'o': generatorOn\n");
        *message = String(F("Help: \n")) + 
          String(F("'L': loggingEnabled\n")) +
  // Generator state vars
          String(F("'r': generatorRunning\n")) +
          String(F("'o': generatorOn\n")) +
  // Error and warning is bitwise
          String(F("'w': generatorWarning\n")) +
          String(F("'e': generatorError\n")) +
  // Generator start vars
          String(F("'S': maxRetries\n")) +
          String(F("'s': numRetries\n")) +
          String(F("'C': starterContactorTimeSeconds\n")) +
          String(F("'c': starterContactorTimeSecondsCurrent\n")) +
          String(F("'V': startToOnTimeSeconds\n")) +
          String(F("'v': startToOnTimeSecondsCurrent\n")) +
  // Environmental conditions
          String(F("'T': checkLevelTemperatureMax\n")) +
          String(F("'t': checkLevelTemperatureWarning\n")) +
          String(F("'F': checkLevelFuelLevelMin\n")) +
          String(F("'f': checkLevelFuelLevelWarning\n")) +
  // Inputs
          String(F("'R': inputRunStop\n")) +
          String(F("'i': inpputRunIndicator\n")) +
          String(F("'y': inputTemperature\n")) +
          String(F("'u': inputFuelLevel\n")) +
  // Outputs
          String(F("'b': outputGeneratorStarterContactor\n")) +
          String(F("'n': outputGeneratorOutputEnable\n")) +
          String(F("'m': outputIndicatorWarning\n")) +
          String(F("',': outputIndicatorError\n")) +
          String(F("'.': outputIndicatorTemperature\n")) +
          String(F("'/': outputIndicatorFuelLevel\n"));
        break;
      case 'L':
        progVars->stateChange = argDisplayOrSetBoolean("loggingEnabled", comArgState, &progVars->loggingEnabled, message);
        break;
  // Generator state vars
      case 'r':
        progVars->stateChange = argDisplayOrSetBoolean("generatorRunning", comArgState, &progVars->generatorRunning, message);
        break;
      case 'o':
        progVars->stateChange = argDisplayOrSetBoolean("generatorOn", comArgState, &progVars->generatorOn, message);
        break;
  // Error and warning is bitwise
  // start | stop | temperature | fuel | 4 | 5 | 6| 7
      case 'w':
        progVars->stateChange = argDisplayOrSetUint8("generatorWarning", comArgState, &progVars->generatorWarning, message);
        break;
      case 'e':
        progVars->stateChange = argDisplayOrSetUint8("generatorError", comArgState, &progVars->generatorError, message);
        break;
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
      case 'c':
        progVars->stateChange = argDisplayOrSetUint16("starterContactorTimeSecondsCurrent", comArgState, &progVars->starterContactorTimeSecondsCurrent, message);
        break;
      case 'V':
        progVars->stateChange = argDisplayOrSetUint16("startToOnTimeSeconds", comArgState, &progVars->startToOnTimeSeconds, message);
        break;
      case 'v':
        progVars->stateChange = argDisplayOrSetUint16("startToOnTimeSecondsCurrent", comArgState, &progVars->startToOnTimeSecondsCurrent, message);
        break;
  // Environmental conditions
      case 'T':
        progVars->stateChange = argDisplayOrSetFloatFromLong("checkLevelTemperatureMax", comArgState, &progVars->checkLevelTemperatureMax, 100, message);
        break;
      case 't':
        progVars->stateChange = argDisplayOrSetFloatFromLong("checkLevelTemperatureWarning", comArgState, &progVars->checkLevelTemperatureWarning, 100, message);
        break;
      case 'F':
        progVars->stateChange = argDisplayOrSetFloatFromLong("checkLevelFuelLevelMin", comArgState, &progVars->checkLevelFuelLevelMin, 100, message);
        break;
      case 'f':
        progVars->stateChange = argDisplayOrSetFloatFromLong("checkLevelFuelLevelWarning", comArgState, &progVars->checkLevelFuelLevelWarning, 100, message);
        break;
  // Inputs
      case 'R':
        progVars->stateChange = argDisplayOrSetBoolean("inputRunStop", comArgState, &progVars->inputRunStop, message);
        break;
      case 'i':
        progVars->stateChange = argDisplayOrSetBoolean("inpputRunIndicator", comArgState, &progVars->inpputRunIndicator, message);
        break;
      case 'y':
        progVars->stateChange = argDisplayOrSetFloatFromLong("inputTemperature", comArgState, &progVars->inputTemperature, 100, message);
        break;
      case 'u':
        progVars->stateChange = argDisplayOrSetFloatFromLong("inputFuelLevel", comArgState, &progVars->inputFuelLevel, 100, message);
        break;
  // Outputs
      case 'b':
        progVars->stateChange = argDisplayOrSetBoolean("outputGeneratorStarterContactor", comArgState, &progVars->outputGeneratorStarterContactor, message);
        break;
      case 'n':
        progVars->stateChange = argDisplayOrSetBoolean("outputGeneratorOutputEnable", comArgState, &progVars->outputGeneratorOutputEnable, message);
        break;
      case 'm':
        progVars->stateChange = argDisplayOrSetBoolean("outputIndicatorWarning", comArgState, &progVars->outputIndicatorWarning, message);
        break;
      case ',':
        progVars->stateChange = argDisplayOrSetBoolean("outputIndicatorError", comArgState, &progVars->outputIndicatorError, message);
        break;
      case '.':
        progVars->stateChange = argDisplayOrSetBoolean("outputIndicatorTemperature", comArgState, &progVars->outputIndicatorTemperature, message);
        break;
      case '/':
        progVars->stateChange = argDisplayOrSetBoolean("outputIndicatorFuelLevel", comArgState, &progVars->outputIndicatorFuelLevel, message);
        break;
      default:
        progVars->stateChange = false;
        *message = "No recognised command";
        break;
      }
      return EXIT_SUCCESS;
      };


      String formatProgVars(long time, ProgramVars progVars) {
        return String(timestamp) +
          F(" loggingEnabled: ") + String(progVars.loggingEnabled) +
        // Generator state vars
          F(" generatorRunning: ") + String(progVars.generatorRunning) +
          F(" generatorOn: ") + String(progVars.generatorOn) +
        // Error and warning is bitwise
        // start | stop | temperature | fuel | 4 | 5 | 6| 7
          F(" generatorWarning: ") + String(progVars.generatorWarning) +
          F(" generatorError: ") + String(progVars.generatorError) +
        // Generator start vars
          F(" maxRetries: ") + String(progVars.maxRetries) +
          F(" numRetries: ") + String(progVars.numRetries) +
          F(" starterContactorTimeSeconds: ") + String(progVars.starterContactorTimeSeconds) +
          F(" starterContactorTimeSecondsCurrent: ") + String(progVars.starterContactorTimeSecondsCurrent) +
          F(" startToOnTimeSeconds: ") + String(progVars.startToOnTimeSeconds) +
          F(" startToOnTimeSecondsCurrent: ") + String(progVars.startToOnTimeSecondsCurrent) +
        // Environmental conditions
          F(" checkLevelTemperatureMax: ") + String(progVars.checkLevelTemperatureMax) +
          F(" checkLevelTemperatureWarning: ") + String(progVars.checkLevelTemperatureWarning) +
          F(" checkLevelFuelLevelMin: ") + String(progVars.checkLevelFuelLevelMin) +
          F(" checkLevelFuelLevelWarning: ") + String(progVars.checkLevelFuelLevelWarning) +
        // Inputs
          F(" inputRunStop: ") + String(progVars.inputRunStop) +
          F(" inpputRunIndicator: ") + String(progVars.inpputRunIndicator) +
          F(" inputTemperature: ") + String(progVars.inputTemperature) +
          F(" inputFuelLevel: ") + String(progVars.inputFuelLevel) +
        // Outputs
          F(" outputGeneratorStarterContactor: ") + String(progVars.outputGeneratorStarterContactor) +
          F(" outputGeneratorOutputEnable: ") + String(progVars.outputGeneratorOutputEnable) +
          F(" outputIndicatorWarning: ") + String(progVars.outputIndicatorWarning) +
          F(" outputIndicatorError: ") + String(progVars.outputIndicatorError) +
          F(" outputIndicatorTemperature: ") + String(progVars.outputIndicatorTemperature) +
          F(" outputIndicatorFuelLevel: ") + String(progVars.outputIndicatorFuelLevel);
      };
};

// Instantiate the terminal
CatFrogTerminal terminal;


void setup() {
  // put your setup code here, to run once:

  // Timer0 is already used for millis() - we'll just interrupt somewhere
  // in the middle and call the "Compare A" function below
  OCR0A = 0xAF;
  TIMSK0 |= _BV(OCIE0A);

  // Initialise the serial hardware at baude 115200
  Serial.begin(115200);
  
  // Dummy LED thingee
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  // If medium timer has fired, do medium stuff
  if(mediumTimeFlag == true) {
    mediumTimeFlag = false;

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
      // Serial.println(serialBuffer);
      // Process the commands
      terminal.processCommands(serialBuffer, &programVars, &messages);
      // Print the message
      Serial.println(messages);
      // Reset the buffer to empty
      serialBuffer = "";
    }
  }

  // If the Long timer has fired, do Long stuff
  if(longTimeFlag == true) {
    longTimeFlag = false;

    // print logging info if enabled
    if (programVars.loggingEnabled == true) {
      String logMessage = terminal.formatProgVars(timestamp, programVars);    
      Serial.println(logMessage);
    }

    // Toggle the LED
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}