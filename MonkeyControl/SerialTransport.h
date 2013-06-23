//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : SerialTransport.h
// Brief    : Serial comms wrapper
//==============================================================================

#ifndef SERIALTRANSPORT_H
#define SERIALTRANSPORT_H

#include "mbed.h"

class SerialTransport
{
public:
    SerialTransport(PinName tx, PinName rx);
    Serial& port() { return _port; }

private:
    Serial _port;
};

#endif // SERIALTRANSPORT_H
