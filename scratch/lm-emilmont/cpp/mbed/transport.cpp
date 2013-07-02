#include "transport.h"
#include <iostream>
#include <vector>

Transport::Transport(int baud, bool verbose)
    : _port(p9,p10), 
    _logStream(USBTX,USBRX),
    _verbose(verbose)
{
    _port.baud(baud);
    _port.format(8, Serial::None, 1);
}

Transport::~Transport()
{
}

void Transport::log(const std::string& msg)
{
    if( _verbose )
    {
        _logStream.puts(msg.c_str());
    }
}

std::string Transport::command(const std::string& msg)
{
    std::string str = msg + std::string("\n");
    
    while( _port.writeable() == 0 ) {}
        
    log(std::string("> ") + str);

    if( _port.puts(str.c_str()) < 0 )
    {
        _logStream.puts("write error");
        return "";
    }

    while( _port.readable() == 0 ) {}
    
    char replyBuf[200];
    std::string reply( _port.gets(replyBuf,200) );

    log(std::string(": ") + reply);
    return reply;
}
