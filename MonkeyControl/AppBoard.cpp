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
      _robotPort(ROBOT_SERIAL_TX, ROBOT_SERIAL_RX)
{
}

//------------------------------------------------------------------------------
AppBoard::~AppBoard()
//------------------------------------------------------------------------------
{}

//------------------------------------------------------------------------------
bool AppBoard::initEthernet()
//------------------------------------------------------------------------------
{
    AppBoard& bd = singleton();

    // initialise ethernet. Do DHCP
    if( 0 != bd._eth.init() )
    {
        bd._lcd.locate(DISP_ERR_LOC_X, DISP_ERR_LOC_Y);
        bd._lcd.printf("\nEthernet init error");
        return false;
    }

    if( 0 != bd._eth.connect() )
    {
        bd._lcd.locate(DISP_ERR_LOC_X, DISP_ERR_LOC_Y);
        bd._lcd.printf("\nEthernet connect error");
        return false;
    }
    return true;
}

//------------------------------------------------------------------------------
bool AppBoard::initRobotPort()
//------------------------------------------------------------------------------
{
    AppBoard& bd = singleton();

    bd._robotPort.baud(ROBOT_SERIAL_BAUD);
    bd._robotPort.format(8, Serial::None, 1);
}
