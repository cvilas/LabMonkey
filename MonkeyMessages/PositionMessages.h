//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : PositionMessages.h
//==============================================================================

#ifndef POSITIONMESSAGES_H
#define POSITIONMESSAGES_H

#include "RemoteMessages.h"
#include <inttypes.h>

/// \brief Set home
/// \ingroup console
///
/// Message format:
/// \code
/// [SET_HOME][0][CSUM]
/// \endcode
/// \see PositionResponse
class SetHomeCommand : public RemoteCommandT<0>
{
public:
    SetHomeCommand()
    {
        initialise(RemoteMessage::SET_POSITION_HOME, NULL, 0);
    }
    virtual ~SetHomeCommand() {}
}; // SetHomeCommand

/// \brief Set encoders at current position
/// \ingroup console
///
/// Message format:
/// \code
/// [SET_POSITION][20][5 * ints][CSUM]
/// \endcode
/// \see PositionResponse
class SetPositionCommand : public RemoteCommandT<20>
{
public:
    SetPositionCommand()
    {
        initialise(RemoteMessage::SET_POSITION, NULL, 0);
    }
    virtual ~SetPositionCommand() {}

    int getPosition(int index)
    {
        int i = 4*index;
        if( i < payloadLength() )
        {
            int32_t* p = (int32_t*)&(_bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1+i]);
            return *p;
        }
        return 0;
    }

    void setPosition(int index, int sp)
    {
        int i = 4*index;
        if( i < payloadLength() )
        {
            int32_t* p = (int32_t*)&(_bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1+i]);
            *p = sp;
            setModified();
        }
    }
}; // SetPositionCommand

/// \brief Get position
/// \ingroup console
///
/// Message format:
/// \code
/// [GET_POSITION][0][CSUM]
/// \endcode
/// \see PositionResponse
class GetPositionCommand : public RemoteCommandT<0>
{
public:
    GetPositionCommand() { initialise(RemoteMessage::GET_POSITION, NULL, 0); }
    virtual ~GetPositionCommand() {}
}; // GetPositionCommand

/// \brief Response from robot after SetHomeCommand, SetPositionCommand or GetPositionCommand
/// \ingroup console
///
/// Message format:
/// \code
/// [GET_POSITION][20][5 * ints][CSUM]
/// \endcode
/// Five 4-byte integers are returned
/// \see SetHomeCommand, SetPositionCommand, GetPositionCommand
class PositionResponse : public RemoteResponseT<20>
{
public:
    PositionResponse()
    {
        initialise(RemoteMessage::GET_POSITION, NULL, 0);
    }
    virtual ~PositionResponse() {}

    /// Calls base class method of same name and additionally
    /// verifies message ID is correct.
    virtual bool validate()
    {
        return RemoteResponseT::validate() && (id() == RemoteMessage::GET_POSITION);
    }

    int getPosition(int index)
    {
        int i = 4*index;
        if( i < payloadLength() )
        {
            int32_t* p = (int32_t*)&(_bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1+i]);
            return *p;
        }
        return 0;
    }

    void setPosition(int index, int sp)
    {
        int i = 4*index;
        if( i < payloadLength() )
        {
            int32_t* p = (int32_t*)&(_bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX+1+i]);
            *p = sp;
            setModified();
        }
    }
}; // PositionResponse

#endif // POSITIONMESSAGES_H
