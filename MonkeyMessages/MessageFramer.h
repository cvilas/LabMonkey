//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : MessageFramer.h
// Brief    : Frames messages for transport
//==============================================================================

#ifndef MESSAGEFRAMER_H
#define MESSAGEFRAMER_H

#include <vector>

/// \brief Processes messages for transport
/// \ingroup console
///
/// The MessageFramer has two functions: frame a message for transport to a remote
/// host, and unframe a packet from remote host.
///
/// Framing: A byte array is appended with a [START] and [END] framing characters
/// on either ends of the array, respectively. If any character [char] in between
/// [START] and [END] characters are the reserved characters, they are replaced
/// with characters [STUFF_BYTE][STUFF_BYTE+char]. The reserved characters are
/// [START][END] and [STUFF_BYTE].
///
/// Unframing: Takes data stream and strips it off the framing characters.
class MessageFramer
{
public:
    static const unsigned int START_BYTE    = 0x02; // ascii STX
    static const unsigned int END_BYTE      = 0x03; // ascii ETX
    static const unsigned int STUFF_BYTE    = 0x10; // ascii DLE
public:
    MessageFramer() {}
    ~MessageFramer() {}

    /// frame. no error checks are performed.
    /// \param in input message
    /// \param out framed message
    void frame(const std::vector<unsigned char> &in, std::vector<unsigned char> &out);
    void frame(const unsigned char* inBuf, unsigned int sz, std::vector<unsigned char> &out);

    /// unframe. No error checks are performed.
    /// \param in input framed message
    /// \param out unframed message
    void unframe(const std::vector<unsigned char> &in, std::vector<unsigned char> &out);
    void unframe(const unsigned char* inBuf, unsigned int sz, std::vector<unsigned char> &out);

    /// Check if the message is framed properly
    /// \return true if framing is good.
    bool isFramed(const std::vector<unsigned char> & message);
    bool isFramed(const unsigned char* inBuf, unsigned int sz);
};

#endif // MESSAGEFRAMER_H
