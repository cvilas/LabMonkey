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
#include "DesiredState.h"
#include "CurrentState.h"
#include "Queue.h"
#include <string>

/// \brief singleton accessor for hardware io on the mbed application board
class AppBoard
{
public:

    // the one and only me
    static inline AppBoard& singleton();

    static const int VERBOSITY = 0; // 0: critical info, 1: 0 + motor log

    // robot serial port configuration
    static const PinName ROBOT_SERIAL_TX = p9;
    static const PinName ROBOT_SERIAL_RX = p10;
    static const int ROBOT_SERIAL_BAUD = 9600;

    static const PinName LOG_SERIAL_TX = USBTX;
    static const PinName LOG_SERIAL_RX = USBRX;

    // command server configuration
    static const int SERVER_PORT = 1234;

    // lcd configuration
    static const int DISP_INFO_LOC_X = 0;     //!< X location for info message on LCD
    static const int DISP_INFO_LOC_Y = 0;     //!< Y location for info message on LCD

    // peripherals
    static C12832_LCD& lcd() { return singleton()._lcd; }
    static Serial& logStream() { return singleton()._logStream; }
    static EthernetInterface& eth() { return singleton()._eth; }
    static Serial& robotPort() { return singleton()._robotPort; }

    // remote console connection state
    static bool isConsoleActive() { return singleton()._isConsoleActive; }
    static void setConsoleActive(bool option) { singleton()._isConsoleActive = option; }

    // robot state
    static DesiredState& desiredState() { return singleton()._desiredState; }
    static CurrentState& currentState() { return singleton()._currentState; }

    // queue for pending commands and replies
    static rtos::Queue<RemoteMessage::MessageID, 1>& pendingCommand() { return singleton()._pendingCommand; }
    static rtos::Queue<RemoteMessage::MessageID, 1>& pendingReply() { return singleton()._pendingReply; }

    static bool initComms();

private:
    AppBoard();
    ~AppBoard();

public:
    bool                _isConsoleActive;

    Serial              _robotPort;
    Serial              _logStream;
    C12832_LCD          _lcd;
    EthernetInterface   _eth;

    DesiredState _desiredState;
    CurrentState _currentState;

    rtos::Queue<RemoteMessage::MessageID, 1> _pendingCommand;
    rtos::Queue<RemoteMessage::MessageID, 1> _pendingReply;
};

//------------------------------------------------------------------------------
AppBoard& AppBoard::singleton()
//------------------------------------------------------------------------------
{
    static AppBoard s_singleton;
    return s_singleton;
 }

#endif // APPBOARD_H
