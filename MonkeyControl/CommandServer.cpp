//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : CommandServer.cpp
//==============================================================================

#include "CommandServer.h"
#include "../MonkeyMessages/RemoteMessages.h"
#include "../MonkeyMessages/ModeMessages.h"
#include "../MonkeyMessages/SpeedMessages.h"
#include "../MonkeyMessages/PositionMessages.h"
#include "../MonkeyMessages/WaypointMessages.h"

//==============================================================================
bool CommandServer::init(int port)
//==============================================================================
{
    if( 0 != _server.bind(port) )
    {
        AppBoard::logStream().printf("[CommandServer::init] Server bind error\n");
        return false;
    }

    if( 0 != _server.listen() )
    {
        AppBoard::logStream().printf("[CommandServer::init] Server listen error\n");
        return false;
    }

    AppBoard::lcd().locate(AppBoard::DISP_INFO_LOC_X, AppBoard::DISP_INFO_LOC_Y);
    AppBoard::lcd().printf("I am %s:%d", AppBoard::eth().getIPAddress(), port);
    AppBoard::logStream().printf("[CommandServer::init] I am %s:%d\n", AppBoard::eth().getIPAddress(), port);

    return true;
}

//------------------------------------------------------------------------------
void CommandServer::run()
//------------------------------------------------------------------------------
{
    while(true)
    {
        TCPSocketConnection client;
        _server.accept(client);
        client.set_blocking(true);

        AppBoard::logStream().printf("Connected to %s\n", client.get_address());

        _robot.setConsoleActive(true);

        while(true)
        {
            const unsigned int cmdBufferLen = 256;
            unsigned char pCmdBuffer[cmdBufferLen];

            int cmdSz = receive(client, pCmdBuffer, cmdBufferLen);
            if( cmdSz == COMM_ERROR )
            {
                break;
            }
            else if (cmdSz == MSG_ERROR )
            {
                continue;
            }

            const unsigned int respBufLen = 256;
            unsigned char pRespBuffer[respBufLen];

            unsigned int respLen = process(pCmdBuffer, cmdSz, pRespBuffer, respBufLen);
            if( respLen == COMM_ERROR )
            {
                break;
            }
            else if (respLen == MSG_ERROR || respLen == IPC_ERROR )
            {
                continue;
            }

            int ret = respond(client, pRespBuffer, respLen);
            if( ret == COMM_ERROR )
            {
                break;
            }
            else if (ret == MSG_ERROR )
            {
                continue;
            }

        }
        client.close();
        _robot.setConsoleActive(false);
        AppBoard::logStream().printf("Disconnected from console\n");        
    }
}

//-----------------------------------------------------------------------------
int CommandServer::receive(TCPSocketConnection& transport, unsigned char* buffer, unsigned int bufferLen)
//-----------------------------------------------------------------------------
{
    char sockBuf[256];

    //receive message
    int bytesReceived = transport.receive(sockBuf, sizeof(sockBuf));
    if( bytesReceived < 0 )
    {
        AppBoard::logStream().printf("[CommandServer::receive] command receive error\n");
        return COMM_ERROR;
    }

    if( !_framer.isFramed((const unsigned char*)sockBuf, bytesReceived) )
    {
        AppBoard::logStream().printf("[CommandServer::receive] command framing error\n");
        return MSG_ERROR;
    }

    //unframe
    unsigned int unframedSz = _framer.computeUnFramedSize((const unsigned char*)sockBuf, bytesReceived);
    if( bufferLen < unframedSz )
    {
        AppBoard::logStream().printf("[CommandServer::receive] command buffer short\n");
        return MSG_ERROR;
    }

    _framer.unframe((const unsigned char*)sockBuf, bytesReceived, buffer, bufferLen);

    //validate checksum
    if( buffer[unframedSz-1] != RemoteMessage::computeChecksum(buffer, unframedSz-1) )
    {
        AppBoard::logStream().printf("[CommandServer::receive] command checksum error\n");
        return MSG_ERROR;
    }

    return unframedSz;
}

//-----------------------------------------------------------------------------
int CommandServer::respond(TCPSocketConnection& transport, unsigned char* buffer, unsigned int len)
//-----------------------------------------------------------------------------
{
    char sockBuf[256];

    // frame
    unsigned int szFramed = _framer.computeFramedSize(buffer, len);
    if( szFramed > sizeof(sockBuf) )
    {
        AppBoard::logStream().printf("[CommandServer::respond] framed response too long\n");
        return MSG_ERROR;
    }

    _framer.frame(buffer, len, (unsigned char*)sockBuf, sizeof(sockBuf));

    // send
    if( szFramed != transport.send(sockBuf, szFramed) )
    {
        AppBoard::logStream().printf("[CommandServer::respond] response send error\n");
        return COMM_ERROR;
    }

    return 0;
}

//-----------------------------------------------------------------------------
int CommandServer::process(unsigned char* pCmd, unsigned int cmdLen,
                           unsigned char* pRespBuf, unsigned int respBufLen)
