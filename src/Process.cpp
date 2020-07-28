
#include "Process.h"

/**
 * Setup the hardware, inputs and outputs
 */
void Process::begin(void) {
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
}

bool Process::read_run_stop(uint8_t digitalInputPin, uint16_t debounceNum) {
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


bool Process::read_generator_running(uint8_t digitalInputPin, uint16_t debounceNum) {
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


uint16_t Process::secondsToTicks(uint16_t seconds, uint16_t ticksPerCycle) {
  return seconds*(1000.0/ticksPerCycle);
}


/**
 * Read inputs and process such that the 'input' parameters are
 * ready for use by the application. i.e witches are debounced,
 * and analogue signals are calibrated/corrected
 */
void Process::read_inputs(ProgramVars *progVars, uint16_t periodTimeMillis) {

    progVars->inputRunStop = read_run_stop(
      INPUT_PIN_RUN_STOP, progVars->startSwitchDebounceCount);
    progVars->inpputRunIndicator = read_generator_running(
      INPUT_PIN_RUN_INDICATOR, progVars->generatorRunningDebounceCount);
    progVars->inputTemperature = analogRead(INPUT_PIN_ANALOG_TEMPERATURE);
    progVars->inputFuelLevel = analogRead(INPUT_PIN_ANALOG_FUEL_LEVEL);
}


/**
 * Calculate the state - i.e start, stop, and manage the generator
 */
void Process::calculate_state(ProgramVars *progVars, uint16_t periodTimeMillis) {
  // programVars.outputGeneratorStarterContactor = programVars.inputRunStop;
  // programVars.outputGeneratorOutputEnable = programVars.inpputRunIndicator;

  /**
   * Generator starting and stopping logic.
   * Probably need a few functions in here...
   */
  // Lets determine if there is a state change and which one
  // We need to debounce this input somehow. Maybe count up debounce max-> starting and down to 0-> stopping
  // Serial.print("calculating state. ");
  if(progVars->inputRunStop == true){
    // Serial.print("runStop true, ");
    if(progVars->generatorState == GENERATOR_STATE_ON) {
      // Serial.print("generator ON, ");
      // Check whether we are still in a 'running' state.
      // If not, move to retry
      if(progVars->inpputRunIndicator == false) {
        progVars->outputGeneratorOutputEnable = false;
        progVars->generatorState = GENERATOR_STATE_RETRY_WAIT;
      }
    } else if(progVars->generatorState == GENERATOR_STATE_RUNNING) {
      // Serial.print("Generator Running, ");
      // We need to move from Running to On, which means waiting the
      // required number of seconds
      // TODO: Can we set the X_TIME_MILLIS context so this is not hard coded to MEDIUM?
      if(progVars->startToOnTimeCurrentTicks >= secondsToTicks(progVars->startToOnTimeSeconds, periodTimeMillis)){
        // Sweet, the generator has been on long enough, can set the state to ON
        progVars->startToOnTimeCurrentTicks = 0;
        progVars->outputGeneratorOutputEnable = true;
        progVars->generatorState = GENERATOR_STATE_ON;
      } else {
        // We have not quite waited long enough
        progVars->startToOnTimeCurrentTicks++;
        // Let us be safe and ensure that the output is disabled
        progVars->outputGeneratorOutputEnable = false;
      }
    } else if(progVars->generatorState == GENERATOR_STATE_START_CHECK) {
      // We want to ensure that the generator is reporting a 'running' state for a while, and at least the right number of times
      if(progVars->checkTimeCurrentTicks >= secondsToTicks(progVars->checkTimeSeconds, periodTimeMillis)) {
        // If the generator is in reporting running, then let us move to running, otherwise retry/wait
        if(progVars->inpputRunIndicator == true){
          // Good, the generator is reporting a running state, we can begin counting down to enabling the output
          progVars->checkTimeCurrentTicks = 0;
          progVars->generatorState = GENERATOR_STATE_RUNNING;
        } else {
          // Oh dear, we need to retry starting the generator
          progVars->checkTimeCurrentTicks = 0;
          // We set the retry count here!
          progVars->numRetries++;
          progVars->outputGeneratorIgnition = false;
          progVars->generatorState = GENERATOR_STATE_RETRY_WAIT;
        }
      } else {
        // Incriment the count and check the output debounce
        progVars->checkTimeCurrentTicks++;
      }
    } else if(progVars->generatorState == GENERATOR_STATE_STARTING) {
      // We have got to keep that contactor on for the required time period
      if(progVars->starterContactorTimeCurrentTicks >= secondsToTicks(progVars->starterContactorTimeSeconds, periodTimeMillis)) {
        // Sweet, the contactor has been on long enough
        progVars->starterContactorTimeCurrentTicks = 0;
        progVars->outputGeneratorStarterContactor = false;
        progVars->outputGeneratorIgnition = true;
        progVars->generatorState = GENERATOR_STATE_START_CHECK;
      } else {
        // Lets keep the contactor on
        progVars->starterContactorTimeCurrentTicks++;
        progVars->outputGeneratorStarterContactor = true;
        progVars->outputGeneratorIgnition = true;
      }
    } else if(
      progVars->generatorState == GENERATOR_STATE_STOPPING ||
      progVars->generatorState == GENERATOR_STATE_OFF) {
        // Serial.print("setting to Starting, ");
        progVars->startSwitchDebounceCurrentTicks = 0;
        progVars->starterContactorTimeCurrentTicks = 0;
        progVars->startToOnTimeCurrentTicks = 0;
        progVars->outputGeneratorIgnition = true;
        progVars->generatorState = GENERATOR_STATE_STARTING;
    } else if(progVars->generatorState == GENERATOR_STATE_RETRY_WAIT) {
      // If the maximum number of retries is exceeded, then we shutdown and go to error state
      if(progVars->numRetries >= progVars->maxRetries) {
        progVars->numRetries = 0;
        progVars->generatorState = GENERATOR_STATE_START_ERROR;
        // Lets turn off the outputs just to be sure
        progVars->outputGeneratorStarterContactor = false;
        progVars->outputGeneratorOutputEnable = false;
      } else if(progVars->retryWaitTimeCurrentTicks >= secondsToTicks(progVars->retryWaitTimeSeconds, periodTimeMillis)){
        // Lets try starting again
        progVars->retryWaitTimeCurrentTicks = 0;
        progVars->generatorState = GENERATOR_STATE_STARTING;
      } else {
        // Incriment that timer
        progVars->retryWaitTimeCurrentTicks++;
      }
    } else if(
      progVars->generatorState == GENERATOR_STATE_START_ERROR ||
      progVars->generatorState == GENERATOR_STATE_STOP_ERROR) {
      // Reset all the counters, and... do nothing as we are in error and
      // it is not the role of this process to correct it.
      progVars->outputGeneratorIgnition = false;
      progVars->outputGeneratorStarterContactor = false;
      progVars->outputGeneratorOutputEnable = false;
      progVars->startSwitchDebounceCurrentTicks = 0;
      progVars->starterContactorTimeCurrentTicks = 0;
      progVars->startToOnTimeCurrentTicks = 0;
    }
  } else {
    // The run switch is off
    if (progVars->generatorState == GENERATOR_STATE_OFF) {
      // Do nothing
      // If the generator is running, issue an error
      if(progVars->inpputRunIndicator == true){
        progVars->generatorState = GENERATOR_STATE_STOP_ERROR;
      }
    } else if (
        progVars->generatorState == GENERATOR_STATE_RETRY_WAIT ||
        progVars->generatorState == GENERATOR_STATE_STARTING ||
        progVars->generatorState == GENERATOR_STATE_START_CHECK ||
        progVars->generatorState == GENERATOR_STATE_RUNNING ||
        progVars->generatorState == GENERATOR_STATE_ON) {
      // Let us stop the generator
      progVars->outputGeneratorIgnition = false;
      progVars->outputGeneratorStarterContactor = false;
      progVars->outputGeneratorOutputEnable = false;
      progVars->checkTimeCurrentTicks = 0;
      progVars->generatorState = GENERATOR_STATE_STOPPING;
    } else if(progVars->generatorState == GENERATOR_STATE_STOPPING) {
      // Use the check time and check if it is running or not given debounce
      if(progVars->checkTimeCurrentTicks >= secondsToTicks(progVars->checkTimeSeconds, periodTimeMillis)) {
        // Let us check how many times the input was set off, i.e debounced
        if(progVars->inpputRunIndicator == false){
          // Good, the generator is reporting a not running state
          progVars->checkTimeCurrentTicks = 0;
          progVars->generatorState = GENERATOR_STATE_OFF;
        } else {
          // Oh dear, the generator is not turning off! faark
          progVars->checkTimeCurrentTicks = 0;
          progVars->generatorState = GENERATOR_STATE_STOP_ERROR;
        }
      } else {
        // Incriment the count and check the output debounce
        progVars->checkTimeCurrentTicks++;
      }
    }
  }
}


/**
 * Flash a light with a given on ticks and off time (ticks)
 */
bool Process::flash_error_light(bool indicatorState, uint8_t onTicks, uint8_t offTicks) {
  static uint8_t onTicksCount = 0;
  static uint8_t offTicksCount = 0;
  static bool returnState = indicatorState;
  
  if(indicatorState == true) {
    if(onTicksCount++ >= onTicks) {
      onTicksCount = 0;
      returnState = false;
    }
  } else {
    if(offTicksCount++ >= offTicks) {
      offTicksCount = 0;
      returnState = true;
    }
  }

  return returnState;
}


/**
 * This process function is to check the limit conditions and shut
 * down or raise warnings if limits breached or are close
 * 
 * Right now all this does is flash an error light if the
 * calculate_state function thinks there is an error
 */
void Process::check_limits(ProgramVars *progVars, uint16_t periodTimeMillis) {
  // Simply raise the error output for now.
  // You must reset to break out of error
  if(
      progVars->generatorState == GENERATOR_STATE_START_ERROR || 
      progVars->generatorState == GENERATOR_STATE_STOP_ERROR) {
    progVars->outputIndicatorError = flash_error_light(progVars->outputIndicatorError, 7, 3);
  }
}

void Process::set_outputs(ProgramVars *progVars, uint16_t periodTimeMillis) {
  digitalWrite(OUTPUT_PIN_GENERATOR_IGNITION, progVars->outputGeneratorIgnition);
  digitalWrite(OUTPUT_PIN_GENERATOR_STARTER_CONTACTOR, progVars->outputGeneratorStarterContactor);
  digitalWrite(OUTPUT_PIN_GENERATOR_OUTPUT_ENABLED, progVars->outputGeneratorOutputEnable);
  digitalWrite(OUTPUT_PIN_INDICATOR_ERROR, progVars->outputIndicatorError);
}


void Process::process(ProgramVars *progVars, uint16_t periodTimeMillis) {
  if(progVars->processControlEnabled == true) {
    // Read inputs
    if (progVars->readInputsEnabled) {
      read_inputs(progVars, periodTimeMillis);
    }

    // Calculate state
    if (progVars->calculateStateEnabled) {
      calculate_state(progVars, periodTimeMillis);
    }

    // Check Limits
    if (progVars->checkLimitsEnabled) {
      check_limits(progVars, periodTimeMillis);
    }

    // Set outputs
    if (progVars->setOutputsEnabled) {
      set_outputs(progVars, periodTimeMillis);
    }
  }
}
