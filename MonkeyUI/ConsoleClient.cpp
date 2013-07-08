//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : ConsoleClient.cpp
//==============================================================================

#include "ConsoleClient.h"
#include "MonkeyMessages/ModeMessages.h"
#include "MonkeyMessages/WaypointMessages.h"
#include "MonkeyMessages/SpeedMessages.h"
#include "MonkeyMessages/PositionMessages.h"

//==============================================================================
ConsoleClient::ConsoleClient()
//==============================================================================
    : _pTransport(NULL)
{}

//------------------------------------------------------------------------------
ConsoleClient::~ConsoleClient()
//------------------------------------------------------------------------------
{}

//------------------------------------------------------------------------------
bool ConsoleClient::command(RemoteMessage &cmd, RemoteMessage &resp)
//------------------------------------------------------------------------------
{
    if( NULL == _pTransport )
    {
        qDebug() << "[ConsoleClient::command] Invalid transport";
        return false;
    }

    // frame it
    std::vector<unsigned char> framedMsg;
    _framer.frame(cmd.bytes(), cmd.size(), framedMsg);

    // send it
    unsigned int bytesWritten = _pTransport->write(framedMsg);
    if( bytesWritten < framedMsg.size() )
    {
        qDebug() << "[ConsoleClient::command] transmit error";
        return false;
    }

    // wait for response
    Grape::IDataPort::Status st = _pTransport->waitForRead(TIMEOUT_MILLISECS);
    if( st != Grape::IDataPort::PORT_OK )
    {
        qDebug() << "[ConsoleClient::command] Error or timeout waiting for reply";
        return false;
    }

    // read the reply
    unsigned int bytesReceived = _pTransport->readAll(framedMsg);

    // check framing
    if( !_framer.isFramed(&framedMsg[0], bytesReceived) )
    {
        qDebug() << "[ConsoleClient::command] Response frame error";
        return false;
    }

    // unframe it
    unsigned int expSz = resp.size();
    unsigned int unframedSz = _framer.computeUnFramedSize(&framedMsg[0], bytesReceived);
    if( unframedSz != expSz )
    {
        qDebug() << "[ConsoleClient::command] Response size error. Expected " << expSz << " got " << unframedSz;
        return false;
    }

    _framer.unframe(&framedMsg[0], bytesReceived, resp.bytes(), unframedSz);

    // validate
    if( !resp.validate())
    {
        qDebug() << "[ConsoleClient::command] Response validation failed";
        return false;
    }

    return true;
}

//------------------------------------------------------------------------------
RemoteMessage::Mode ConsoleClient::setMode(RemoteMessage::Mode mode)
//------------------------------------------------------------------------------
{

    SetModeCommand cmd(mode);
    ModeResponse resp;

    if( !command(cmd, resp) )
    {
        return RemoteMessage::MODE_UNKNOWN;
    }
    return resp.getMode();
}

//------------------------------------------------------------------------------
RemoteMessage::Mode ConsoleClient::getMode()
//------------------------------------------------------------------------------
{

    GetModeCommand cmd;
    ModeResponse resp;

    if( !command(cmd, resp) )
    {
        return RemoteMessage::MODE_UNKNOWN;
    }
    return resp.getMode();
}

//------------------------------------------------------------------------------
bool ConsoleClient::play(bool option)
//------------------------------------------------------------------------------
{
    PlayWpCommand cmd(option);
    PlayWpResponse resp;

    if( !command(cmd, resp) )
    {
        return false;
    }
    return resp.isPlaying();
}

//------------------------------------------------------------------------------
int ConsoleClient::setSpeed(int sp)
//------------------------------------------------------------------------------
{
    SetSpeedScaleCommand cmd(sp);
    SpeedScaleResponse resp;

    if( !command(cmd, resp) )
    {
        return false;
    }
    return resp.getSpeedScale();
}

//------------------------------------------------------------------------------
int ConsoleClient::getSpeed()
//------------------------------------------------------------------------------
{
    GetSpeedScaleCommand cmd;
    SpeedScaleResponse resp;

    if( !command(cmd, resp) )
    {
        return 0;
    }
    return resp.getSpeedScale();
}

//------------------------------------------------------------------------------
int ConsoleClient::getNumWayPoints()
//------------------------------------------------------------------------------
{
    GetNumWpCommand cmd;
    NumWpResponse resp;

    if( !command(cmd, resp) )
    {
        return 0;
    }
    return resp.getNumber();
}

//------------------------------------------------------------------------------
int ConsoleClient::addWayPoint()
//------------------------------------------------------------------------------
{
    RecordWpCommand cmd;
    NumWpResponse resp;

    if( !command(cmd, resp) )
    {
        return false;
    }
    return resp.getNumber();
}

//------------------------------------------------------------------------------
bool ConsoleClient::setHome()
//------------------------------------------------------------------------------
{
    SetHomeCommand cmd;
    PositionResponse resp;
    return command(cmd, resp);
}

//------------------------------------------------------------------------------
int ConsoleClient::clearWayPoints()
//------------------------------------------------------------------------------
{
    ClearAllWpCommand cmd;
    NumWpResponse resp;
    if( !command(cmd, resp) )
    {
        return false;
    }
    return resp.getNumber();
}

//------------------------------------------------------------------------------
void ConsoleClient::setTransport(Grape::IDataPort* pTransport)
//------------------------------------------------------------------------------
{
    if( _pTransport )
    {
        _pTransport->close();
        delete _pTransport;
    }

    _pTransport = pTransport;
}
