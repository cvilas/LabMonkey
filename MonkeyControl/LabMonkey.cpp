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
    _pMotor[0] = new Motor( MotorConfig(1,  3000,   5000,   3000) );
    _pMotor[1] = new Motor( MotorConfig(2,  200,    500,    3000) );
    _pMotor[2] = new Motor( MotorConfig(3,  300,    500,    3000) );
    _pMotor[3] = new Motor( MotorConfig(4,  1500,   5000,   4096) );
    _pMotor[4] = new Motor( MotorConfig(5,  2000,   5000,   4096) );

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
void LabMonkey::getPosition(int p[NUM_JOINTS])
//------------------------------------------------------------------------------
{
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        p[i] = _pMotor[i]->getPosition();
    }
}

//------------------------------------------------------------------------------
void LabMonkey::setSpeedScale(int s)
//------------------------------------------------------------------------------
{
    if( s < 1 ) s = 1;
    if( s > 100 ) s = 100;
    _speedScale = s;
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

    // compute fastest time we can do
    double movementTimeMins = 0;
    long countsToTarget[NUM_JOINTS];
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        float maxCountsPerMin = _pMotor[i]->getRpmLimit() * _pMotor[i]->getCountsPerRev();
        countsToTarget[i] = labs( position[i] - waypoint.pos[i] );
        movementTimeMins = std::max<double>( movementTimeMins, ((double)countsToTarget[i]/maxCountsPerMin) );
    }

    movementTimeMins = movementTimeMins * (11 - getSpeedScale()/10);
    AppBoard::logStream().printf(" [T: %f]", movementTimeMins);

    // set rpm
    AppBoard::logStream().printf(" [V ");
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        int rpm = (int)(countsToTarget[i]/(_pMotor[i]->getCountsPerRev() * movementTimeMins));
        _pMotor[i]->setTargetRpm(rpm);
        AppBoard::logStream().printf("%d ", rpm);
    }
    AppBoard::logStream().printf("]");

    // move
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        _pMotor[i]->moveAbsolute( waypoint.pos[i] );
    }

    /*
    // compute time allowed for move
    int movementTimeMs = (waypoint.periodMs * 10)/getSpeedScale();
    long scaledRevsToTarget[NUM_JOINTS]; // scaled so that dividing by rpm gives time in ms
    for( int i = 0; i < NUM_JOINTS; ++i )
    {
        scaledRevsToTarget[i] = labs(position[i] - waypoint.pos[i]) * (60000L/(long)_pMotor[i]->getCountsPerRev());
        int minMs = scaledRevsToTarget[i]/_pMotor[i]->getRpmLimit();
        movementTimeMs = std::max( movementTimeMs, minMs);
    }

    AppBoard::logStream().printf(" [T: %d]", movementTimeMs);

    AppBoard::logStream().printf(" [SD %d %d %d %d %d]",
                                 scaledRevsToTarget[0],scaledRevsToTarget[1],
            scaledRevsToTarget[2],scaledRevsToTarget[3],scaledRevsToTarget[4]);

    // set move speed
    AppBoard::logStream().printf(" [V ");
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        int rpm = std::max<int>(10,scaledRevsToTarget[i]/movementTimeMs);
        _pMotor[i]->setTargetRpm(rpm);
        AppBoard::logStream().printf("%d ", rpm);
    }
    AppBoard::logStream().printf("]");

    // move
    for(int i = 0; i < NUM_JOINTS; ++i)
    {
        _pMotor[i]->moveAbsolute( waypoint.pos[i] );
    }
    */

}

