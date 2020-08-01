/** 
 * This is an abstract class for terminal interaction in Arduino
 * 
 * This class is abstract and generic due to each project having its
 * own program data structure requirements.
 * As such it needs to be extended/inherited by a concrete class.
 * 
 * The assumption is that the generic type 'ProgramVars' is a struct
 * with at least the following members:
 *  bool loggingEnabled
 *  
 * The following functions must implemented:
 *  'formatProgVars' which gets and sets ProgramVars from command and argument taken 
 *   from the serial input. 
 * and
 *  'formatProgVars' which is intended to format the provided program specic
 * struct to produce the loggable output.
 * 
 * See 'Terminal.cpp' for examples.
 */
#ifndef _ARDUINO_TERMINAL
#define _ARDUINO_TERMINAL
#pragma once

#include "Arduino.h"
#include "Timer.h"

// Argument type defines
#define ARGUMENT_TYPE_NONE            0
#define ARGUMENT_TYPE_LONG            1
#define ARGUMENT_TYPE_DOUBLE          2
#define ARGUMENT_TYPE_STRING          3

template <typename ProgramVars>
class Terminal {
    public:
        Terminal(Stream & Serial, ProgramVars *programVars);
        ~Terminal();

        /**
         * The Actual public methds
         * Handle serial handles serial, setting variables etc...
         * print_logs prints the formatted prog vars
         */
        void handle_serial_input(void);
        void print_logs(void);

    protected:
        String serialBuffer = "";
        String messages = "";
        String strSep = ",";
        Stream & _Serial;
        ProgramVars * _progVars;
        
        /**
         * These functions are the boring ones
         */

        // A 'struct' to hold parsed commands and arguments
        struct CommandAndArguments {
            char    command;
            int     argType;
            long    argLong;
            String  argString;
            boolean parseState;
        };

        // Impliment these please in concrete class
        /**
         * The following function should get and set members of _progVars
         * which is in the class scope
         */
        virtual void getAndSetProgramVars(CommandAndArguments comArgState, String *message) =0;
        
        /**
         * This function is intended to format the program vars
         * for logging output
         */
        virtual String formatProgVars(long time) = 0;

        /**
         * Process the serial string to separte the command and arguments
         * This calls 'getAndSetProgramVars' to get and set ProgamVars
         */
        int processCommands(String inputString, String *message);

        // /** 
        //  * The standard OP for getting/setting/displaying command and args
        //  * This is the generic Integer type
        //  **/
        // template <typename Z>
        // boolean argDisplayOrSetGenericInteger(String argName, CommandAndArguments comAndArg, Z *var, String *message);
        
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

        // Cant do this with 64bit and Strins. See implimentation
        // boolean argDisplayOrSetUintSt64(String argName, CommandAndArguments comAndArg, uint64_t *var, String *message);
        // boolean argDisplayOrSetInt64(String argName, CommandAndArguments comAndArg, int64_t *var, String *message);

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

};

#endif