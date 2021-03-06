//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : LabMonkey.h
// Brief    : Robot interface
//==============================================================================

#ifndef LABMONKEY_H
#define LABMONKEY_H

#include "Motor.h"

/// \brief the robot
/// \ingroup robot
class LabMonkey
{
public:
    static const int NUM_JOINTS = 5;

public:
    /// \brief A single waypoint
    /// \ingroup robot
    /// defines a position for all joints and a speed at which to reach that position
    class WayPoint
    {
    public:
        WayPoint() : periodMs(0) {}
        int periodMs;           //!< time period allocated to reach position (motor acceleration and velocity settings permitting)
        int pos[NUM_JOINTS];    //!< waypoint position defined in encoder counts
    };

public:
    LabMonkey();
    ~LabMonkey();

    /// enable motor power
    void enableMotorPower();

    /// disable motor power
    void disableMotorPower();

    /// set current position as home (encoder zero)
    void setHome();

    /// set encoders for current position
    void setPosition(int p[NUM_JOINTS]);

    /// read current position
    void getPosition(int p[NUM_JOINTS]);

    /// set global speed scale factor
    /// \param s scale factor in range [1 - 10]
    void setSpeedScale(int s);

    /// returns global speed scale factor x 10.
    int getSpeedScale() { return _speedScale; }

    /// initiate movement to specified waypoint. use isMoveCompleted() to check if
    /// movement was completed, or waitMoveCompleted() to wait until movement is complete
    void moveToWaypoint(const WayPoint& point);

    /// Stop movement
    void stop();

    /// wait until last move command is completed
    void waitMoveCompleted();

    /// check completion status of last move command
    bool isMoveCompleted();

private:
    int _speedScale;
    Motor* _pMotor[NUM_JOINTS];

};

#endif // LABMONKEY_H
