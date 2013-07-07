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
#include "RobotController.h"

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
/// - Interprets the command and notifies the robot controller thread
/// - Waits for robot controller thread to respond
/// - Replies to the remote console when a response becomes available
class CommandServer
{
public:
    static const int COMM_ERROR = -1;   //!< console transport error
    static const int MSG_ERROR = -2;    //!< console message format error
    static const int IPC_ERROR = -3;    //!< error in internal message passing between threads

public:
    CommandServer(RobotController& rc) : _robot(rc) {}
    ~CommandServer() {}

    /// Initialise server and bind to specified port
    bool init(int port);

    /// receive message from remote host
    /// \param transport data transfer mechanism
    /// \param buffer buffer to put message in
    /// \param bufferLen length of above buffer
    /// \return COMM_ERROR, MSG_ERROR, or number of valid bytes in the message
    int receive(TCPSocketConnection& transport, unsigned char* buffer, unsigned int bufferLen);

    /// process and command
    /// \param pCmd command buffer
    /// \param cmdLen length of the command
    /// \param pRespBuf response buffer
    /// \param respBufLen size of the above buffer
    /// \return -1 on error, number of valid bytes in response buffer on success.
    int process(unsigned char* pCmd, unsigned int cmdLen, unsigned char* pRespBuf, unsigned int respBufLen);

    /// respond to remote host
    /// \param transport data transfer mechanism
    /// \param buffer buffer containing response
    /// \param len length of the response
    /// \return COMM_ERROR, MSG_ERROR, or 0 on success
    int respond(TCPSocketConnection& transport, unsigned char* buffer, unsigned int len);

    /// Start the server loop
    void run();

private:
    int processGetMode(unsigned char* pRespBuf, unsigned int respBufLen);
    int processGetSpeed(unsigned char* pRespBuf, unsigned int respBufLen);

    int processSetPosition(unsigned char* pCmd, unsigned int cmdLen, unsigned char* pRespBuf, unsigned int respBufLen);
    int processGetPosition(unsigned char* pRespBuf, unsigned int respBufLen);

    int processPlayWp(unsigned char* pCmd, unsigned int cmdLen, unsigned char* pRespBuf, unsigned int respBufLen);
    int processGetNumWp(unsigned char* pRespBuf, unsigned int respBufLen);

private:
    TCPSocketServer _server;
    MessageFramer   _framer;
    RobotController& _robot;
};


#endif // COMMAND_SERVER_H
