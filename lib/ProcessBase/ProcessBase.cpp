
#include "ProcessBase.h"

/**
 * Setup the hardware, inputs and outputs
 */
template <typename ProgramVars>
void ProcessBase<ProgramVars>::begin(void) {
  // Setup the LED to output
  pinMode(LED_BUILTIN, OUTPUT);
}

/**
 * Run the complete process control loop
 * Ideally this should be registered to a timer
 * on a 'medium' or 'fast' time interval
 */
template <typename ProgramVars>
void ProcessBase<ProgramVars>::process(ProgramVars *progVars, uint16_t periodTimeMillis) {
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


/**
 * Flash an LED to show the uC is alive
 * Ideally this should be registered to a timer
 * on a 'long' time interval
 * 
 * This should likely be overwridden
 */
template <typename ProgramVars>
void ProcessBase<ProgramVars>::flash_led(ProgramVars *progVars, uint16_t periodTimeMillis){
  // Toggle the LED
  digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

/**
 * Read inputs and process such that the 'input' parameters are
 * ready for use by the application. i.e witches are debounced,
 * and analogue signals are calibrated/corrected
 */
template <typename ProgramVars>
void ProcessBase<ProgramVars>::read_inputs(ProgramVars *progVars, uint16_t periodTimeMillis) {

}

/**
 * Calculate the new program states based on inputs etc.
 */
template <typename ProgramVars>
void ProcessBase<ProgramVars>::calculate_state(ProgramVars *progVars, uint16_t periodTimeMillis) {

}

/**
 * Check for limit conditions, and handle warnings and errors
 */
template <typename ProgramVars>
void ProcessBase<ProgramVars>::check_limits(ProgramVars *progVars, uint16_t periodTimeMillis) {

}

/**
 * Set the uC outputs
 */
template <typename ProgramVars>
void ProcessBase<ProgramVars>::set_outputs(ProgramVars *progVars, uint16_t periodTimeMillis) {

}
