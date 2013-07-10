//==============================================================================
// Project  : Lab Monkey
// Module   : mbed board
// File     : Display.h
// Brief    : Facilities from the application board
//==============================================================================

#ifndef DISPLAY_H
#define DISPLAY_H

#include "mbed.h"
#include "C12832_lcd.h"
#include "../MonkeyMessages/RemoteMessages.h"

/// \brief mbed LCD display functions
class Display
{
public:
    Display();
    ~Display();
    void updateTitle(const char* ipStr, int port);
    void updateModeInfo(RemoteMessage::Mode mode, int pt = 0);
    void updateSpeed(int sp);
    void updateConnectionStatus(bool connected);
private:
    C12832_LCD _lcd;
}; // Display

#endif // DISPLAY_H
