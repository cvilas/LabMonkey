//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : MessageFramer.cpp
//==============================================================================

#include "MessageFramer.h"

//==============================================================================
void MessageFramer::frame(const std::vector<unsigned char> &in,
                          std::vector<unsigned char> &out)
{
    frame(&in[0], in.size(), out);
}

//==============================================================================
void MessageFramer::frame(const unsigned char* inBuf, unsigned int sz, std::vector<unsigned char> &out)
//==============================================================================
{
    out.clear();
    out.reserve(sz+2); // we will need at least these many

    out.push_back(START_BYTE);
    unsigned int i = 0;
    while( i < sz )
    {
        unsigned char b = inBuf[i];
        if( b == START_BYTE || b == END_BYTE || b == STUFF_BYTE )
        {
            out.push_back(STUFF_BYTE);
            out.push_back( (STUFF_BYTE + b)&0xFF );
        }
        else
        {
            out.push_back(b);
        }
        ++i;
    }
    out.push_back(END_BYTE);
}

//------------------------------------------------------------------------------
void MessageFramer::unframe(const std::vector<unsigned char> &in,
                            std::vector<unsigned char> &out)
//------------------------------------------------------------------------------
{
    unframe(&in[0], in.size(), out);
}

//------------------------------------------------------------------------------
void MessageFramer::unframe(const unsigned char* inBuf, unsigned int sz, std::vector<unsigned char> &out)
//------------------------------------------------------------------------------
{
    unsigned int iLast = sz-1;
    out.clear();
    out.reserve(iLast-1);

    for( unsigned int i = 1; i < iLast; ++i )
    {
        unsigned char b = inBuf[i];
        if( b == STUFF_BYTE )
        {
            ++i;
            b = 0xFF&(inBuf[i] - STUFF_BYTE);
        }
        out.push_back(b);
    }
}

//------------------------------------------------------------------------------
bool MessageFramer::isFramed(const std::vector<unsigned char> & message)
//------------------------------------------------------------------------------
{
    return isFramed(&message[0], message.size());
}

//------------------------------------------------------------------------------
bool MessageFramer::isFramed(const unsigned char* inBuf, unsigned int sz)
//------------------------------------------------------------------------------
{
    unsigned int iLast = sz-1;

    if( (inBuf[0] != START_BYTE) || (inBuf[iLast] != END_BYTE) )
    {
        return false;
    }

    for( unsigned int i = 1; i < iLast; ++i )
    {
        unsigned char b = inBuf[i];
        if( (b == START_BYTE) || (b == END_BYTE) )
        {
            return false;
        }
        else if (b == STUFF_BYTE )
        {
            // expect the next byte to be one of the reserved characters, and not end of stream

            ++i;
            if( i == iLast )
            {
                // second last byte can't be stuff byte
                return false;
            }

            b = 0xFF&(inBuf[i] - STUFF_BYTE);
            if( (b != START_BYTE) && (b != END_BYTE) && (b != STUFF_BYTE) )
            {
                return false;
            }
        }
    }
    return true;
}
