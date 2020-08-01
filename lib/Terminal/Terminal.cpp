/** 
 * This is an abstract class for terminal interaction in Arduino
 */
#include "Terminal.h"


/**
 * Constructor
 */
template <typename ProgramVars>
Terminal<ProgramVars>::Terminal(Stream & Serial, ProgramVars *programVars): _Serial (Serial) {
  _progVars = programVars;
}


/**
 * Deconstructor
 */
template <typename ProgramVars>
Terminal<ProgramVars>::~Terminal() {}


template <typename ProgramVars>
void Terminal<ProgramVars>::handle_serial_input(void) {
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
    processCommands(serialBuffer, &messages);
    // Print the message
    Serial.println(messages);
    // Reset the buffer to empty
    serialBuffer = "";
  }

}

template <typename ProgramVars>
void Terminal<ProgramVars>::print_logs(void){
   // print logging info if enabled
  if (_progVars->loggingEnabled == true) {
  // if (programVars.inputRunStop == true) {
  // if (digitalRead(INPUT_PIN_RUN_STOP) == true) {
    String logMessage = formatProgVars(timestamp);    
    _Serial.println(logMessage);
  }
}


/** This function takes a string and separates out the command and argument
 * The command is the first character, the argument is the remainder
 * 
 * In C/C++ rather than return the parts, we act on pointers to the variables
 * Pointers to variables (the memory location of a variable) are denoted by a '*'
 * 
 * This function returns EXIT_FAILURE (cuold be 0) if it fails,
 * or EXIT_SUCCESS (could be 1) if everything is OK
 **/
template <typename ProgramVars>
int Terminal<ProgramVars>::getCommandAndArgument(String inputString, char *command, String *argument) {
    // Check that the String is long enough to process and return fail otherwise
    if (inputString.length() <= 1) {
        return EXIT_FAILURE;
    }

    *command = inputString.charAt(0);
    *argument = inputString.substring(1);
    return EXIT_SUCCESS;
}


/** stringToLong function that returns state
 * Why is state important?
 * The standard string to integer functions the integer representation
 * of a string if one exists and 0 if it does not (i.e atoi('a') = 0)
 * This is **shit** because "0" is a valid and common number
 * If the string cannot be converted then we should know about it.
 * 
 * To do this we use a function that returns a state and modifes a
 * pointer to an int if the conversion is successful
 * 
 **/
