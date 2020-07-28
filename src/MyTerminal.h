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

class MyTerminal: Terminal {
    public:
        MyTerminal(Stream & Serial, ProgramVars *programVars);
        ~MyTerminal(void);

        void handle_serial_input(void);
        void print_logs(void);
    private:
        String serialBuffer = "";
        String messages = "";
        String strSep = ",";

        Stream & _Serial;
        ProgramVars * _progVars;

        int processCommands(String inputString, String *message);
        String formatProgVars(long time);
};

#endif
