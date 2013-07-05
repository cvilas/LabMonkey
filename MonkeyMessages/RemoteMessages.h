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
/// [ID][N][.... payload ....][CSUM]
/// \endcode
/// where
/// - ID (1 byte): One of the codes from RemoteMessage::MessageID
/// - N (1 byte): number of bytes in the payload
/// - payload (N bytes): The payload bytes
/// - CSUM (1 byte): Checksum computed over the entire message
///
/// Before transmitting data to remote endpoint, the message is framed by passing it through
/// the MessageFramer. Similarly, data received from the remote end is first unframed using
/// the MessageFramer before processing the message.
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

        SET_POSITION_HOME,  //!< Set current position as zero
        SET_POSITION,       //!< Set encoder values at current position
        GET_POSITION,       //!< Get current position specified by list of joint angles

        PLAY_WP,            //!< Play/Stop waypoints playback
        RECORD_WP,          //!< record a waypoint

        FREEZE_WP_LIST,     //!< Freeze waypoints list (don't allow modifications)
        CLEAR_WP,           //!< Clear waypoints list
        GET_NUM_WP,         //!< Get the number of waypoints
        SET_WP,             //!< Set a way point (list of joint angle lists)
        GET_WP,             //!< Get a way point

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
    static const unsigned int ID_INDEX              = 0;
    static const unsigned int PAYLOAD_LENGTH_INDEX  = 1;

public:
    virtual unsigned char* bytes() = 0;
    virtual unsigned int size() const = 0;
    static inline unsigned char computeChecksum(const unsigned char* pBuf, unsigned int len);

protected:
    RemoteMessage() {}
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
    virtual unsigned int size() const { return nPayloadBytes + 3; }
    virtual unsigned char* bytes() { return _bytes; }
    unsigned int payloadLength() const { return nPayloadBytes; }

    MessageID id() const { return MessageID(_bytes[RemoteMessage::ID_INDEX]&0xFF); }
    unsigned char csum() const { return this->_bytes[this->size()-1]&0xFF; }

protected:
    RemoteMessageT() : RemoteMessage() {}
    virtual ~RemoteMessageT() {}

    /// Create message given message code and payload
    /// \param id Command id (RemoteMessage::MessageID)
    /// \param pPayloadData Pointer to buffer containing *payload* data.
    /// \param len Number of bytes in the above payload buffer(0-255).
    ///             - If the len > payloadLength(), only payloadLength() elements are copied.
    ///             - If len < payloadLength(), [len - payloadLength()-1] elements are set to 0.
    void initialise(RemoteMessage::MessageID id, unsigned char* pPayloadData, unsigned int len);

    /// Derived classes must always call this method whenever it modifies the message buffer in any way.
    void setModified() { this->_bytes[this->size()-1] = this->computeChecksum(); }

protected:
    unsigned char _bytes[nPayloadBytes + 3/*ID, N, CSUM*/];
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

    /// Verify that the message construction is correct. The method verifies
    /// message start, checksum and end bytes but not the payload.
    /// Derived classes can override this method to provide additional checks.
    /// \return true if the message format is correct
    virtual bool validate();

}; // RemoteResponse

//-----------------------------------------------------------------------------
unsigned char RemoteMessage::computeChecksum(const unsigned char* pBuf, unsigned int len)
//-----------------------------------------------------------------------------
{
    unsigned int csum = 0;
    for( unsigned int i = 0; i < len; ++i )
    {
        csum += (unsigned int)(pBuf[i]);
    }
    return( (unsigned char)(csum&0xFF));
}

//-----------------------------------------------------------------------------
unsigned char RemoteMessage::computeChecksum()
//-----------------------------------------------------------------------------
{
    return computeChecksum(bytes(), size()-1/*all except last byte*/);
}

//-----------------------------------------------------------------------------
template<unsigned int np>
void RemoteMessageT<np>::initialise(RemoteMessage::MessageID id,
                                    unsigned char *pPayloadData,
                                    unsigned int len)
//-----------------------------------------------------------------------------
{
    this->_bytes[RemoteMessage::ID_INDEX] = id;
    this->_bytes[RemoteMessage::PAYLOAD_LENGTH_INDEX] = len;

    unsigned int i = RemoteMessage::PAYLOAD_LENGTH_INDEX + 1;
    unsigned int j = 0;
    while( i < (this->size() - 1) )
    {
        if( j < len )
        {
            this->_bytes[i] = pPayloadData[j];
            ++j;
        }
        else
        {
            this->_bytes[i] = 0;
        }
        ++i;
    }

    this->setModified();
}

//-----------------------------------------------------------------------------
template<unsigned int np>
bool RemoteResponseT<np>::validate()
//-----------------------------------------------------------------------------
{
    return ( this->csum() == this->computeChecksum() );
}

#endif // REMOTEMESSAGES_H
