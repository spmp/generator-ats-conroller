#include "MyTerminal.h"

/**
 * Constructor
 */
MyTerminal::MyTerminal(Stream & Serial, ProgramVars *programVars): Terminal(Serial, programVars) {}

/**
 * Deconstructor
 */
MyTerminal::~MyTerminal() {};


/**
 * Process the UART commands, updating the variables or producing a string message
 */
void MyTerminal::getAndSetProgramVars(CommandAndArguments comArgState, String *message) {

  // Let us process the commands
  switch (comArgState.command)
  {
  case 'h':
    _progVars->stateChange = false;
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
    _progVars->stateChange = argDisplayOrSetBoolean("loggingEnabled", comArgState, &_progVars->loggingEnabled, message);
    break;
  // Process control stages
  case 'P':
    _progVars->stateChange = argDisplayOrSetBoolean("processControlEnabled", comArgState, &_progVars->processControlEnabled, message);
    break;
  case 'I':
    _progVars->stateChange = argDisplayOrSetBoolean("readInputsEnabled", comArgState, &_progVars->readInputsEnabled, message);
    break;
  case 'Y':
    _progVars->stateChange = argDisplayOrSetBoolean("calculateStateEnabled", comArgState, &_progVars->calculateStateEnabled, message);
    break;
  case 'U':
    _progVars->stateChange = argDisplayOrSetBoolean("checkLimitsEnabled", comArgState, &_progVars->checkLimitsEnabled, message);
    break;
  case 'O':
    _progVars->stateChange = argDisplayOrSetBoolean("setOutputsEnabled", comArgState, &_progVars->setOutputsEnabled, message);
    break;
  // Generator state vars
  case 'r':
    _progVars->stateChange = argDisplayOrSetUint16("generatorState", comArgState, &_progVars->generatorState, message);
    break;
  // case 'o':
  //   _progVars->stateChange = argDisplayOrSetBoolean("generatorOn", comArgState, &_progVars->generatorOn, message);
  //   break;
  // Error and warning is bitwise
  // start | stop | temperature | fuel | 4 | 5 | 6| 7
  // case 'w':
  //   _progVars->stateChange = argDisplayOrSetUint8("generatorWarning", comArgState, &_progVars->generatorWarning, message);
  //   break;
  // case 'e':
  //   _progVars->stateChange = argDisplayOrSetUint8("generatorError", comArgState, &_progVars->generatorError, message);
  //   break;
  // Generator start vars
  case 'S':
    _progVars->stateChange = argDisplayOrSetUint8("maxRetries", comArgState, &_progVars->maxRetries, message);
    break;
  case 's':
    _progVars->stateChange = argDisplayOrSetUint8("numRetries", comArgState, &_progVars->numRetries, message);
    break;
  case 'C':
    _progVars->stateChange = argDisplayOrSetUint16("starterContactorTimeSeconds", comArgState, &_progVars->starterContactorTimeSeconds, message);
    break;
  // case 'XX':
  //   _progVars->stateChange = argDisplayOrSetUint16("starterContactorTimeCurrentTicks", comArgState, &_progVars->starterContactorTimeCurrentTicks, message);
  //   break;
  case 'c':
    _progVars->stateChange = argDisplayOrSetUint16("checkTimeSeconds", comArgState, &_progVars->checkTimeSeconds, message);
    break;
  case 'D':
    _progVars->stateChange = argDisplayOrSetUint16("generatorRunningDebounceCount", comArgState, &_progVars->generatorRunningDebounceCount, message);
    break;
  case 'd':
    _progVars->stateChange = argDisplayOrSetUint16("startSwitchDebounceCount", comArgState, &_progVars->startSwitchDebounceCount, message);
    break;
  case 'V':
    _progVars->stateChange = argDisplayOrSetUint16("startToOnTimeSeconds", comArgState, &_progVars->startToOnTimeSeconds, message);
    break;
  case 'N':
    _progVars->stateChange = argDisplayOrSetUint16("retryWaitTimeSeconds", comArgState, &_progVars->retryWaitTimeSeconds, message);
    break;
  // case 'v':
  //   _progVars->stateChange = argDisplayOrSetUint16("startToOnTimeCurrentTicks", comArgState, &_progVars->startToOnTimeCurrentTicks, message);
  //   break;
  // // Environmental conditions
  // case 'T':
  //   _progVars->stateChange = argDisplayOrSetFloatFromLong("checkLevelTemperatureMax", comArgState, &_progVars->checkLevelTemperatureMax, 100, message);
  //   break;
  // case 't':
  //   _progVars->stateChange = argDisplayOrSetFloatFromLong("checkLevelTemperatureWarning", comArgState, &_progVars->checkLevelTemperatureWarning, 100, message);
  //   break;
  // case 'F':
  //   _progVars->stateChange = argDisplayOrSetFloatFromLong("checkLevelFuelLevelMin", comArgState, &_progVars->checkLevelFuelLevelMin, 100, message);
  //   break;
  // case 'f':
  //   _progVars->stateChange = argDisplayOrSetFloatFromLong("checkLevelFuelLevelWarning", comArgState, &_progVars->checkLevelFuelLevelWarning, 100, message);
  //   break;
  // Inputs
  case 'R':
    _progVars->stateChange = argDisplayOrSetBoolean("inputRunStop", comArgState, &_progVars->inputRunStop, message);
    break;
  case 'i':
    _progVars->stateChange = argDisplayOrSetBoolean("inpputRunIndicator", comArgState, &_progVars->inpputRunIndicator, message);
    break;
  // case 'y':
  //   _progVars->stateChange = argDisplayOrSetFloatFromLong("inputTemperature", comArgState, &_progVars->inputTemperature, 100, message);
  //   break;
  // case 'u':
  //   _progVars->stateChange = argDisplayOrSetFloatFromLong("inputFuelLevel", comArgState, &_progVars->inputFuelLevel, 100, message);
  //   break;
  // Outputs
  // case 'b':
  //   _progVars->stateChange = argDisplayOrSetBoolean("outputGeneratorIgnition", comArgState, &_progVars->outputGeneratorIgnition, message);
  //   break;
  // case 'n':
  //   _progVars->stateChange = argDisplayOrSetBoolean("outputGeneratorStarterContactor", comArgState, &_progVars->outputGeneratorStarterContactor, message);
  //   break;
  // case 'm':
  //   _progVars->stateChange = argDisplayOrSetBoolean("outputGeneratorOutputEnable", comArgState, &_progVars->outputGeneratorOutputEnable, message);
  //   break;
  // case ',':
  //   _progVars->stateChange = argDisplayOrSetBoolean("outputIndicatorWarning", comArgState, &_progVars->outputIndicatorWarning, message);
  //   break;
  case '.':
    _progVars->stateChange = argDisplayOrSetBoolean("outputIndicatorError", comArgState, &_progVars->outputIndicatorError, message);
    break;
  // case '/':
  //   _progVars->stateChange = argDisplayOrSetBoolean("outputIndicatorTemperature", comArgState, &_progVars->outputIndicatorTemperature, message);
  //   break;
  // case '?':
  //   _progVars->stateChange = argDisplayOrSetBoolean("outputIndicatorFuelLevel", comArgState, &_progVars->outputIndicatorFuelLevel, message);
  //   break;
  default:
    _progVars->stateChange = false;
    *message = "No recognised command";
    break;
  }
};


