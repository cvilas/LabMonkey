//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : RobotController.h
// Brief    : robot controller class
//==============================================================================

#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

class RobotController
{   
public:
    RobotController();
    ~RobotController();

    bool init();
    void run();

private:
    void processConsoleCommand();
    void processConsoleCommandSetMode();
};

#endif // ROBOT_CONTROLLER_H
