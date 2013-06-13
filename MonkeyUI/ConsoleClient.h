//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : ConsoleClient.h
// Brief    : Remote client for console commands
//==============================================================================

#ifndef CONSOLECLIENT_H
#define CONSOLECLIENT_H

#include "io/IDataPort.h"

/// \brief Remote client interface for communicating with the robot
/// \ingroup console
class ConsoleClient
{
public:
    ConsoleClient(Grape::IDataPort& transport);
    ~ConsoleClient();
private:
    Grape::IDataPort& _transport;
};

#endif // CONSOLECLIENT_H
