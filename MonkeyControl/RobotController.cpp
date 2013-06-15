//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : RobotController.cpp
//==============================================================================

#include "RobotController.h"
#include "../MonkeyMessages/RemoteMessages.h"
#include "AppBoard.h"

//==============================================================================
RobotController::RobotController()
//==============================================================================
{
}

//------------------------------------------------------------------------------
RobotController::~RobotController()
//------------------------------------------------------------------------------
{}

//------------------------------------------------------------------------------
bool RobotController::init()
//------------------------------------------------------------------------------
{
    return true;
}

//------------------------------------------------------------------------------
void RobotController::run()
//------------------------------------------------------------------------------
{
    // check pending command
    // process it
    // notify completion

    while(1)
    {

        // example
        osEvent evt = AppBoard::pendingCommand().get(0);
        if( evt.status == osEventMessage )
        {
            RemoteMessage::MessageID* id = (RemoteMessage::MessageID*)evt.value.p;

            if( *id == RemoteMessage::SET_MODE )
            {
                RemoteMessage::MessageID* reply = new RemoteMessage::MessageID;
                *reply = *id;
                AppBoard::pendingReply().put(reply);
            }
            delete id;
        }

    }

}
