#include "config.h"
#include "AppBoard.h"
#include "CommandServer.h"
#include "rtos.h"


//=============================================================================
void command_thread(void const* arg)
//=============================================================================
{
    CommandServer cs;
    if( cs.init(SERVER_PORT) )
    {
        cs.run();
    }
}

//=============================================================================
int main()
//=============================================================================
{
    AppBoard::lcd().cls();

    if( !AppBoard::initEthernet() )
    {
        while(1){}
    }

    AppBoard::lcd().locate(AppBoard::DISP_ID_LOC_X, AppBoard::DISP_ID_LOC_Y);
    AppBoard::lcd().printf("IP: %s", AppBoard::eth().getIPAddress());

    // command thread
    Thread commandserver(command_thread);

    while(1)
    {
        Thread::wait(1000);
    }
}
