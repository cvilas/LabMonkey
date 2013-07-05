//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : SpeedMessages.h
//==============================================================================

#ifndef SPEEDMESSAGES_H
#define SPEEDMESSAGES_H

#include "RemoteMessages.h"

/// \brief Set speed
/// \ingroup console
///
/// Message format:
/// \code
/// [SET_SPEED][1][speed][CSUM]
/// \endcode
/// \see SpeedResponse
class SetSpeedCommand : public RemoteCommandT<1>
{
public:
    SetSpeedCommand(int speed)
    {
        if( speed < 0 ) speed = 0;
        if( speed > 100 ) speed = 100;

        unsigned char d = speed;
        initialise(RemoteMessage::SET_SPEED, &d, 1);
    }
    virtual ~SetSpeedCommand() {}
}; // SetSpeedCommand

/// \brief Get current global speed
/// \ingroup console
///
/// Message format:
/// \code
/// [GET_SPEED][0][CSUM]
/// \endcode
/// \see SpeedResponse
class GetSpeedCommand : public RemoteCommandT<0>
{
public:
    GetSpeedCommand() { initialise(RemoteMessage::GET_SPEED, NULL, 0); }
    virtual ~GetSpeedCommand() {}
}; // GetSpeedCommand


/// \brief Response from robot after SetSpeedCommand or GetSpeedCommand
/// \ingroup console
///
/// Message format:
/// \code
/// [GET_SPEED][1][Mode][CSUM]
/// \endcode
/// \see SetSpeedCommand, GetSpeedCommand
class SpeedResponse : public RemoteResponseT<1>
{
public:
    SpeedResponse()
    {
        unsigned char d = 0xFF;
        initialise(RemoteMessage::GET_SPEED, &d, 1);
    }
    virtual ~SpeedResponse() {}

    /// Calls base class method of same name and additionally
    /// verifies message ID is correct.
    virtual bool validate()
    {
        return RemoteResponseT::validate() && (id() == RemoteMessage::GET_SPEED);
    }

    int getSpeed() { return _bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1]; }
    void setSpeed(int sp)
    {
        _bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1] = sp;
        setModified();
    }
}; // SpeedResponse

#endif // SPEEDMESSAGES_H
