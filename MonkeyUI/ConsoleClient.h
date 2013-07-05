//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : ConsoleClient.h
// Brief    : Remote client for console commands
//==============================================================================

#ifndef CONSOLECLIENT_H
#define CONSOLECLIENT_H

#include "io/IDataPort.h"
#include "MonkeyMessages/RemoteMessages.h"
#include "MonkeyMessages/MessageFramer.h"

/// \brief Remote client interface for communicating with the robot
/// \ingroup console
class ConsoleClient
{
public:
    static const unsigned int TIMEOUT_MILLISECS = 10000;

public:
    ConsoleClient();
    ~ConsoleClient();
    void setTransport(Grape::IDataPort* pTransport);

    RemoteMessage::Mode setMode(RemoteMessage::Mode mode);
private:
    Grape::IDataPort* _pTransport;
    MessageFramer _framer;
};

#endif // CONSOLECLIENT_H
