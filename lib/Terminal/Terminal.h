/** 
 * This is an abstract class for terminal interaction in Arduino
 */
#ifndef ARDUINO_TERMINAL
#define ARDUINO_TERMINAL
#pragma once

#include "Arduino.h"

// Argument type defines
#define ARGUMENT_TYPE_NONE            0
#define ARGUMENT_TYPE_LONG            1
#define ARGUMENT_TYPE_DOUBLE          2
#define ARGUMENT_TYPE_STRING          3

template <typename ProgramVars>
class Terminal {
    public:
        Terminal(void);
        ~Terminal(void);

        // Serial buffer and message
        String serialBuffer = "";
        String messages = "";

        // A 'struct' to hold parsed commands and arguments
        struct CommandAndArguments {
            char    command;
            int     argType;
            long    argLong;
            String  argString;
            boolean parseState;
        };

        /**
         * The 'setup' function
         */

        /** This function takes a string and separates out the command and argument
         * The command is the first character, the argument is the remainder
         * 
         * In C/C++ rather than return the parts, we act on pointers to the variables
         * Pointers to variables (the memory location of a variable) are denoted by a '*'
         * 
         * This function returns EXIT_FAILURE (cuold be 0) if it fails,
         * or EXIT_SUCCESS (could be 1) if everything is OK
         **/
        int getCommandAndArgument(String inputString, char *command, String *argument);

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
        int stringToLong(String inputString, long *targetInt);

        /** Parse the command/args string
         * Find the command if present, and parse the arguments
         * determining where there are none, are a number, or a string
         **/
        CommandAndArguments parseCommandArgs(String commandArgs);

        /**
         * Display or set the argument of the specific Integer types.
         * If the argument value is empty the var is returned in String form,
         * otherwise the var is updated to that provided and the message contains
         * confirmation
         */
        boolean argDisplayOrSetUint8(String argName, CommandAndArguments comAndArg, uint8_t *var, String *message);
        boolean argDisplayOrSetInt8(String argName, CommandAndArguments comAndArg, int8_t *var, String *message);

        boolean argDisplayOrSetUint16(String argName, CommandAndArguments comAndArg, uint16_t *var, String *message);
        boolean argDisplayOrSetInt16(String argName, CommandAndArguments comAndArg, int16_t *var, String *message);

        boolean argDisplayOrSetUint32(String argName, CommandAndArguments comAndArg, uint32_t *var, String *message);
        boolean argDisplayOrSetInt32(String argName, CommandAndArguments comAndArg, int32_t *var, String *message);

        boolean argDisplayOrSetUintSt64(String argName, CommandAndArguments comAndArg, uint64_t *var, String *message);
        boolean argDisplayOrSetInt64(String argName, CommandAndArguments comAndArg, int64_t *var, String *message);

        /** The standard OP for getting/setting/displaying command and args
         * There are two semi identical forms of this function, one where the 
         * argument is a number (long), and one where it is a string
         **/
        boolean argDisplayOrSetLong(String argName, CommandAndArguments comAndArg, long *var, String *message);


        /** The standard OP for getting/setting/displaying command and args
         * There are two semi identical forms of this function, one where the 
         * argument is a number (long), and one where it is a string
         **/
        boolean argDisplayOrSetDoubleFromLong(String argName, CommandAndArguments comAndArg, double *var, uint16_t denominator, String *message);


        /** The standard OP for getting/setting/displaying command and args
         * There are two semi identical forms of this function, one where the 
         * argument is a number (long), and one where it is a string
         **/
        boolean argDisplayOrSetFloatFromLong(String argName, CommandAndArguments comAndArg, float *var, uint16_t denominator, String *message);


        // String version
        boolean argDisplayOrSetString(String argName, CommandAndArguments comAndArg, String *var, String *message);


        // Boolean version
        boolean argDisplayOrSetBoolean(String argName, CommandAndArguments comAndArg, boolean *var, String *message);


        /** 
         * *do things* based on inputString:
         *   * Update progVars
         *   * Show progVars
         * This function should not change output state, but if vars change
         * the 'stateChange' flag should be set
         */
        virtual int processCommands(String inputString, ProgramVars *progVars, String *message) = 0;
        
        virtual String formatProgVars(long time, ProgramVars progVars) = 0;
    private:
        /** 
         * The standard OP for getting/setting/displaying command and args
         * This is the generic Integer type
         **/
        template <typename Z>
        boolean argDisplayOrSetGenericInteger(String argName, Terminal<ProgramVars>::CommandAndArguments comAndArg, Z *var, String *message);

};

#endif