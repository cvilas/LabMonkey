//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : LabMonkey.cpp
//==============================================================================

#include "LabMonkey.h"
#include <iostream>
#include <vector>

//==============================================================================
LabMonkey::LabMonkey()
//==============================================================================
    : _speedScale(10)
{
    // manually configure all motors (id, rpmLimit, acc, cpr)
    _pMotor[0] = new Motor( MotorConfig(1,  3000,   5000,   1) );
    _pMotor[1] = new Motor( MotorConfig(2,  200,    500,    1) );
    _pMotor[2] = new Motor( MotorConfig(3,  300,    500,    1) );
    _pMotor[3] = new Motor( MotorConfig(4,  1500,   5000,   1) );
    _pMotor[4] = new Motor( MotorConfig(5,  2000,   5000,   1) );

    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        _pMotor[i]->disablePower();
    }
}

//------------------------------------------------------------------------------
LabMonkey::~LabMonkey()
//------------------------------------------------------------------------------
{
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        delete _pMotor[i];
    }
}

//------------------------------------------------------------------------------
void LabMonkey::setHome()
//------------------------------------------------------------------------------
{
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        _pMotor[i]->home();
    }
}

//------------------------------------------------------------------------------
void LabMonkey::setPosition(int p[NUM_JOINTS])
//------------------------------------------------------------------------------
{
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        _pMotor[i]->home( p[i] );
    }
}

//------------------------------------------------------------------------------
void LabMonkey::enableMotorPower()
//------------------------------------------------------------------------------
{
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        _pMotor[i]->enablePower();
    }
}

//------------------------------------------------------------------------------
void LabMonkey::disableMotorPower()
//------------------------------------------------------------------------------
{
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        _pMotor[i]->disablePower();
    }
}

//------------------------------------------------------------------------------
void LabMonkey::waitMoveCompleted()
//------------------------------------------------------------------------------
{
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        _pMotor[i]->waitMoveCompleted();
    }
}

//------------------------------------------------------------------------------
bool LabMonkey::isMoveCompleted()
//------------------------------------------------------------------------------
{
    for( int i = 0; i < NUM_JOINTS; ++i )
    {
        if( !_pMotor[i]->isMoveCompleted() )
        {
            return false;
        }
    }
    return true;
}

//------------------------------------------------------------------------------
void LabMonkey::getPosition(int p[5])
//------------------------------------------------------------------------------
{
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        p[i] = _pMotor[i]->getPosition();
    }
}

//------------------------------------------------------------------------------
void LabMonkey::stop()
//------------------------------------------------------------------------------
{
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        _pMotor[i]->stop();
    }
}

//------------------------------------------------------------------------------
void LabMonkey::moveToWaypoint(const WayPoint& waypoint)
//------------------------------------------------------------------------------
{
    int position[NUM_JOINTS];
    getPosition(position);

    // compute time allowed for move
    int movementTimeMs = (waypoint.periodMs * 10)/getSpeedScale();
    long int scaledRevsToTarget[NUM_JOINTS]; // scaled so that dividing by rpm gives time in ms
    for( int i = 0; i < NUM_JOINTS; ++i )
    {
        scaledRevsToTarget[i] = (60000 * abs(position[i] - waypoint.pos[i]))/_pMotor[i]->getCountsPerRev();

        int minMs = scaledRevsToTarget[i]/_pMotor[i]->getRpmLimit();
        movementTimeMs = std::max( movementTimeMs, minMs);
    }

    // set move speed
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        int rpm = scaledRevsToTarget[i]/movementTimeMs;
        _pMotor[i]->setTargetRpm(rpm);
    }

    // move
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        _pMotor[i]->moveAbsolute( waypoint.pos[i] );
    }
}

