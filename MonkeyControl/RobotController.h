//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : RobotController.h
// Brief    : robot controller class
//==============================================================================

#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include "LabMonkey.h"
#include "../MonkeyMessages/RemoteMessages.h"

/// \brief hmi interface for labmonkey
/// \ingroup robot
class RobotController
{
public:
    static const int MAX_WAYPOINTS = 20;

    RobotController();
    ~RobotController();

    bool init();
    void run();

    inline void lockRobot(bool opt);
    LabMonkey& getMonkey() { return _monkey; }

    // ------------------------ user interface -----------------------------

    // remote console connection state
    bool isConsoleActive() { return _isConsoleActive; }
    void setConsoleActive(bool option);

    void setMode(RemoteMessage::Mode mode);
    RemoteMessage::Mode getMode() { return _mode; }

    void setSpeed(int speed);
    int getSpeed() { return _monkey.getSpeedScale(); }

    void play(bool option);
    void recordPosition();
    bool isPlaying() { return _playWayPoints; }

    void setHome();
    void setPosition(int p[LabMonkey::NUM_JOINTS]);
    void getPosition(LabMonkey::WayPoint& wp);

    /// Add a waypoint at the end of the existing list of waypoints
    inline void appendWayPoint(const LabMonkey::WayPoint& wp);

    /// get pointer to a waypoint in the list. NULL is returned for invalid index
    inline LabMonkey::WayPoint* getWayPoint(int index);

    /// remove waypoint specified by index
    inline bool removeWayPoint(int index);

    /// remove all waypoints
    inline void clearWayPoints();

    /// get the number of waypoints
    int getNumWayPoints() const { return _numWayPoints; }

private:
    void runPlayMode();
    void runTeachMode();

    void onModeBtn();
    void onFunctionBtn();
    void onClearBtn();
    void onHomeBtn();

    void doFunctionButtons();
    void changeMode();

private:
    int                 _numWayPoints;
    bool                _isConsoleActive;
    rtos::Mutex         _monkeyLock;
    LabMonkey           _monkey;
    RemoteMessage::Mode _mode;
    bool                _playWayPoints;
    bool                _isProcessingModes;
    LabMonkey::WayPoint _wayPoints[MAX_WAYPOINTS];

    InterruptIn _modeBtn;
    InterruptIn _functionBtn;
    InterruptIn _clearBtn;
    InterruptIn _homeBtn;

    bool _modeButtonPressed;
    bool _functionBtnPressed;
    bool _clearBtnPressed;
    bool _homeBtnPressed;
};

//------------------------------------------------------------------------------
void RobotController::lockRobot(bool opt)
//------------------------------------------------------------------------------
{
    opt ? _monkeyLock.lock() : _monkeyLock.unlock();
}

//------------------------------------------------------------------------------
void RobotController::appendWayPoint(const LabMonkey::WayPoint& wp)
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::appendWayPoint]\n");

    if( (_mode == RemoteMessage::MODE_TEACH) && (_numWayPoints < MAX_WAYPOINTS) )
    {
        lockRobot(true);
        _wayPoints[_numWayPoints] = wp;
        _numWayPoints++;
        AppBoard::lcd().updateModeInfo(_mode, getNumWayPoints());
        lockRobot(false);

        AppBoard::logStream().printf("[RobotController::appendWayPoint] %d %d %d %d %d [%d]\n",
                                     wp.pos[0], wp.pos[1], wp.pos[2], wp.pos[3],wp.pos[4], wp.periodMs);
    }
}

//------------------------------------------------------------------------------
bool RobotController::removeWayPoint(int index)
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::removeWayPoint] at %d\n", index);

    if( (index < _numWayPoints) && (_mode == RemoteMessage::MODE_TEACH) )
    {
        lockRobot(true);
        for(int i = index; i < (_numWayPoints-1); ++i)
        {
            _wayPoints[i] = _wayPoints[i+1];
        }
        --_numWayPoints;
        lockRobot(false);

        return true;
    }

    return false;
}

//------------------------------------------------------------------------------
LabMonkey::WayPoint* RobotController::getWayPoint(int index)
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::getWayPoint] at %d\n", index);

    if( index < _numWayPoints )
    {
        return &_wayPoints[index];
    }

    return NULL;
}

//------------------------------------------------------------------------------
void RobotController::clearWayPoints()
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::clearWayPoints]\n");
    _numWayPoints = 0;
}

#endif // ROBOT_CONTROLLER_H
