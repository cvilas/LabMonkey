//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : Motor.h
// Brief    : Motor controller interface
//==============================================================================

#ifndef MOTOR_H
#define MOTOR_H

#include "AppBoard.h"

/// \brief motor configuration constants
/// \ingroup robot
class MotorConfig
{
public:
    unsigned int id;
    unsigned int rpmLimit; // speed limit in rpm
    unsigned int acc_cpss; // nominal acceleration in counts/sec/sec
    unsigned int cpr; // encoder counts per revolution

    MotorConfig(unsigned int id=0, unsigned int v=0, unsigned int a=0, unsigned int c = 1000) : id(id), rpmLimit(v), acc_cpss(a), cpr(c) {}
};

/// \brief motor control interface
/// \ingroup robot
class Motor
{
    static const int POSITION_ATTAINED = 0x10000;

public:
    Motor(const MotorConfig& cfg);
    ~Motor();

    /// motor control command
    std::string command(const std::string& msg);

    /// position at last encoder update or movement
    int getLastknownPosition() { return _lastKnownPosition; }

    /// power up motor
    bool enablePower();

    /// power down motor
    bool disablePower();

    /// get operating status
    int operationStatus();

    /// stop motion
    bool stop();

    /// encoder counts per revolution
    int getCountsPerRev() { return _config.cpr; }

    /// 'Never Exceed' speed for the motor in rpm
    int getRpmLimit() { return _config.rpmLimit; }

    /// set target speed (RPM) for motion
    bool setTargetRpm(int rpm);

    /// set acceleration in counts/sec/sec
    bool setAcceleration(int acc);

    /// Set encoder position
    bool home(int pos=0);

    /// move relative to current position in encoder counts
    bool moveRelative(int steps);

    /// move to absolute position in encoder counts
    bool moveAbsolute(int pos);

    /// read current absolute position in encoder counts
    int getPosition();

    /// wait for last move command to complete
    void waitMoveCompleted();

    /// check if last move command completed.
    bool isMoveCompleted();

    bool startProg();
    bool endProg();
    bool delay(int seconds);
    bool runProg();

private:
    bool loadRelative(int counts);
    bool loadAbsolute(int counts);
    bool moveP(); // initiate move to last set setpoint
    bool moveV(int rpm); // initiate move at velocity
    bool setMaxAcceleration(int acc);
    bool setMaxDeceleration(int dec);

private:
    MotorConfig _config;
    std::string _okResp;
    int         _lastKnownPosition;
};

#endif // MOTOR_H
