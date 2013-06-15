//==============================================================================
// Project  : Lab Monkey
// Module   : mbed board
// File     : AppBoard.h
// Brief    : Facilities from the application board
//==============================================================================

#ifndef APPBOARD_H
#define APPBOARD_H

#include "mbed.h"
#include "C12832_lcd.h"
#include "EthernetInterface.h"

/// \brief singleton accessor for hardware io on the mbed application board
class AppBoard
{
public:
    static inline AppBoard& singleton();

    static const int DISP_ID_LOC_X = 0;     //!< X location for identifier message on LCD
    static const int DISP_ID_LOC_Y = 0;     //!< Y location for identifier message on LCD
    static const int DISP_INFO_LOC_X = 0;     //!< X location for info message on LCD
    static const int DISP_INFO_LOC_Y = 1;     //!< Y location for info message on LCD
    static const int DISP_ERR_LOC_X = 0;    //!< X location for error message on LCD
    static const int DISP_ERR_LOC_Y = 2;   //!< Y location for error message on LCD

    static C12832_LCD& lcd() { return singleton()._lcd; }
    static EthernetInterface& eth() { return singleton()._eth; }

    static bool initEthernet();
private:
    AppBoard();
    ~AppBoard();

public:
    C12832_LCD          _lcd;
    EthernetInterface   _eth;
};

//------------------------------------------------------------------------------
AppBoard& AppBoard::singleton()
//------------------------------------------------------------------------------
{
    static AppBoard s_singleton;
    return s_singleton;
 }

#endif // APPBOARD_H