//-----------------------------------------------------------------------------
{
    int respLen = 0;

    switch( pCmd[RemoteMessage::ID_INDEX] )
    {
    case RemoteMessage::SET_MODE:
        respLen = processSetMode(pCmd, cmdLen, pRespBuf, respBufLen);
        break;
    case RemoteMessage::GET_MODE:
        respLen = processGetMode(pRespBuf, respBufLen);
        break;
    case RemoteMessage::SET_SPEED:
        respLen = processSetSpeed(pCmd, cmdLen, pRespBuf, respBufLen);
        break;
    case RemoteMessage::GET_SPEED:
        respLen = processGetSpeed(pRespBuf, respBufLen);
        break;
    case RemoteMessage::SET_POSITION_HOME:
        respLen = processSetHome(pCmd, cmdLen, pRespBuf, respBufLen);
        break;
    case RemoteMessage::SET_POSITION:
        respLen = processSetPosition(pCmd, cmdLen, pRespBuf, respBufLen);
        break;
    case RemoteMessage::GET_POSITION:
        respLen = processGetPosition(pRespBuf, respBufLen);
        break;
    case RemoteMessage::PLAY_WP:
        respLen = processPlayWp(pCmd, cmdLen, pRespBuf, respBufLen);
        break;
    case RemoteMessage::RECORD_WP:
        respLen = processRecWp(pCmd, cmdLen, pRespBuf, respBufLen);
        break;
    case RemoteMessage::GET_NUM_WP:
        respLen = processGetNumWp(pRespBuf, respBufLen);
    default:
        break;
    }

    return respLen;
}

//-----------------------------------------------------------------------------
int CommandServer::processSetMode(unsigned char* pCmd, unsigned int cmdLen,
                                  unsigned char* pRespBuf, unsigned int respBufLen)
//-----------------------------------------------------------------------------
{
    _robot.setMode( (RemoteMessage::Mode)(pCmd[RemoteMessage::PAYLOAD_LENGTH_INDEX+1]) );
    return processGetMode(pRespBuf, respBufLen);
}

//-----------------------------------------------------------------------------
int CommandServer::processGetMode(unsigned char* pRespBuf, unsigned int respBufLen)
//-----------------------------------------------------------------------------
{
    ModeResponse resp;

    unsigned int sz = resp.size();

    if( sz > respBufLen )
    {
        AppBoard::logStream().printf("get mode: resp buff short\n");
        return MSG_ERROR;
    }

    resp.setMode( _robot.getMode() );

    memcpy( pRespBuf, resp.bytes(), sz );

    return sz;
}

//-----------------------------------------------------------------------------
int CommandServer::processSetSpeed(unsigned char* pCmd, unsigned int cmdLen,
                                  unsigned char* pRespBuf, unsigned int respBufLen)
//-----------------------------------------------------------------------------
{
    _robot.setSpeed( (int)(pCmd[RemoteMessage::PAYLOAD_LENGTH_INDEX+1]) );
    return processGetSpeed(pRespBuf, respBufLen);
}

//-----------------------------------------------------------------------------
int CommandServer::processGetSpeed(unsigned char* pRespBuf, unsigned int respBufLen)
//-----------------------------------------------------------------------------
{
    SpeedResponse resp;

    unsigned int sz = resp.size();

    if( sz > respBufLen )
    {
        AppBoard::logStream().printf("get speed: resp buff short\n");
        return MSG_ERROR;
    }

    resp.setSpeed( _robot.getSpeed() );

    memcpy( pRespBuf, resp.bytes(), sz );

    return sz;
}

//-----------------------------------------------------------------------------
int CommandServer::processSetHome(unsigned char* pCmd, unsigned int cmdLen, unsigned char* pRespBuf, unsigned int respBufLen)
//-----------------------------------------------------------------------------
{
    _robot.onHomeBtn();
    return processGetPosition(pRespBuf, respBufLen);
}

//-----------------------------------------------------------------------------
int CommandServer::processSetPosition(unsigned char* pCmd, unsigned int cmdLen, unsigned char* pRespBuf, unsigned int respBufLen)
//-----------------------------------------------------------------------------
{
    int p[LabMonkey::NUM_JOINTS];
    int32_t* pJoints = (int32_t*)pCmd;
    for(int i = 0; i < 5; ++i)
    {
        p[i] = pJoints[i];
    }
    _robot.setPosition(p);
    return processGetPosition(pRespBuf, respBufLen);
}

//-----------------------------------------------------------------------------
int CommandServer::processGetPosition(unsigned char* pRespBuf, unsigned int respBufLen)
//-----------------------------------------------------------------------------
{
    PositionResponse resp;

    unsigned int sz = resp.size();

    if( sz > respBufLen )
    {
        AppBoard::logStream().printf("get position: resp buff short\n");
        return MSG_ERROR;
    }

    int p[LabMonkey::NUM_JOINTS];
    _robot.getMonkey().getPosition(p);

    for(int i = 0; i < LabMonkey::NUM_JOINTS; ++i)
    {
        resp.setPosition(i, p[i]);
    }

    memcpy( pRespBuf, resp.bytes(), sz );

    return sz;
}

//-----------------------------------------------------------------------------
int CommandServer::processPlayWp(unsigned char* pCmd, unsigned int cmdLen,
                                 unsigned char* pRespBuf, unsigned int respBufLen)
//-----------------------------------------------------------------------------
{
    _robot.play( (pCmd[RemoteMessage::PAYLOAD_LENGTH_INDEX+1] != 0) );
    return processGetNumWp(pRespBuf, respBufLen);
}

//-----------------------------------------------------------------------------
int CommandServer::processRecWp(unsigned char* pCmd, unsigned int cmdLen,
                                unsigned char* pRespBuf, unsigned int respBufLen)
//-----------------------------------------------------------------------------
{
    _robot.onRecBtn();
    return processGetNumWp(pRespBuf, respBufLen);
}

//-----------------------------------------------------------------------------
int CommandServer::processGetNumWp(unsigned char* pRespBuf, unsigned int respBufLen)
//-----------------------------------------------------------------------------
{
    NumWpResponse resp;

    unsigned int sz = resp.size();

    if( sz > respBufLen )
    {
        AppBoard::logStream().printf("get num waypoints: resp buff short\n");
        return MSG_ERROR;
    }

    resp.setNumber( _robot.getNumWayPoints() );

    memcpy( pRespBuf, resp.bytes(), sz );

    return sz;
}



