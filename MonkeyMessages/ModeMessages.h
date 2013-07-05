//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : ModeMessages.h
//==============================================================================

#ifndef MODEMESSAGES_H
#define MODEMESSAGES_H

#include "RemoteMessages.h"

/// \brief Set control mode
/// \ingroup console
///
/// Message format:
/// \code
/// [SET_MODE][1][Mode][CSUM]
/// \endcode
/// \see ModeResponse
class SetModeCommand : public RemoteCommandT<1>
{
public:
    SetModeCommand(RemoteMessage::Mode mode)
    {
        unsigned char d = mode;
        initialise(RemoteMessage::SET_MODE, &d, 1);
    }
    virtual ~SetModeCommand() {}
}; // SetModeCommand

/// \brief Get currently active control mode
/// \ingroup console
///
/// Message format:
/// \code
/// [GET_MODE][0][CSUM]
/// \endcode
/// \see ModeResponse
class GetModeCommand : public RemoteCommandT<0>
{
public:
    GetModeCommand() { initialise(RemoteMessage::GET_MODE, NULL, 0); }
    virtual ~GetModeCommand() {}
}; // GetModeCommand


/// \brief Response from robot after SetModeCommand or GetModeCommand
/// \ingroup console
///
/// Message format:
/// \code
/// [GET_MODE][1][Mode][CSUM]
/// \endcode
/// \see SetModeCommand, GetModeCommand
class ModeResponse : public RemoteResponseT<1>
{
public:
    ModeResponse()
    {
        unsigned char d = RemoteMessage::MODE_UNKNOWN;
        initialise(RemoteMessage::GET_MODE, &d, 1);
    }
    virtual ~ModeResponse() {}

    /// Calls base class method of same name and additionally
    /// verifies message ID is correct.
    virtual bool validate()
    {
        return RemoteResponseT::validate() && (id() == RemoteMessage::GET_MODE);
    }

    RemoteMessage::Mode getMode() { return (RemoteMessage::Mode)_bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1]; }
    void setMode(RemoteMessage::Mode m) { _bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1] = m; setModified(); }
}; // ModeResponse

#endif // MODEMESSAGES_H
