#include "motor.h"

Motor::Motor(Transport& t, int node)
    : _transport(t),
      _node(node),
      _okResp("OK\r\n"),
      lastPosition(0)
{
    enable();
}

Motor::~Motor()
{
    disable();
}

std::string Motor::command(const std::string& msg)
{
    if( _node == 0)
    {
        return _transport.command(msg);
    }
    else
    {
        std::ostringstream str;
        str << _node << msg;
        return _transport.command(str.str());
    }
}

bool Motor::enable()
{
    return _okResp == command("EN");
}

bool Motor::disable()
{
    return _okResp == command("DI");
}

int Motor::operationStatus()
{
    return Grape::stringAsT<int>( command("OST") );
}

bool Motor::velocity(int rpm)
{
    std::ostringstream str;
    str << "V" << rpm;
    return _okResp == command(str.str());
}

bool Motor::stop()
{
    return velocity(0);
}

bool Motor::setMaxSpeed(int rpm)
{
    std::ostringstream str;
    str << "SP" << rpm;
    return _okResp == command(str.str());
}

bool Motor::setMaxAcceleration(int acc)
{
    std::ostringstream str;
    str << "AC" << acc;
    return _okResp == command(str.str());
}

bool Motor::setMaxDeceleration(int dec)
{
    std::ostringstream str;
    str << "DEC" << dec;
    return _okResp == command(str.str());
}

bool Motor::setAcceleration(int acc)
{
    if( !setMaxAcceleration(acc) )
        return false;

    return setMaxDeceleration(acc);
}

bool Motor::move()
{
    return _okResp == command("M");
}

bool Motor::loadRelative(int steps)
{
    std::ostringstream str;
    str << "LR" << steps;
    return _okResp == command(str.str());
}

bool Motor::moveSteps(int steps)
{
    if( !loadRelative(steps) )
        return false;
    return move();
}

bool Motor::home(int pos)
{
    std::ostringstream str;
    str << "HO";
    if( pos != 0 )
    {
        str << pos;
    }
    return _okResp == command(str.str());
}

bool Motor::loadAbsolute(int pos)
{
    lastPosition = pos;
    std::ostringstream str;
    str << "LA" << pos;
    return _okResp == command(str.str());
}

bool Motor::moveToLocation(int pos)
{
    if( !loadAbsolute(pos) )
        return false;
    return move();
}

int Motor::getPosition()
{
    lastPosition = Grape::stringAsT<int>( command("POS") );
    return lastPosition;
}

void Motor::waitPosition()
{
    while( (operationStatus() & POSITION_ATTAINED) == 0 )
    {
        Grape::StopWatch::nanoSleep(100 * 1000000);
    }
}

bool Motor::startProg()
{
    return _okResp == command("PROGSEQ");
}

bool Motor::endProg()
{
    return _okResp == command("END");
}

bool Motor::delay(int seconds)
{
    std::ostringstream str;
    str << "DELAY" << seconds*1000;
    return _okResp == command(str.str());
}

bool Motor::runProg()
{
    return _okResp == command("ENPROG ");
}
