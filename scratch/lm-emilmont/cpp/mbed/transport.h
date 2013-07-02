#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "mbed.h"
#include <string>

class Transport
{
public:
    Transport(int baud=9600, bool verbose=true);
    ~Transport();
    void log(const std::string& msg);
    std::string command(const std::string& msg);
    Serial& getConsole() { return _logStream; }
private:
    Serial _port;
    Serial _logStream;    
    bool _verbose;
};
#endif // TRANSPORT_H
