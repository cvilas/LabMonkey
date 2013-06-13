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
    if( !_eth.init() ) // use dhcp
    {
        _lcd.printf("\n[CommandServer] ethernet init error");
        return false;
    }

    if( !_eth.connect() )
    {
        _lcd.printf("\n[CommandServer] ethernet connect error");
        return false;
    }

    _lcd.printf("\n[CommandServer] IP: %s", _eth.getIPAddress());

    if( !_server.bind(port) )
    {
        _lcd.printf("\n[CommandServer] server bind error");
        return false;
    }

    if( !_server.listen() )
    {
        _lcd.printf("\n[CommandServer] server listen error");
        return false;
    }

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

        _lcd.printf("\n[CommandServer] Connection from: %s\n", client.get_address());

        char sockBuf[256];

        while(true)
        {
            //receive message
            int bytesReceived = client.receive_all(sockBuf, sizeof(sockBuf));
            if( bytesReceived < 0 )
            {
                // receive error;
            }

            if( !_framer.isFramed((const unsigned char*)sockBuf, bytesReceived) )
            {
                // framing error
            }

            //unframe
            std::vector<unsigned char> command;
            _framer.unframe((const unsigned char*)sockBuf, bytesReceived, command);

            //validate
            unsigned int csz = command.size();
            unsigned char* pCmd = &command[0];
            if( pCmd[csz-1] != RemoteMessage::computeChecksum(pCmd, csz-1) )
            {
                // command checksum error
            }

            //process command
            // - read command
            // - set 'desired' section of shared memory
            // - insert command in the 'pending command' queue
            switch(pCmd[0])
            {
            case RemoteMessage::SET_MODE:
                break;
            case RemoteMessage::SET_SPEED:
                break;
            };

            //wait for robot control to reply
            // - if command was in pending queue, wait for reply
            // - read 'actual' section of shared memory

            //reply_all
        }
    }
}
