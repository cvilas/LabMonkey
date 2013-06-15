//==============================================================================
// Project  : Lab Monkey
// Module   : console
// File     : MessageFramer.cpp
//==============================================================================

#include "MessageFramer.h"
#include <assert.h>

//==============================================================================
void MessageFramer::frame(const unsigned char* inBuf, unsigned int inSize,
                          std::vector<unsigned char> &outBuf)
//==============================================================================
{
    unsigned int outSize = computeFramedSize(inBuf, inSize);
    outBuf.clear();
    outBuf.resize( outSize );
    frame(inBuf, inSize, &outBuf[0], outSize);
}

//------------------------------------------------------------------------------
void MessageFramer::frame(const unsigned char* inBuf, unsigned int inSize,
           unsigned char* outBuf, unsigned int outSize)
//------------------------------------------------------------------------------
{
    outBuf[0] = START_BYTE;

    unsigned int iIndex = 0;
    unsigned int oIndex = 1;

    while( iIndex < inSize )
    {
        assert(oIndex < outSize);

        unsigned char b = inBuf[iIndex];
        if( b == START_BYTE || b == END_BYTE || b == STUFF_BYTE )
        {
            outBuf[oIndex] = STUFF_BYTE;
            ++oIndex;
            assert(oIndex < outSize);
            outBuf[oIndex] = (STUFF_BYTE + b)&0xFF;
        }
        else
        {
            outBuf[oIndex] = b;
        }
        ++oIndex;
        ++iIndex;
    }
    outBuf[oIndex] = END_BYTE;
}

//------------------------------------------------------------------------------
void MessageFramer::unframe(const std::vector<unsigned char> &inBuf,
                            unsigned char* outBuf, unsigned int outSize)
//------------------------------------------------------------------------------
{
    unframe(&inBuf[0], inBuf.size(), outBuf, outSize);
}


//------------------------------------------------------------------------------
void MessageFramer::unframe(const unsigned char* inBuf, unsigned int inSize,
                            unsigned char* outBuf, unsigned int outSize)
//------------------------------------------------------------------------------
{
    unsigned int iLast = inSize-1;
    unsigned int oIndex = 0;

    for( unsigned int i = 1; i < iLast; ++i )
    {
        assert(oIndex < outSize);
        unsigned char b = inBuf[i];
        if( b == STUFF_BYTE )
        {
            ++i;
            b = 0xFF&(inBuf[i] - STUFF_BYTE);
        }
        outBuf[oIndex] = b;
        ++oIndex;
    }
}

//------------------------------------------------------------------------------
unsigned int MessageFramer::computeUnFramedSize(const unsigned char* inBuf, unsigned int sz)
//------------------------------------------------------------------------------
{
    unsigned int szUnframed = sz-2;
    for(int i = 1; i < sz-1; ++i)
    {
        unsigned char b = inBuf[i];
        if( b == START_BYTE || b == END_BYTE || b == STUFF_BYTE )
        {
            --szUnframed;
        }
    }
    return szUnframed;
}

//------------------------------------------------------------------------------
unsigned int MessageFramer::computeFramedSize(const unsigned char* inBuf, unsigned int sz)
//------------------------------------------------------------------------------
{
    unsigned int szFramed = 2;
    for(int i = 0; i < sz; ++i)
    {
        ++szFramed;
        unsigned char b = inBuf[i];
        if( b == START_BYTE || b == END_BYTE || b == STUFF_BYTE )
        {
            ++szFramed;
        }
    }
    return szFramed;
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
