//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : SpeedMessages.h
//==============================================================================

#ifndef SPEEDMESSAGES_H
#define SPEEDMESSAGES_H

#include "RemoteMessages.h"

/// \brief Set speed scale factor
/// \ingroup console
///
/// Message format:
/// \code
/// [SET_SPEED][1][speed][CSUM]
/// \endcode
/// \see SpeedResponse
class SetSpeedScaleCommand : public RemoteCommandT<1>
{
public:
    SetSpeedScaleCommand(int speed)
    {
        if( speed < 1 ) speed = 1;
        if( speed > 10 ) speed = 10;

        unsigned char d = speed;
        initialise(RemoteMessage::SET_SPEED, &d, 1);
    }
    virtual ~SetSpeedScaleCommand() {}
}; // SetSpeedCommand

/// \brief Get current global speed
/// \ingroup console
///
/// Message format:
/// \code
/// [GET_SPEED][0][CSUM]
/// \endcode
/// \see SpeedResponse
class GetSpeedScaleCommand : public RemoteCommandT<0>
{
public:
    GetSpeedScaleCommand() { initialise(RemoteMessage::GET_SPEED, NULL, 0); }
    virtual ~GetSpeedScaleCommand() {}
}; // GetSpeedCommand


/// \brief Response from robot after SetSpeedCommand or GetSpeedCommand
/// \ingroup console
///
/// Message format:
/// \code
/// [GET_SPEED][1][Mode][CSUM]
/// \endcode
/// \see SetSpeedCommand, GetSpeedCommand
class SpeedScaleResponse : public RemoteResponseT<1>
{
public:
    SpeedScaleResponse()
    {
        unsigned char d = 0xFF;
        initialise(RemoteMessage::GET_SPEED, &d, 1);
    }
    virtual ~SpeedScaleResponse() {}

    /// Calls base class method of same name and additionally
    /// verifies message ID is correct.
    virtual bool validate()
    {
        return RemoteResponseT::validate() && (id() == RemoteMessage::GET_SPEED);
    }

    int getSpeedScale() { return _bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1]; }
    void setSpeedScale(int sp)
    {
        _bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1] = sp;
        setModified();
    }
}; // SpeedResponse

#endif // SPEEDMESSAGES_H
