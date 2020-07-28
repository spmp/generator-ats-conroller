/** 
 * This is the concrete implementation of the terminal
 * for
 * Generator ATS Controller
 */
#ifndef _ARDUINO_MY_TERMINAL
#define _ARDUINO_MY_TERMINAL
#pragma once

#include "Arduino.h"
// #include "Terminal.h"
#include "Process.h"
#include "Timer.h"

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

class MyTerminal {
    public:
        MyTerminal(Stream & Serial, ProgramVars *programVars);
        ~MyTerminal(void);

        void handle_serial_input(void);
        String formatProgVars(long time);
    private:
        String serialBuffer = "";
        String messages = "";
        String strSep = ",";

        Stream & _Serial;
        ProgramVars * _progVars;

        // A 'struct' to hold parsed commands and arguments
        struct CommandAndArguments {
            char    command;
            int     argType;
            long    argLong;
            String  argString;
            boolean parseState;
        };

        int processCommands(String inputString, String *message);

        int getCommandAndArgument(String inputString, char *command, String *argument);
        int stringToLong(String inputString, long *targetInt);
        CommandAndArguments parseCommandArgs(String commandArgs);

        boolean argDisplayOrSetBoolean(String argName, CommandAndArguments comAndArg, boolean *var, String *message);
        boolean argDisplayOrSetUint8(String argName, CommandAndArguments comAndArg, uint8_t *var, String *message);
        boolean argDisplayOrSetUint16(String argName, CommandAndArguments comAndArg, uint16_t *var, String *message);
        boolean argDisplayOrSetFloatFromLong(String argName, CommandAndArguments comAndArg, float *var, uint16_t denominator, String *message);

};

#endif
