//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : CurrentState.h
// Brief    : structure to hold current arm state
//==============================================================================

#ifndef CURRENTSTATE_H
#define CURRENTSTATE_H

#include "../MonkeyMessages/RemoteMessages.h"
#include "Mutex.h"

/// \brief holds desired state values for the arm
class CurrentState
{
public:
    CurrentState()
        : mode(RemoteMessage::MODE_TEACH),
          speed(0)
    {}

    ~CurrentState() {}

    rtos::Mutex         lock;   //!< access lock

    RemoteMessage::Mode mode;
    unsigned int        speed;  //!< global speed 0 - 100 range
};

#endif // CURRENTSTATE_H
