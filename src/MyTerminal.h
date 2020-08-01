/** 
 * This is the concrete implementation of the terminal
 * for
 * Generator ATS Controller
 */
#ifndef _ARDUINO_MY_TERMINAL
#define _ARDUINO_MY_TERMINAL
#pragma once

#include "Arduino.h"
#include "Terminal.h"
#include "Terminal.cpp"
#include "Process.h"

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

class MyTerminal: public Terminal<ProgramVars> {
    public:
        MyTerminal(Stream & Serial, ProgramVars *programVars);
        ~MyTerminal(void);

    private:
        void getAndSetProgramVars(CommandAndArguments comArgState, String *message);
        String formatProgVars(long time);
};

#endif
