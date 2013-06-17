//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : LabMonkey.h
// Brief    : Robot interface
//==============================================================================

#ifndef LABMONKEY_H
#define LABMONKEY_H

//==============================================================================
/// \brief The robot!
class LabMonkey
{
public:
    LabMonkey();
    ~LabMonkey();
    void initialise();
    void setHome();
    void enableMotors();
    void disableMotors();
    void waitPosition();
    void getPosition();

    void learnWaypoint();
    void playWaypoints();
    void clearWaypoints();
};

#endif // LABMONKEY_H
