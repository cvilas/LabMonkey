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
/// [START_BYTE][SET_MODE][1][Mode][CSUM][END_BYTE]
/// \endcode
/// \see ModeResponse
class SetModeCommand : public RemoteCommandT<1>
{
public:
    SetModeCommand(RemoteMessage::Mode mode)
    {
        unsigned char d = mode;
        initialise(RemoteMessage::SET_MODE, 1, &d);
    }
    virtual ~SetModeCommand() {}
}; // SetModeCommand

/// \brief Get currently active control mode
/// \ingroup console
///
/// Message format:
/// \code
/// [START_BYTE][GET_MODE][0][CSUM][END_BYTE]
/// \endcode
/// \see ModeResponse
class GetModeCommand : public RemoteCommandT<0>
{
public:
    GetModeCommand() { initialise(RemoteMessage::GET_MODE, 0, NULL); }
    ~GetModeCommand() {}
}; // GetModeCommand


/// \brief Response from robot after SetModeCommand or GetModeCommand
/// \ingroup console
///
/// Message format:
/// \code
/// [START_BYTE][SET_MODE or GET_MODE][1][Mode][CSUM][END_BYTE]
/// \endcode
/// \see SetModeCommand, GetModeCommand
class ModeResponse : public RemoteResponseT<1>
{
public:
    ModeResponse() {}
    virtual ~ModeResponse() {}

    /// Calls base class method of same name and additionally
    /// verifies message ID is correct.
    virtual bool validate()
    {
        RemoteMessage::MessageID id = id();
        return RemoteResponseT<1>::validate() &&
                ( (id == RemoteMessage::SET_MODE) || (id == RemoteMessage::GET_MODE));
    }

    RemoteMessage::Mode mode() { return _bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1]; }
}; // ModeResponse

#endif // MODEMESSAGES_H
