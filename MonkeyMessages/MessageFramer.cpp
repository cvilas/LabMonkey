//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : MessageFramer.cpp
//==============================================================================

#include "MessageFramer.h"

//==============================================================================
void MessageFramer::frame(const std::vector<unsigned char> &in,
                          std::vector<unsigned char> &out)
//==============================================================================
{
    out.clear();
    out.reserve(in.size()+2); // we will need at least these many

    out.push_back(START_BYTE);
    std::vector<unsigned char>::const_iterator it = in.begin();
    while( it != in.end() )
    {
        unsigned char b = *it;
        if( b == START_BYTE || b == END_BYTE || b == STUFF_BYTE )
        {
            out.push_back(STUFF_BYTE);
            out.push_back( (STUFF_BYTE + b)&0xFF );
        }
        else
        {
            out.push_back(b);
        }
        ++it;
    }
    out.push_back(END_BYTE);
}

//------------------------------------------------------------------------------
void MessageFramer::unframe(const std::vector<unsigned char> &in,
                            std::vector<unsigned char> &out)
//------------------------------------------------------------------------------
{
    unsigned int iLast = in.size()-1;
    out.clear();
    out.reserve(iLast-1);

    for( unsigned int i = 1; i < iLast; ++i )
    {
        unsigned char b = in[i];
        if( b == STUFF_BYTE )
        {
            ++i;
            b = 0xFF&(in[i] - STUFF_BYTE);
        }
        out.push_back(b);
    }
}

//------------------------------------------------------------------------------
bool MessageFramer::isFramed(const std::vector<unsigned char> & message)
//------------------------------------------------------------------------------
{
    unsigned int iLast = message.size()-1;

    if( (message[0] != START_BYTE) || (message[iLast] != END_BYTE) )
    {
        return false;
    }

    for( unsigned int i = 1; i < iLast; ++i )
    {
        unsigned char b = message[i];
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

            b = 0xFF&(message[i] - STUFF_BYTE);
            if( (b != START_BYTE) && (b != END_BYTE) && (b != STUFF_BYTE) )
            {
                return false;
            }
        }
    }
    return true;
}
