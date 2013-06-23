//==============================================================================
// Project  : Lab Monkey
// Module   : mbed board
// File     : AppBoard.cpp
//==============================================================================

#include "AppBoard.h"

//==============================================================================
AppBoard::AppBoard()
//==============================================================================
    : _isConsoleActive(false),
      _robotPort(ROBOT_SERIAL_TX, ROBOT_SERIAL_RX),
      _debugPort(DEBUG_SERIAL_TX, DEBUG_SERIAL_RX)
{
}

//------------------------------------------------------------------------------
AppBoard::~AppBoard()
//------------------------------------------------------------------------------
{}

//------------------------------------------------------------------------------
bool AppBoard::initComms()
//------------------------------------------------------------------------------
{
    // serial ports
    robotPort().baud(ROBOT_SERIAL_BAUD);
    robotPort().format(8, Serial::None, 1);

    debugPort().baud(DEBUG_SERIAL_BAUD);
    debugPort().format(8, Serial::None, 1);

    // initialise ethernet. Do DHCP
    if( 0 != eth().init() )
    {
        lcd().locate(DISP_ERR_LOC_X, DISP_ERR_LOC_Y);
        lcd().printf("\nEthernet init error");
        return false;
    }

    if( 0 != eth().connect() )
    {
        lcd().locate(DISP_ERR_LOC_X, DISP_ERR_LOC_Y);
        lcd().printf("\nEthernet connect error");
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
void AppBoard::debugPrint(const std::string &msg)
//------------------------------------------------------------------------------
{
    if( debugPort().writeable() )
    {
        debugPort().puts( msg.c_str() );
    }
}

//------------------------------------------------------------------------------
std::string AppBoard::robotWrite(const std::string& msg)
//------------------------------------------------------------------------------
{
    robotPort().puts( msg.c_str() );

    int nTries = 10;
    while( 0 == robotPort().readable() )
    {
        nTries--;
        Thread::wait(10);
        if( nTries < 1 )
        {
            debugPrint("[AppBoard::robotWrite] timed out waiting for response");
            return std::string("");
        }
    }

    const int len = 256;
    char buff[len];
    std::string reply( robotPort().gets(buff, len) );

    return reply;
}
