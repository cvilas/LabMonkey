//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : WaypointMessages.h
//==============================================================================

#ifndef WAYPOINTMESSAGES_H
#define WAYPOINTMESSAGES_H

#include "RemoteMessages.h"

/// \brief Play waypoints
/// \ingroup console
///
/// Message format:
/// \code
/// [PLAY_WP][1][0:stop, 1:play[CSUM]
/// \endcode
/// \see NumWpResponse
class PlayWpCommand : public RemoteCommandT<1>
{
public:
    PlayWpCommand(bool option)
    {
        unsigned char d = ( option ? 1 : 0);
        initialise(RemoteMessage::PLAY_WP, &d, 1);
    }

    bool getOption() { return (_bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1] != 0); }

    virtual ~PlayWpCommand() {}
}; // PlayWpCommand

/// \brief Record a waypoint
/// \ingroup console
///
/// Message format:
/// \code
/// [RECORD_WP][0][CSUM]
/// \endcode
/// \see NumWpResponse
class RecordWpCommand : public RemoteCommandT<0>
{
public:
    RecordWpCommand()
    {
        initialise(RemoteMessage::RECORD_WP, NULL, 0);
    }
    virtual ~RecordWpCommand() {}
}; // RecordWpCommand


/// \brief Number of waypoints Response
/// \ingroup console
///
/// Message format:
/// \code
/// [GET_NUM_WP][1][Mode][CSUM]
/// \endcode
class NumWpResponse : public RemoteResponseT<1>
{
public:
    NumWpResponse()
    {
        initialise(RemoteMessage::GET_NUM_WP, NULL, 0);
    }
    virtual ~NumWpResponse() {}

    /// Calls base class method of same name and additionally
    /// verifies message ID is correct.
    virtual bool validate()
    {
        return RemoteResponseT::validate() && (id() == RemoteMessage::GET_NUM_WP);
    }

    int getNumber() { return _bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1]; }
    void setNumber(int n) { _bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1] = n; }

}; // NumWpResponse

#endif // WAYPOINTMESSAGES_H
