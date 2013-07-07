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
#include <QDebug>

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
    Grape::IDataPort* getTransport() { return _pTransport; }

    RemoteMessage::Mode setMode(RemoteMessage::Mode mode);
    RemoteMessage::Mode getMode();
    int getNumWayPoints();

    bool play(bool option);
    int setSpeed(int value);
    int getSpeed();

    int addWayPoint();
    bool setHome();
    int clearWayPoints();

private:
    bool command(RemoteMessage &cmd, RemoteMessage &resp);
    Grape::IDataPort* _pTransport;
    MessageFramer _framer;
};



#endif // CONSOLECLIENT_H
