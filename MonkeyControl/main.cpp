#include "AppBoard.h"
#include "CommandServer.h"
#include "RobotController.h"
#include "rtos.h"


//=============================================================================
void command_thread(void const* arg)
//=============================================================================
{
    CommandServer* cs = (CommandServer*)(arg);
    if( cs->init(AppBoard::SERVER_PORT) )
    {
        cs->run();
    }
}

//=============================================================================
void robot_thread(void const* arg)
//=============================================================================
{
    RobotController* rc = (RobotController*)(arg);
    if( rc->init() )
    {
        rc->run();
    }
}

//=============================================================================
int main()
//=============================================================================
{
    AppBoard::lcd().cls();
    RobotController rc;
    CommandServer cs(rc);

    if( !AppBoard::initPorts() )
    {
        while(1){}
    }

    // Robot control thread
    Thread robotcontroller(robot_thread, &rc);

    // command thread
    Thread commandserver(command_thread, &cs);

    while(1)
    {
        Thread::wait(1000);
    }
}
