//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : Motor.h
// Brief    : motor interface
//==============================================================================

#ifndef MOTOR_H
#define MOTOR_H

//==============================================================================
/// \brief motor interface
class Motor
{
public:
    Motor(int node);


    void setMaxVelocity(int v);
    bool enable();

private:
    int _node;
    int _maxVel;
};

#endif // MOTOR_H
