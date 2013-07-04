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
      _logStream(LOG_SERIAL_TX, LOG_SERIAL_RX)
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
    AppBoard& bd = singleton();

    // log port
    bd._logStream.baud(ROBOT_SERIAL_BAUD);
    bd._logStream.format(8, Serial::None, 1);

    // initialise ethernet. Do DHCP
    if( 0 != bd._eth.init() )
    {
        bd._logStream.printf("[AppBoard::initComms] Ethernet init error\n");
        return false;
    }

    if( 0 != bd._eth.connect() )
    {
        bd._logStream.printf("[AppBoard::initComms] Ethernet connect error\n");
        return false;
    }

    // robot port
    bd._robotPort.baud(ROBOT_SERIAL_BAUD);
    bd._robotPort.format(8, Serial::None, 1);

    bd._lcd.locate(AppBoard::DISP_INFO_LOC_X, AppBoard::DISP_INFO_LOC_Y);
    bd._lcd.printf("IP: %s", AppBoard::eth().getIPAddress());
    bd._logStream.printf("[AppBoard::initComms] Robot IP: %s\n", AppBoard::eth().getIPAddress());

    return true;
}

