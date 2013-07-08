//==============================================================================
// Project  : Lab Monkey
// Module   : mbed board
// File     : Display.cpp
//==============================================================================

#include "Display.h"
#include <sstream>

//==============================================================================
Display::Display()
//==============================================================================
{
    _lcd.set_auto_up(0);
    _lcd.setmode(NORMAL);
    _lcd.cls();
    _lcd.set_contrast(25);
    _lcd.locate(20,0);
    _lcd.printf("LabMonkey says Hi!");
    _lcd.copy_to_lcd();
}

//------------------------------------------------------------------------------
Display::~Display()
//------------------------------------------------------------------------------
{}

//------------------------------------------------------------------------------
void Display::updateTitle(const char* ipStr, int port)
//------------------------------------------------------------------------------
{
    _lcd.locate(0,0);
    std::ostringstream str;
    str << ipStr << ":" << port;
    _lcd.printf("%-25s", str.str().c_str());
    _lcd.copy_to_lcd();
}

//------------------------------------------------------------------------------
void Display::updateModeInfo(RemoteMessage::Mode mode, int pt)
//------------------------------------------------------------------------------
{
    _lcd.locate(0,9);
    switch(mode)
    {
    case RemoteMessage::MODE_TEACH:
        _lcd.printf("%-10s %3d pts", "TEACH mode", pt);
        break;
    case RemoteMessage::MODE_REPLAY:
        _lcd.printf("%-10s %3d    ", "PLAY  mode", pt);
        break;
    default:
        _lcd.printf("%-25s", "");
        break;
    }

    _lcd.copy_to_lcd();
}

//------------------------------------------------------------------------------
void Display::updateSpeed(int sp)
//------------------------------------------------------------------------------
{
    _lcd.locate(0,18);
    _lcd.printf("%-11s %3d/10", "Speed", sp);
    _lcd.copy_to_lcd();
}

//------------------------------------------------------------------------------
void Display::updateConnectionStatus(bool connected)
//------------------------------------------------------------------------------
{
    _lcd.locate(100,0);
    if( connected )
    {
        _lcd.printf("*");
    }
    else
    {
        _lcd.printf(" ");
    }
    _lcd.copy_to_lcd();
}
