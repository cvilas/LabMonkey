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
    if( !AppBoard::initPorts() )
    {
        while(1){}
    }
    AppBoard::logStream().printf("Ports initialised\n");

    // Robot control thread
    AppBoard::logStream().printf("Starting robot controller\n");
    RobotController rc;
    Thread robotcontroller(robot_thread, &rc);
    Thread::wait(1000);

    // command thread
    AppBoard::logStream().printf("Starting command server\n");
    CommandServer cs(rc);
    Thread commandserver(command_thread, &cs);

    while(1)
    {
        Thread::wait(1000);
    }
}