String MyTerminal::formatProgVars(long time) {
  // Mode specific parts
  String modeString =  String("");
  switch(_progVars->generatorState) {
    case GENERATOR_STATE_OFF:
      // modeString = String(", m: off, timer: 0");
      modeString = String(", m: off") +
      // modeString = String(", m: ") + _progVars->generatorState +
        String(", t: 0/0");
      break;
    case GENERATOR_STATE_ON:
      // modeString = String(", m:  on, timer: 0");
      modeString = String(", m: on") +
      // modeString = String(", m: ") + _progVars->generatorState +
        String(", t: 0/0");
      break;
    case GENERATOR_STATE_STARTING:
      // modeString = String(", m:  starting, timer: ") + String(_progVars->starterContactorTimeCurrentTicks/MEDIUM_TIME_MILLIS);
      modeString = String(", m: starting") +
      // modeString = String(", m: ") + _progVars->generatorState + 
        String(", t: ") + 
        _progVars->starterContactorTimeCurrentTicks/MEDIUM_TIME_MILLIS + "/" + _progVars->starterContactorTimeSeconds;
      break;
    case GENERATOR_STATE_STOPPING:
      // modeString = String(", m:  stopping, timer: ") + String(_progVars->starterContactorTimeCurrentTicks/MEDIUM_TIME_MILLIS);
      modeString = String(", m: stopping") +
      // modeString = String(", m: ") + _progVars->generatorState +
        String(", t: ") + 
        String(_progVars->starterContactorTimeCurrentTicks/MEDIUM_TIME_MILLIS) + "/" + _progVars->starterContactorTimeSeconds;
      break;
    case GENERATOR_STATE_START_CHECK:
      // modeString = String(", m:  start-check, timer: ") + String(_progVars->checkTimeCurrentTicks/MEDIUM_TIME_MILLIS);
      modeString = String(", m: start-check") +
      // modeString = String(", m: ") + _progVars->generatorState +
        String(", t: ") +
        String(_progVars->checkTimeCurrentTicks/MEDIUM_TIME_MILLIS) + "/" + _progVars->checkTimeSeconds;
      break;
    case GENERATOR_STATE_RUNNING:
      // modeString = String(", m:  running, timer: ") + String(_progVars->startToOnTimeCurrentTicks/MEDIUM_TIME_MILLIS);
      modeString = String(", m: running") +
      // modeString = String(", m: ") + _progVars->generatorState +
        String(", t: ") + 
        String(_progVars->startToOnTimeCurrentTicks/MEDIUM_TIME_MILLIS)+ "/" + _progVars->startToOnTimeSeconds;
      break;
    case GENERATOR_STATE_RETRY_WAIT:
      // modeString = String(", m:  retrying, timer: ") + String(_progVars->retryWaitTimeCurrentTicks/MEDIUM_TIME_MILLIS);
      modeString = String(", m: retry-wait") +
      // modeString = String(", m: ") + _progVars->generatorState +
        String(", t: ") +
        String(_progVars->retryWaitTimeCurrentTicks/MEDIUM_TIME_MILLIS) + "/" + _progVars->retryWaitTimeSeconds;
      break;
    case GENERATOR_STATE_START_ERROR:
      // modeString = String(", m:  start-error, timer: 0");
      modeString = String(", m: start-error") +
      // modeString = String(", m: ") + _progVars->generatorState +
        String(", t: 0/0");
      break;
    case GENERATOR_STATE_STOP_ERROR:
      // modeString = String(", m:  stop-error, timer: 0");
      modeString = String(", m: stop-error") +
      // modeString = String(", m: ") + _progVars->generatorState +
        String(", t: 0/0");
      break;
    default:
      // modeString = String(", m: unknown, time: 0");
      modeString = String(", m: ") + _progVars->generatorState + String(", t: 0/0");
      break;
  };
  return String(time) +
    // // Generator state
      modeString +
      // retries
      String(", R: ") +
      String(_progVars->numRetries) +
      String("/") +
      String(_progVars->maxRetries) +
    // // F(" loggingEnabled: ") + 
    //   String(strSep) +
    //   String(_progVars->loggingEnabled) +
  // Process control stages
      // String(F(", Process: ")) +
      String(F(", P: ")) +
    // F(" processControlEnabled: ") + 
      // String(strSep) +
      String(_progVars->processControlEnabled) +
    // F(" readInputsEnabled: ") + 
      String(strSep) +
      String(_progVars->readInputsEnabled) +
    // F(" calculateStateEnabled: ") + 
      String(strSep) +
      String(_progVars->calculateStateEnabled) +
    // F(" checkLimitsEnabled: ") + 
      String(strSep) +
      String(_progVars->checkLimitsEnabled) +
    // F(" setOutputsEnabled: ") + 
      String(strSep) +
      String(_progVars->setOutputsEnabled) +
  // Generator state vars
      // String(F(", Gen State: ")) +
      String(F(", WE: ")) +
    // // F(" generatorState: ") + 
    //   // String(strSep) +
    //   String(_progVars->generatorState) +
    // // F(" generatorOn: ") + 
    //   String(strSep) +
    //   String(_progVars->generatorOn) +
  // Error and warning is bitwise
  // start | stop | temperature | fuel | 4 | 5 | 6| 7
    // F(" generatorWarning: ") + 
      // String(strSep) +
      String(_progVars->generatorWarning) +
    // F(" generatorError: ") + 
      String(strSep) +
      String(_progVars->generatorError) +
  // // Generator start vars
  //   // F(" maxRetries: ") + 
  //     String(strSep) +
  //     String(_progVars->maxRetries) +
  //   // F(" numRetries: ") + 
  //     String(strSep) +
  //    String(_progVars->numRetries) +
  //   // F(" starterContactorTimeSeconds: ") + 
  //     String(strSep) +
  //    String(_progVars->starterContactorTimeSeconds) +
  //   // F(" starterContactorTimeCurrentTicks: ") + 
  //     String(strSep) +
  //    String(_progVars->starterContactorTimeCurrentTicks) +
  //   // F(" startToOnTimeSeconds: ") + 
  //     String(strSep) +
  //    String(_progVars->startToOnTimeSeconds) +
  //   // F(" startToOnTimeCurrentTicks: ") + 
  //     String(strSep) +
  //     String(_progVars->startToOnTimeCurrentTicks) +
  // // Environmental conditions
  //   // F(" checkLevelTemperatureMax: ") + 
  //     String(strSep) +
  //     String(_progVars->checkLevelTemperatureMax) +
  //   // F(" checkLevelTemperatureWarning: ") + 
  //     String(strSep) +
  //     String(_progVars->checkLevelTemperatureWarning) +
  //   // F(" checkLevelFuelLevelMin: ") + 
  //     String(strSep) +
  //     String(_progVars->checkLevelFuelLevelMin) +
  //   // F(" checkLevelFuelLevelWarning: ") + 
  //     String(strSep) +
  //     String(_progVars->checkLevelFuelLevelWarning) +
  // Inputs
      // String(F(", Inputs: ")) +
      String(F(", I: ")) +
    // F(" inputRunStop: ") + 
      // String(strSep) +
      String(_progVars->inputRunStop) +
    // F(" inpputRunIndicator: ") + 
      String(strSep) +
      String(_progVars->inpputRunIndicator) +
    // // F(" inputTemperature: ") + 
    //   String(strSep) +
    //   String(_progVars->inputTemperature) +
    // // F(" inputFuelLevel: ") + 
    //   String(strSep) +
    //   String(_progVars->inputFuelLevel) +
  // Outputs
      // String(F(", Outputs: ")) +
      String(F(", O: ")) +
    // F(" outputGeneratorIgnition: ") + 
      String(_progVars->outputGeneratorIgnition) +
    // F(" outputGeneratorStarterContactor: ") + 
      String(strSep) +
      String(_progVars->outputGeneratorStarterContactor) +
    // F(" outputGeneratorOutputEnable: ") + 
      String(strSep) +
      String(_progVars->outputGeneratorOutputEnable) +
    // F(" outputGeneratorOutputEnable: ") + 
      String(strSep) +
      String(_progVars->outputIndicatorError) ;// +
    // // F(" outputIndicatorWarning: ") + 
    //   String(strSep) +
    //   String(_progVars->outputIndicatorWarning) +
    // // F(" outputIndicatorError: ") + 
    //   String(strSep) +
    //   String(_progVars->outputIndicatorError) +
    // // F(" outputIndicatorTemperature: ") + 
    //   String(strSep) +
    //   String(_progVars->outputIndicatorTemperature) +
    // // F(" outputIndicatorFuelLevel: ") + 
    //   String(strSep) +
    //   String(_progVars->outputIndicatorFuelLevel);
    // String(F(",IO: A0: ")) +
    // String(digitalRead(A0));
};
