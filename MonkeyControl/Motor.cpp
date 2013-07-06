//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : Motor.cpp
//==============================================================================

#include "Motor.h"
#include <sstream>

const std::string Motor::OK_RESPONSE("OK\r\n");

//==============================================================================
Motor::Motor(const MotorConfig& cfg)
//==============================================================================
    : _config(cfg),
      _lastKnownPosition(0)
{
    enablePower();
    setAcceleration(cfg.acc_cpss);
}

//------------------------------------------------------------------------------
Motor::~Motor()
//------------------------------------------------------------------------------
{
    disablePower();
}

//------------------------------------------------------------------------------
std::string Motor::command(const std::string& msg)
//------------------------------------------------------------------------------
{
    Serial& robot = AppBoard::robotPort();
    Serial& log = AppBoard::logStream();

    std::ostringstream motorCmd;

    // construct command
    ( _config.id != 0 ) ? (motorCmd << _config.id << msg) : (motorCmd << msg);
    motorCmd << std::endl;

    // wait for port to be ready
    while( robot.writeable() == 0 ) {}

    const char* str = motorCmd.str().c_str();
    if( AppBoard::VERBOSITY > 0 )
    {
        log.printf("[Motor::command] -> %s", str);
    }

    // send it
    if( robot.puts(str) < 0 )
    {
        log.puts("write error");
        return "";
    }

    // wait for reply
    while( robot.readable() == 0 ) {}

    // read reply
    char replyBuf[200];
    replyBuf[0] = '\0';
    std::string reply( robot.gets(replyBuf,200) );

    if( AppBoard::VERBOSITY > 0 )
    {
        log.printf("[Motor::command] <- %s", replyBuf);
    }

    return reply;
}

//------------------------------------------------------------------------------
bool Motor::enablePower()
//------------------------------------------------------------------------------
{
    return OK_RESPONSE == command("EN");
}

//------------------------------------------------------------------------------
bool Motor::disablePower()
//------------------------------------------------------------------------------
{
    return OK_RESPONSE == command("DI");
}

//------------------------------------------------------------------------------
int Motor::operationStatus()
//------------------------------------------------------------------------------
{
    int t;
    std::istringstream stream(command("OST"));
    stream >> t;
    return t;
}

//------------------------------------------------------------------------------
bool Motor::moveV(int rpm)
//------------------------------------------------------------------------------
{
    std::ostringstream str;
    str << "V" << rpm;
    return OK_RESPONSE == command(str.str());
}

//------------------------------------------------------------------------------
bool Motor::stop()
//------------------------------------------------------------------------------
{
    return moveV(0);
}

//------------------------------------------------------------------------------
bool Motor::setTargetRpm(int rpm)
//------------------------------------------------------------------------------
{
    std::ostringstream str;
    str << "SP" << rpm;
    return OK_RESPONSE == command(str.str());
}

//------------------------------------------------------------------------------
bool Motor::setMaxAcceleration(int acc)
//------------------------------------------------------------------------------
{
    std::ostringstream str;
    str << "AC" << acc;
    return OK_RESPONSE == command(str.str());
}

//------------------------------------------------------------------------------
bool Motor::setMaxDeceleration(int dec)
//------------------------------------------------------------------------------
{
    std::ostringstream str;
    str << "DEC" << dec;
    return OK_RESPONSE == command(str.str());
}

//------------------------------------------------------------------------------
bool Motor::setAcceleration(int acc)
//------------------------------------------------------------------------------
{
    if( !setMaxAcceleration(acc) )
        return false;
    return setMaxDeceleration(acc);
}

//------------------------------------------------------------------------------
bool Motor::moveP()
//------------------------------------------------------------------------------
{
    return OK_RESPONSE == command("M");
}

//------------------------------------------------------------------------------
bool Motor::loadRelative(int counts)
//------------------------------------------------------------------------------
{
    std::ostringstream str;
    str << "LR" << counts;
    return OK_RESPONSE == command(str.str());
}

//------------------------------------------------------------------------------
bool Motor::moveRelative(int counts)
//------------------------------------------------------------------------------
{
    if( !loadRelative(counts) )
        return false;
    _lastKnownPosition += counts;
    return moveP();
}

//------------------------------------------------------------------------------
bool Motor::home(int counts)
//------------------------------------------------------------------------------
{
    std::ostringstream str;
    str << "HO";
    if( counts != 0 )
    {
        str << counts;
    }
    _lastKnownPosition = counts; /*todo: check this is correct*/
    return OK_RESPONSE == command(str.str());
}

//------------------------------------------------------------------------------
bool Motor::loadAbsolute(int counts)
//------------------------------------------------------------------------------
{
    std::ostringstream str;
    str << "LA" << counts;
    return OK_RESPONSE == command(str.str());
}

//------------------------------------------------------------------------------
bool Motor::moveAbsolute(int counts)
//------------------------------------------------------------------------------
{
    if( !loadAbsolute(counts) )
        return false;
    _lastKnownPosition = counts;
    return moveP();
}

//------------------------------------------------------------------------------
int Motor::getPosition()
//------------------------------------------------------------------------------
{
    int t;
    std::istringstream stream(command("POS"));
    stream >> t;
    _lastKnownPosition = t;
    return t;
}

//------------------------------------------------------------------------------
void Motor::waitMoveCompleted()
//------------------------------------------------------------------------------
{
    while( !isMoveCompleted() )
    {
        Thread::wait(10);
    }
}

//------------------------------------------------------------------------------
bool Motor::isMoveCompleted()
//------------------------------------------------------------------------------
{
    return ( (operationStatus() & POSITION_ATTAINED) != 0 );
}

//------------------------------------------------------------------------------
bool Motor::startProg()
//------------------------------------------------------------------------------
{
    return OK_RESPONSE == command("PROGSEQ");
}

//------------------------------------------------------------------------------
bool Motor::endProg()
//------------------------------------------------------------------------------
{
    return OK_RESPONSE == command("END");
}

//------------------------------------------------------------------------------
bool Motor::delay(int seconds)
//------------------------------------------------------------------------------
{
    std::ostringstream str;
    str << "DELAY" << seconds*1000;
    return OK_RESPONSE == command(str.str());
}

//------------------------------------------------------------------------------
bool Motor::runProg()
//------------------------------------------------------------------------------
{
    return OK_RESPONSE == command("ENPROG ");
}
