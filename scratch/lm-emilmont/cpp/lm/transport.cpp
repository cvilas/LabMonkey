#include "transport.h"
#include <iostream>
#include "utils/stringfuncs.h"

Transport::Transport(const std::string& port, Grape::SerialPort::BaudRate b, bool verbose, std::ostream& log)
    : _verbose(verbose), _logStream(log)
{
    _port.setPortName(port);
    _port.open();
    _port.setBaudRate(b);
    _port.setDataFormat(Grape::SerialPort::D8N1);
}

Transport::~Transport()
{
    _port.close();
}

void Transport::log(const std::string& msg)
{
    if( _verbose )
    {
        _logStream << msg << std::endl;
    }
}

std::string Transport::command(const std::string& msg)
{
    log(std::string("> ") + msg);

    std::vector<unsigned char> buf(msg.begin(), msg.end());
    buf.push_back('\n');
    if( buf.size() != _port.write(buf) )
    {
        _logStream << "write error" << std::endl;
        return "";
    }

    Grape::IDataPort::Status st = _port.waitForRead(-1);
    if( st == Grape::IDataPort::PORT_ERROR )
    {
        _logStream << "Error waiting for reply" << std::endl;
        return "";
    }

    buf.clear();
    unsigned int nread = _port.readAll(buf);

    std::string reply(buf.begin() , buf.begin()+nread);

    reply = Grape::removeEndWhiteSpace(reply);

    log(std::string(": ") + reply);
    return reply;
}
