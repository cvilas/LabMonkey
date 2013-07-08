//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : LabMonkey.cpp
//==============================================================================

#include "LabMonkey.h"

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
    if( s > 10 ) s = 10;
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
    /*
     * Calculations:
     *  For motors m
     *  (1) counts to next target for a motor D[m] = position[m] - waypoint[m];
     *  (2) fastest possible motor move time in ms, t[m] = {(60000 * D[m])/cpr[m]} * (1/rpm_limit[m]);
     *  (3) fastest possible waypoint move time t_fast = max(t[m])
     *  (4) move time after appling global speed scale (1-10), t_move = t_fast * (11 - scale);
     *  (5) motor rpm, rpm[m] = {(60000 * D[m])/cpr[m]} * (1/t_move);
     */

    // where ae we now
    int position[NUM_JOINTS];
    getPosition(position);

    // where do we want to go and how fast can we get there
    double movementTime = 0; // units: (1/10X) ms chunks for timing scale of (10X) * 60,000
    double scaledRevsToTarget[NUM_JOINTS];
    for( int i = 0; i < NUM_JOINTS; ++i )
    {
        scaledRevsToTarget[i] = (llabs(1 + position[i] - waypoint.pos[i]) * 60000.0)/_pMotor[i]->getCountsPerRev();
        movementTime = std::max<double>(movementTime, scaledRevsToTarget[i]/_pMotor[i]->getRpmLimit());
    }

    // apply speed scale factor (scale down from fastest possible waypoint move)
    int sp = getSpeedScale();
    if( sp > 10 ) sp = 10;
    if( sp < 1 ) sp = 1;
    movementTime = movementTime * (11 - sp);
    AppBoard::logStream().printf(" [T: %lf]", movementTime);

    // set rpm
    AppBoard::logStream().printf(" [V:");
    for( int i = 0; i < NUM_JOINTS; ++i )
    {
        int rpm = 1 + scaledRevsToTarget[i]/movementTime;
        _pMotor[i]->setTargetRpm(rpm);
        AppBoard::logStream().printf(" %d", rpm);
    }
    AppBoard::logStream().printf("]");

    // move
    for( int i = 0; i < NUM_JOINTS; ++i )
    {
        _pMotor[i]->moveAbsolute( waypoint.pos[i] );
    }
}

