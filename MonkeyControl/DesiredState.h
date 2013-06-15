//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : DesiredState.h
// Brief    : structure to hold desired arm state
//==============================================================================

#ifndef DESIREDSTATE_H
#define DESIREDSTATE_H

#include "../MonkeyMessages/RemoteMessages.h"
#include "Mutex.h"

/// \brief holds desired state values for the arm
class DesiredState
{
public:
    DesiredState()
        : mode(RemoteMessage::MODE_TEACH),
          speed(0)
    {}

    ~DesiredState() {}

    rtos::Mutex         lock;   //!< access lock

    RemoteMessage::Mode mode;
    unsigned int        speed;  //!< global speed 0 - 100 range
};

#endif // DESIREDSTATE_H
