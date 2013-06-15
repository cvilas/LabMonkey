//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : CommandServer.h
// Brief    : Serves commands from the remote console
//==============================================================================

#ifndef COMMAND_SERVER_H
#define COMMAND_SERVER_H

#include "AppBoard.h"
#include "MessageFramer.h"

/// \brief mbed based server for the remote console
/// \ingroup console
///
/// Instantiate this class in a separate thread and call run() to start processing
/// remote commands.
///
/// The server does the following
/// - Creates a TCP server on specified port
/// - Accepts commands from remote console
/// - Deframes the TCP packets
/// - Interprets the command and notifies the robot controller thread via
///   the common message queue
/// - Waits for robot controller thread to respond
/// - Replies to the remote console when a response becomes available
class CommandServer
{
public:
    CommandServer() {}
    ~CommandServer() {}

    /// Initialise server and bind to specified port
    bool init(int port);

    /// receive message
    /// \param transport data transfer mechanism
    /// \param buffer buffer to put message in
    /// \param bufferLen length of above buffer
    /// \return -1 on error. Number of bytes in the message on success
    int receive(TCPSocketConnection& transport, unsigned char* buffer, unsigned int bufferLen);

    /// Start the server loop
    void run();

private:
    TCPSocketServer _server;
    MessageFramer   _framer;
};


#endif // COMMAND_SERVER_H
