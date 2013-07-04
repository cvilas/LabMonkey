//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : CommandServer.cpp
//==============================================================================

#include "CommandServer.h"
#include "../MonkeyMessages/RemoteMessages.h"
#include "../MonkeyMessages/ModeMessages.h"

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

        AppBoard::setConsoleActive(true);
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
        AppBoard::logStream().printf("Disconnected from console\n");
        AppBoard::setConsoleActive(false);
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
    // - read command
    // - set 'desired' section of shared memory
    // - insert command in the 'pending command' queue

    //wait for robot control to reply
    // - if command was in pending queue, wait for reply
    // - read 'actual' section of shared memory

    int respLen = 0;

    switch( pCmd[RemoteMessage::ID_INDEX] )
    {
    case RemoteMessage::SET_MODE:
        respLen = processSetMode(pCmd, cmdLen, pRespBuf, respBufLen);
        break;
    case RemoteMessage::GET_MODE:
        respLen = processGetMode(pRespBuf, respBufLen);
        break;
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
    ModeResponse resp;
    unsigned int sz = resp.size();

    if( sz > respBufLen )
    {
        AppBoard::logStream().printf("set mode: resp buff short\n");
        return MSG_ERROR;
    }

    // set desired state
    DesiredState& desiredState = AppBoard::desiredState();
    desiredState.lock.lock();
    desiredState.mode = (RemoteMessage::Mode)(pCmd[RemoteMessage::PAYLOAD_LENGTH_INDEX+1]);
    desiredState.lock.unlock();

    // notify robot controller
    RemoteMessage::MessageID* id = new RemoteMessage::MessageID;
    *id = RemoteMessage::SET_MODE;
    if( osOK != AppBoard::pendingCommand().put(id,1000) )
    {
        AppBoard::logStream().printf("set mode: ipc put error\n");
        return IPC_ERROR;
    }

    // wait for robot controller to act on it
    osEvent evt = AppBoard::pendingReply().get(10000);
    if( evt.status != osEventMessage )
    {
        AppBoard::logStream().printf("set mode: ipc get error\n");
        return IPC_ERROR;
    }

    // check if reply is for the command we sent
    RemoteMessage::MessageID* evId = (RemoteMessage::MessageID*)evt.value.p;
    bool b = (*evId == RemoteMessage::SET_MODE);
    delete evId;
    if( !b )
    {
        AppBoard::logStream().printf("set mode: ipc id error\n");
        return IPC_ERROR;
    }

    // get updated state
    CurrentState& state = AppBoard::currentState();
    state.lock.lock();
    resp.setMode(state.mode);
    state.lock.unlock();

    // copy state value and return
    memcpy( pRespBuf, resp.bytes(), sz );

    return sz;
}

//-----------------------------------------------------------------------------
int CommandServer::processGetMode(unsigned char* pRespBuf, unsigned int respBufLen)
//-----------------------------------------------------------------------------
{
    ModeResponse resp;

    unsigned int sz = resp.size();

    if( sz > respBufLen )
    {
        return MSG_ERROR;
    }

    CurrentState& state = AppBoard::currentState();
    state.lock.lock();
    resp.setMode(state.mode);
    state.lock.unlock();

    memcpy( pRespBuf, resp.bytes(), sz );

    return sz;
}


