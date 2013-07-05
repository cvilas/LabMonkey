//==============================================================================
// Project  : Lab Monkey
// Module   : mbed board
// File     : AppBoard.h
// Brief    : Facilities from the application board
//==============================================================================

#ifndef APPBOARD_H
#define APPBOARD_H

#include "mbed.h"
#include "C12832_lcd.h"
#include "EthernetInterface.h"
#include <string>

/// \brief singleton accessor for hardware io on the mbed application board
class AppBoard
{
public:

    // the one and only me
    static inline AppBoard& singleton();

    // logging intensity
    static const int VERBOSITY = 0; // 0: critical info, 1: 0 + motor log

    // log port configuration
    static const PinName LOG_SERIAL_TX = USBTX;
    static const PinName LOG_SERIAL_RX = USBRX;

    // robot serial port configuration
    static const PinName ROBOT_SERIAL_TX = p9;
    static const PinName ROBOT_SERIAL_RX = p10;
    static const int ROBOT_SERIAL_BAUD = 9600;

    // command server configuration
    static const int SERVER_PORT = 1234;

    // control button mapping
    static const PinName BTN_MODE = p13; // mode
    static const PinName BTN_REC = p14; // rec
    static const PinName BTN_PLAY = p14; // play/stop
    static const PinName BTN_SPEED_UP = p15;
    static const PinName BTN_CLEAR = p15;
    static const PinName BTN_SPEED_DN = p16;
    static const PinName BTN_HOME = p16;

    // lcd configuration
    static const int DISP_INFO_LOC_X = 0;     //!< X location for info message on LCD
    static const int DISP_INFO_LOC_Y = 0;     //!< Y location for info message on LCD

    // peripherals
    static C12832_LCD& lcd() { return singleton()._lcd; }
    static Serial& logStream() { return singleton()._logStream; }
    static EthernetInterface& eth() { return singleton()._eth; }
    static Serial& robotPort() { return singleton()._robotPort; }

    static bool initPorts();

private:
    AppBoard();
    ~AppBoard();

public:

    Serial              _robotPort;
    Serial              _logStream;
    C12832_LCD          _lcd;
    EthernetInterface   _eth;
};

//------------------------------------------------------------------------------
AppBoard& AppBoard::singleton()
//------------------------------------------------------------------------------
{
    static AppBoard s_singleton;
    return s_singleton;
 }

#endif // APPBOARD_H
