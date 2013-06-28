#ifndef TRANSPORT_H
#define TRANSPORT_H

#include <io/SerialPort.h>

class Transport
{
public:
    Transport(const std::string& port, Grape::SerialPort::BaudRate b, bool verbose, std::ostream& log);

    ~Transport();

    void log(const std::string& msg);

    std::string command(const std::string& msg);

private:
    Grape::SerialPort _port;
    bool _verbose;
    std::ostream& _logStream;
};
#endif // TRANSPORT_H