template <typename ProgramVars>
int Terminal<ProgramVars>::stringToLong(String inputString, long *targetInt) {
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


/** Parse the command/args string
 * Find the command if present, and parse the arguments
 * determining where there are none, are a number, or a string
 **/
template <typename ProgramVars>
typename Terminal<ProgramVars>::CommandAndArguments Terminal<ProgramVars>::parseCommandArgs(String commandArgs) {
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


// /**
//  * This is an attempt to make a generic function for all Integer types. 
//  * It failed.... does not work...
//  */
// template <typename Z>
// template <typename ProgramVars>
// boolean Terminal<ProgramVars>::argDisplayOrSetGenericInteger(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, Z *var, String *message) {
//     if (comAndArg.argType == ARGUMENT_TYPE_NONE) {
//     *message = argName + " is : " + String(*var);
//     return false;
//     }
//     if (comAndArg.argType == ARGUMENT_TYPE_LONG) {
//     *var = comAndArg.argLong;
//     *message = "Set '" + argName + "' to : " + String(*var);
//     return true;
//     }
//     return false;
// }


/**
 * I thought you _should_ be able to do the following, you cant
 */
// template <typename ProgramVars>
// boolean Terminal<ProgramVars>::argDisplayOrSetUint8(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, uint8_t *var, String *message) {
//     return argDisplayOrSetGenericInteger<uint8_t>(argName, comAndArg, *var, *message);
// };


// template <typename ProgramVars>
// boolean Terminal<ProgramVars>::argDisplayOrSetInt8(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, int8_t *var, String *message) {
//     return argDisplayOrSetGenericInteger<int8_t>(argName, comAndArg, *var, *message);
// };

// template <typename ProgramVars>
// boolean Terminal<ProgramVars>::argDisplayOrSetUint16(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, uint16_t *var, String *message) {
//     return argDisplayOrSetGenericInteger<uint16_t>(argName, comAndArg, *var, *message);
// };


template <typename ProgramVars>
boolean Terminal<ProgramVars>::argDisplayOrSetUint8(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, uint8_t *var, String *message) {
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


template <typename ProgramVars>
boolean Terminal<ProgramVars>::argDisplayOrSetInt8(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, int8_t *var, String *message) {
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


template <typename ProgramVars>
boolean Terminal<ProgramVars>::argDisplayOrSetUint16(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, uint16_t *var, String *message) {
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


template <typename ProgramVars>
boolean Terminal<ProgramVars>::argDisplayOrSetInt16(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, int16_t *var, String *message) {
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


template <typename ProgramVars>
boolean Terminal<ProgramVars>::argDisplayOrSetUint32(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, uint32_t *var, String *message) {
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


template <typename ProgramVars>
boolean Terminal<ProgramVars>::argDisplayOrSetInt32(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, int32_t *var, String *message) {
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

/** 
 * NOTE: Cant do this with the String class:
 * >lib/Terminal/Terminal.cpp:255:59: error: call of overloaded 'String(int64_t&)' is ambiguous
 *    *message = "Set '" + argName + "' to : " + String(*var);
 */
// boolean Terminal<ProgramVars>::argDisplayOrSetUintSt64(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, uint64_t *var, String *message) {
//     if (comAndArg.argType == ARGUMENT_TYPE_NONE) {
//     *message = argName + " is : " + String(*var);
//     return false;
//     }
//     if (comAndArg.argType == ARGUMENT_TYPE_LONG) {
//     *var = comAndArg.argLong;
//     *message = "Set '" + argName + "' to : " + String(*var);
//     return true;
//     }
//     return false;
// }


// boolean Terminal<ProgramVars>::argDisplayOrSetInt64(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, int64_t *var, String *message) {
//     if (comAndArg.argType == ARGUMENT_TYPE_NONE) {
//     *message = argName + " is : " + String(*var);
//     return false;
//     }
//     if (comAndArg.argType == ARGUMENT_TYPE_LONG) {
//     *var = comAndArg.argLong;
//     *message = "Set '" + argName + "' to : " + String(*var);
//     return true;
//     }
//     return false;
// }


/** The standard OP for getting/setting/displaying command and args
 * There are two semi identical forms of this function, one where the 
 * argument is a number (long), and one where it is a string
 **/
template <typename ProgramVars>
boolean Terminal<ProgramVars>::argDisplayOrSetLong(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, long *var, String *message) {
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


/** The standard OP for getting/setting/displaying command and args
 * There are two semi identical forms of this function, one where the 
 * argument is a number (long), and one where it is a string
 **/
template <typename ProgramVars>
boolean Terminal<ProgramVars>::argDisplayOrSetDoubleFromLong(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, double *var, uint16_t denominator, String *message) {
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


/** The standard OP for getting/setting/displaying command and args
 * There are two semi identical forms of this function, one where the 
 * argument is a number (long), and one where it is a string
 **/
template <typename ProgramVars>
boolean Terminal<ProgramVars>::argDisplayOrSetFloatFromLong(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, float *var, uint16_t denominator, String *message) {
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


// String version
template <typename ProgramVars>
boolean Terminal<ProgramVars>::argDisplayOrSetString(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, String *var, String *message) {
    if (comAndArg.argType == ARGUMENT_TYPE_NONE) {
    *message = argName + " is : '" + *var + "'";
    return false;
    }
    if (comAndArg.argType == ARGUMENT_TYPE_STRING) {
    *var = comAndArg.argString;
    *message = "Set '" + argName + "' to : '" + *var + "'";
    return true;
    }
    return false;
}


// Boolean version
template <typename ProgramVars>
boolean Terminal<ProgramVars>::argDisplayOrSetBoolean(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, boolean *var, String *message) {
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
