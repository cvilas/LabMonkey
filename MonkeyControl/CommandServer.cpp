//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : CommandServer.cpp
//==============================================================================

#include "CommandServer.h"
#include "../MonkeyMessages/RemoteMessages.h"

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

    AppBoard::lcd().locate(AppBoard::DISP_ID_LOC_X, AppBoard::DISP_ID_LOC_Y);
    AppBoard::lcd().printf("I am %s:%d", AppBoard::eth().getIPAddress(), port);

    return true;
}

//-----------------------------------------------------------------------------
int CommandServer::receive(TCPSocketConnection& transport,
                                    unsigned char* buffer, unsigned int bufferLen)
//-----------------------------------------------------------------------------
{
    char sockBuf[256];

    //receive message
    int bytesReceived = transport.receive(sockBuf, sizeof(sockBuf));
    if( bytesReceived < 0 )
    {
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("command receive error");
        return -1;
    }

    if( !_framer.isFramed((const unsigned char*)sockBuf, bytesReceived) )
    {
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("command framing error");
        return -1;
    }

    //unframe
    unsigned int unframedSz = _framer.computeUnFramedSize((const unsigned char*)sockBuf, bytesReceived);
    if( bufferLen < unframedSz )
    {
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("command buffer short");
        return -1;
    }

    _framer.unframe((const unsigned char*)sockBuf, bytesReceived, buffer, bufferLen);

    //validate checksum
    if( buffer[unframedSz-1] != RemoteMessage::computeChecksum(buffer, unframedSz-1) )
    {
        AppBoard::lcd().locate(AppBoard::DISP_ERR_LOC_X, AppBoard::DISP_ERR_LOC_Y);
        AppBoard::lcd().printf("command checksum error");
        return -1;
    }

    return unframedSz;
}

//bool CommandServer::process(std::vector<unsigned char>& cmd, std::vector<unsigned char>& resp)
//{}

//bool CommandServer::reply()
//{}


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


        // message handling sequence:
        // receive
        // check framing
        // unframe
        // validate
        // process the command
        // wait for reply from robot thread
        // frame reply and send


        while(true)
        {
            const unsigned int pCmdBufferLen = 256;
            unsigned char pCmdBuffer[pCmdBufferLen];

            int cmdSz = receive(client, pCmdBuffer, pCmdBufferLen);
            if( cmdSz < 0 )
            {
                continue;
            }

            //process command
            // - read command
            // - set 'desired' section of shared memory
            // - insert command in the 'pending command' queue
            /*
            switch(pCmd[0])
            {
            case RemoteMessage::SET_MODE:
                break;
            case RemoteMessage::SET_SPEED:
                break;
            };*/

            //wait for robot control to reply
            // - if command was in pending queue, wait for reply
            // - read 'actual' section of shared memory

            //reply_all
        }
        client.close();
    }
}
