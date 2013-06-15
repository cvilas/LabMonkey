//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : ConsoleClient.cpp
//==============================================================================

#include "ConsoleClient.h"
#include "MonkeyMessages/ModeMessages.h"
#include <QDebug>

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
RemoteMessage::Mode ConsoleClient::setMode(RemoteMessage::Mode mode)
//------------------------------------------------------------------------------
{
    // create message
    // frame message and send
    // wait for reply, with timeout
    // unframe
    // validate
    // return

    if( NULL == _pTransport )
    {
        qDebug() << "[ConsoleClient::setMode] Invalid transport";
        return RemoteMessage::MODE_UNKNOWN;
    }

    // create command
    SetModeCommand cmd(mode);

    // frame it
    std::vector<unsigned char> framedMsg;
    _framer.frame(cmd.bytes(), cmd.size(), framedMsg);

    // send it
    unsigned int bytesWritten = _pTransport->write(framedMsg);
    if( bytesWritten < framedMsg.size() )
    {
        qDebug() << "[ConsoleClient::setMode] transmit error";
        return RemoteMessage::MODE_UNKNOWN;
    }

    // wait for response
    Grape::IDataPort::Status st = _pTransport->waitForRead(TIMEOUT_MILLISECS);
    if( st != Grape::IDataPort::PORT_OK )
    {
        qDebug() << "[ConsoleClient::setMode] Error or timeout waiting for reply";
        return RemoteMessage::MODE_UNKNOWN;
    }

    // read the reply
    unsigned int bytesReceived = _pTransport->readAll(framedMsg);

    // check framing
    if( !_framer.isFramed(&framedMsg[0], bytesReceived) )
    {
        qDebug() << "[ConsoleClient::setMode] Response frame error";
        return RemoteMessage::MODE_UNKNOWN;
    }

    // unframe it
    ModeResponse expectedResponse;
    unsigned int unframedSz = _framer.computeUnFramedSize(&framedMsg[0], bytesReceived);
    if( unframedSz != expectedResponse.size() )
    {
        qDebug() << "[ConsoleClient::setMode] Response size error";
        return RemoteMessage::MODE_UNKNOWN;
    }

    _framer.unframe(&framedMsg[0], bytesReceived, expectedResponse.bytes(), unframedSz);

    // validate
    if( !expectedResponse.validate())
    {
        qDebug() << "[ConsoleClient::setMode] Response validation failed";
        return RemoteMessage::MODE_UNKNOWN;
    }

    return expectedResponse.getMode();
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
