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

    // robot serial port configuration
    static const PinName ROBOT_SERIAL_TX = p9;
    static const PinName ROBOT_SERIAL_RX = p10;
    static const int ROBOT_SERIAL_BAUD = 9600;

    // debug serial port configuration
    static const PinName DEBUG_SERIAL_TX = USBTX;
    static const PinName DEBUG_SERIAL_RX = USBRX;
    static const int DEBUG_SERIAL_BAUD = 9600;

    // command server configuration
    static const int SERVER_PORT = 1234;

    // lcd configuration
    static const int DISP_INFO_LOC_X = 0;     //!< X location for info message on LCD
    static const int DISP_INFO_LOC_Y = 0;     //!< Y location for info message on LCD
    static const int DISP_ERR_LOC_X = 0;    //!< X location for error message on LCD
    static const int DISP_ERR_LOC_Y = 1;   //!< Y location for error message on LCD

    // peripherals
    static C12832_LCD& lcd() { return singleton()._lcd; }
    static EthernetInterface& eth() { return singleton()._eth; }
    static Serial& robotPort() { return singleton()._robotPort; }
    static Serial& debugPort() { return singleton()._debugPort; }

    // remote console connection state
    static bool isConsoleActive() { return singleton()._isConsoleActive; }
    static void setConsoleActive(bool option) { singleton()._isConsoleActive = option; }

    // robot state
    static DesiredState& desiredState() { return singleton()._desiredState; }
    static CurrentState& currentState() { return singleton()._currentState; }

    // queue for pending commands and replies
    static rtos::Queue<RemoteMessage::MessageID, 1>& pendingCommand() { return singleton()._pendingCommand; }
    static rtos::Queue<RemoteMessage::MessageID, 1>& pendingReply() { return singleton()._pendingReply; }

    // initialise all communication ports
    static bool initComms();

    // serial comms
    static void debugPrint(const std::string& msg);
    static std::string robotWrite(const std::string& msg);

private:
    AppBoard();
    ~AppBoard();

public:
    bool                _isConsoleActive;

    Serial              _robotPort;
    Serial              _debugPort;
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
