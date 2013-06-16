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
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("\nServer bind error");
        return false;
    }

    if( 0 != _server.listen() )
    {
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("\nServer listen error");
        return false;
    }

    AppBoard::lcd().locate(AppBoard::DISP_INFO_LOC_X, AppBoard::DISP_INFO_LOC_Y);
    AppBoard::lcd().printf("I am %s:%d", AppBoard::eth().getIPAddress(), port);

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

        AppBoard::lcd().locate(AppBoard::DISP_INFO_LOC_X, AppBoard::DISP_INFO_LOC_Y);
        AppBoard::lcd().printf("\nConnected to %s\n", client.get_address());

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
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("command receive error");
        return COMM_ERROR;
    }

    if( !_framer.isFramed((const unsigned char*)sockBuf, bytesReceived) )
    {
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("command framing error");
        return MSG_ERROR;
    }

    //unframe
    unsigned int unframedSz = _framer.computeUnFramedSize((const unsigned char*)sockBuf, bytesReceived);
    if( bufferLen < unframedSz )
    {
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("command buffer short");
        return MSG_ERROR;
    }

    _framer.unframe((const unsigned char*)sockBuf, bytesReceived, buffer, bufferLen);

    //validate checksum
    if( buffer[unframedSz-1] != RemoteMessage::computeChecksum(buffer, unframedSz-1) )
    {
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("command checksum error");
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
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("framed response too long");
        return MSG_ERROR;
    }

    _framer.frame(buffer, len, (unsigned char*)sockBuf, sizeof(sockBuf));

    // send
    if( szFramed != transport.send(sockBuf, szFramed) )
    {
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("response send error");
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
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("set mode: resp buff short");
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
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("set mode: ipc put error");
        return IPC_ERROR;
    }

    // wait for robot controller to act on it
    osEvent evt = AppBoard::pendingReply().get(10000);
    if( evt.status != osEventMessage )
    {
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("set mode: ipc get error");
        return IPC_ERROR;
    }

    // check if reply is for the command we sent
    RemoteMessage::MessageID* evId = (RemoteMessage::MessageID*)evt.value.p;
    bool b = (*evId == RemoteMessage::SET_MODE);
    delete evId;
    if( !b )
    {
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("set mode: ipc id error");
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



