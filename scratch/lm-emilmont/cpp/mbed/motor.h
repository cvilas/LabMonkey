#ifndef MOTOR_H
#define MOTOR_H

#include "transport.h"
#include "rtos.h"

class Motor
{
    static const int POSITION_ATTAINED = 0x10000;

public:
    Motor(Transport& t, int node = 0);

    ~Motor();

    std::string command(const std::string& msg);

    void setVelocityLimit(int v) { _velocityLimit = v; }
    int getVelocityLimit() { return _velocityLimit; }

    bool enable();

    bool disable();

    int operationStatus();

    bool velocity(int rpm);

    bool stop();

    bool setMaxSpeed(int rpm);

    bool setMaxAcceleration(int acc);

    bool setMaxDeceleration(int dec);

    bool setAcceleration(int acc);

    bool move();

    bool loadRelative(int steps);

    bool moveSteps(int steps);

    bool home(int pos=0);

    bool loadAbsolute(int pos);

    bool moveToLocation(int pos);

    int getPosition();

    void waitPosition();

    bool startProg();

    bool endProg();

    bool delay(int seconds);

    bool runProg();

private:
    Transport& _transport;
    int _node;
    std::string _okResp;
    int _velocityLimit;

public:
    int lastPosition;
};

#endif // MOTOR_H
