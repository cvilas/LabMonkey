//==============================================================================
// Project  : Lab Monkey
// Module   : robot
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
    //todo: do all initialisations here
    // - set buttons to interrupts

    return true;
}

//------------------------------------------------------------------------------
void RobotController::onSpeedDnBtn()
//------------------------------------------------------------------------------
{
    int s = _monkey.getSpeedScale() - 5;
    _monkey.setSpeedScale(s);
}

//------------------------------------------------------------------------------
void RobotController::onSpeedUpBtn()
//------------------------------------------------------------------------------
{
    int s = _monkey.getSpeedScale() + 5;
    _monkey.setSpeedScale(s);
}

//------------------------------------------------------------------------------
void RobotController::run()
//------------------------------------------------------------------------------
{
    while(1)
    {
        // todo: read app board io

        // check for pending command from console and process it
        if( AppBoard::isConsoleActive() )
        {
            processConsoleCommand();
        }

        // todo: do motor control

        // todo: update 'AppBoard::currentState'
    }

}

//------------------------------------------------------------------------------
void RobotController::processConsoleCommand()
//------------------------------------------------------------------------------
{
    // check if a console command is waiting..
    osEvent evt = AppBoard::pendingCommand().get(0);

    // yes..
    if( evt.status == osEventMessage )
    {
        RemoteMessage::MessageID* id = (RemoteMessage::MessageID*)evt.value.p;

        switch(*id)
        {
        case RemoteMessage::SET_MODE:
            processConsoleCommandSetMode();
            break;
        default:
            break;
        };

        // release notification memory
        delete id;
    }

}

//------------------------------------------------------------------------------
void RobotController::processConsoleCommandSetMode()
//------------------------------------------------------------------------------
{

    // todo: actually set mode and update AppBoard::currentState

    // notify console
    RemoteMessage::MessageID* reply = new RemoteMessage::MessageID;
    *reply = RemoteMessage::SET_MODE;
    AppBoard::pendingReply().put(reply);
}
