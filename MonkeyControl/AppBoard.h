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

/// \brief singleton accessor for hardware io on the mbed application board
class AppBoard
{
public:
    static inline AppBoard& singleton();

    static const int DISP_ID_LOC_X = 0;     //!< X location for identifier message on LCD
    static const int DISP_ID_LOC_Y = 0;     //!< Y location for identifier message on LCD
    static const int DISP_INFO_LOC_X = 0;     //!< X location for info message on LCD
    static const int DISP_INFO_LOC_Y = 1;     //!< Y location for info message on LCD
    static const int DISP_ERR_LOC_X = 0;    //!< X location for error message on LCD
    static const int DISP_ERR_LOC_Y = 2;   //!< Y location for error message on LCD

    static C12832_LCD& lcd() { return singleton()._lcd; }
    static EthernetInterface& eth() { return singleton()._eth; }

    static DesiredState& desiredState() { return singleton()._desiredState; }
    static CurrentState& currentState() { return singleton()._currentState; }

    /// queue of pending commands. Robot control thread does the following
    /// - check the queue for pending command id
    /// - read and destroy the pending command id (call delete on pointer returned by get)
    /// - update currentState depending on command id
    /// - Append the command id to pendingResponses queue to notify command server
    /// Note: delete any items taken off the queue (call delete)
    static rtos::Queue<RemoteMessage::MessageID, 1>& pendingCommand() { return singleton()._pendingCommand; }

    static rtos::Queue<RemoteMessage::MessageID, 1>& pendingReply() { return singleton()._pendingReply; }

    static bool initEthernet();
private:
    AppBoard();
    ~AppBoard();

public:
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
