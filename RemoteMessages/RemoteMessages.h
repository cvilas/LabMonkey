//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : RemoteMessages.h
// Brief    : Remote control messages for Lab Monkey
//==============================================================================

#ifndef REMOTEMESSAGES_H
#define REMOTEMESSAGES_H

// Note: This is a standalone header common to desktop/tablet based hmi and
// mbed based robot controller.

//==============================================================================
/// \brief Base class interface for all remote control messages
/// \ingroup console
///
/// RemoteMessage defines an interface for messages between remote console and robot
/// but does not deal with memory. See RemoteMessageT.
///
/// Remote console messages are formatted as follows
/// \code
/// [START_BYTE][ID][N][.... payload ....][CSUM][END_BYTE]
/// \endcode
/// where
/// - START_BYTE (1 byte): 0xAA
/// - ID (1 byte): One of the codes from RemoteMessage::MessageID
/// - N (1 byte): number of bytes in the payload
/// - payload (N bytes): The payload bytes
/// - CSUM (1 byte): Checksum computed over the entire message except START_BYTE and END_BYTE
/// - END_BYTE (1 byte): 0xFF
class RemoteMessage
{
public:
    enum MessageID
    {
        ID_UNKNOWN = 0,
        SET_MODE,           //!< Set mode (See Mode)
        GET_MODE,           //!< Get currently active mode
        SET_SPEED,          //!< Set global speed
        GET_SPEED,          //!< Get currently active global speed
        GOTO_POSITION,      //!< Go to a position specified by list of joint angles
        GET_POSITION,       //!< Get current position specified by list of joint angles
        GET_NUM_WAYPOINTS,  //!< Get the number of active waypoints
        SET_WAPOINT,        //!< Set a way point (list of joint angle lists)
        GET_WAYPOINT,       //!< Get a way point
        ID_MAX
    };

    enum Mode
    {
        MODE_UNKNOWN = 0,
        MODE_TEACH,         //!< Teach waypoints
        MODE_REPLAY,        //!< Replay taught waypoints
        MODE_MAX
    };

public:
    static const unsigned int START_BYTE            = 0xAA;
    static const unsigned int END_BYTE              = 0xFF;

    static const unsigned int ID_INDEX              = 1;
    static const unsigned int PAYLOAD_LENGTH_INDEX  = 2;

public:
    virtual unsigned char* bytes() const = 0;
    virtual unsigned int size() const = 0;

protected:
    RemoteMessage();
    virtual ~RemoteMessage() {}
    inline unsigned char computeChecksum();
}; // RemoteMessage

//==============================================================================
/// \brief Base class for all remote control messages
/// \ingroup console
///
/// All remote comms messages are derived from this template class. (We use
/// templated message buffer to force a design where message sizes are known at
/// compile time, avoiding memory allocation).
template<unsigned int nPayloadBytes>
class RemoteMessageT : public RemoteMessage
{
public:

    virtual unsigned char* bytes() const { return _bytes; }
    virtual unsigned int size() const { return nPayloadBytes + 5; }
    unsigned int payloadLength() const { return nPayloadBytes; }

    MessageID id() const { return MessageID(_bytes[ID_INDEX]&0xFF); }
    unsigned char csum() const { return _bytes[size()-2]&0xFF; }

protected:
    RemoteMessageT() : RemoteMessage() {}
    virtual ~RemoteMessageT() {}

protected:
    unsigned char _bytes[nPayloadBytes + 5/*START, ID, N, CSUM, END*/];
}; // RemoteMessage


//==============================================================================
/// \brief Builder for command messages from remote console to robot
/// \ingroup console
template<unsigned int nPayloadBytes>
class RemoteCommandT : public RemoteMessageT<nPayloadBytes>
{
protected:
    RemoteCommandT() : RemoteMessageT<nPayloadBytes>() {}
    virtual ~RemoteCommandT() {}

    /// Create command message given message code and payload
    /// \param id Command id (RemoteMessage::MessageID)
    /// \param len Number of bytes in the above payload buffer(0-255).
    ///             - If the len > size(), only size() elements are copied.
    ///             - If len < size(), [len - size()-1] elements are set to 0.
    /// \param pPayloadData Pointer to buffer containing *payload* data.
    void initialise(RemoteMessage::MessageID id, unsigned int len, unsigned char* pPayloadData);

    /// Derived classes must always call this method whenever it modifies the command
    /// message buffer in any way.
    void setCommandModified() { _bytes[size()-2] = computeChecksum(); }

}; // RemoteCommand

//==============================================================================
/// \brief Builder for response messages from robot to console
/// \ingroup console
/// \note Always check message is valid before using it.
template<unsigned int nPayloadBytes>
class RemoteResponseT : public RemoteMessageT<nPayloadBytes>
{
public:
    RemoteResponseT() : RemoteMessageT<nPayloadBytes>() {}
    virtual ~RemoteResponseT() {}

    /// Verify that the message construction is correct
    /// The method verifies message start, checksum and end bytes but not the payload.
    /// Derived classes can override this method to provide additional checks.
    /// \return true if the message format is correct
    virtual bool validate();

}; // RemoteResponse

//-----------------------------------------------------------------------------
unsigned char RemoteMessage::computeChecksum()
//-----------------------------------------------------------------------------
{
    unsigned int csum = 0;
    unsigned int end = size()-2; // ignore first byte and the last two bytes
    unsigned char* pBytes = bytes();
    for( unsigned int i = 1; i < end; ++i )
    {
        csum += (unsigned int)(pBytes[i]);
    }
    return( (unsigned char)(csum&0xFF));
}

//-----------------------------------------------------------------------------
template<unsigned int np>
void RemoteCommandT<np>::initialise(RemoteMessage::MessageID id, unsigned int len, unsigned char *pPayloadData)
//-----------------------------------------------------------------------------
{
    /// [START_BYTE][ID][N][.... payload ....][CSUM][END_BYTE]

    _bytes[0] = RemoteMessage::START_BYTE;
    _bytes[size()-1] = RemoteMessage::END_BYTE;
    _bytes[ID_INDEX] = id;
    _bytes[PAYLOAD_LENGTH_INDEX] = len;

    unsigned int i = PAYLOAD_LENGTH_INDEX + 1;
    unsigned int j = 0;
    while( i < (size() - 2) )
    {
        if( j < len )
        {
            _bytes[i] = pPayloadData[j];
            ++j;
        }
        else
        {
            _bytes[i] = 0;
        }
        ++i;
    }

    setCommandModified();
}

//-----------------------------------------------------------------------------
template<unsigned int np>
bool RemoteResponseT<np>::validate()
//-----------------------------------------------------------------------------
{
    if( _bytes[0] != RemoteMessage::START_BYTE )
    {
        return false;
    }
    if( csum() != computeChecksum() )
    {
        return false;
    }
    if( _bytes[size()-1] != RemoteMessage::END_BYTE )
    {
        return false;
    }
    return true;
}

#endif // REMOTEMESSAGES_H
