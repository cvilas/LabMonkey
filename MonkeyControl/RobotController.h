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
#include <list>

/// \brief hmi interface for labmonkey
/// \ingroup robot
class RobotController
{   
public:
    RobotController();
    ~RobotController();

    bool init();
    void run();

    LabMonkey& getMonkey() { return _monkey; }

    // ------------------------ user interface -----------------------------

    // remote console connection state
    bool isConsoleActive() { return _isConsoleActive; }
    void setConsoleActive(bool option);

    void setMode(RemoteMessage::Mode mode);
    RemoteMessage::Mode getMode() { return _mode; }
    void onModeBtn();

    void setSpeed(int speed);
    int getSpeed() { _monkey.getSpeedScale(); }
    void onSpeedDnBtn();
    void onSpeedUpBtn();

    void play(bool option);
    bool isPlaying() { return _playWayPoints; }
    void onPlayBtn();

    void onRecBtn();
    void onClearBtn();

    void setPosition(int p[LabMonkey::NUM_JOINTS]);
    void onHomeBtn();

    /// Add a waypoint at the end of the existing list of waypoints
    void appendWayPoint(const LabMonkey::WayPoint& wp) { _wayPoints.push_back(wp); }

    /// insert a waypoint into the list, before 'index' position
    inline bool insertWayPoint(int index, const LabMonkey::WayPoint& wp);

    /// get pointer to a waypoint in the list. NULL is returned for invalid index
    inline LabMonkey::WayPoint* getWayPoint(int index);

    /// remove waypoint specified by index
    inline bool removeWayPoint(int index);

    /// remove all waypoints
    void clearWayPoints() { _wayPoints.clear(); }

    /// get the number of waypoints
    int getNumWayPoints() const { return _wayPoints.size(); }

private:
    void runPlayMode();
    void runTeachMode();

private:
    bool                _isConsoleActive;
    rtos::Mutex         _monkeyLock;
    LabMonkey           _monkey;
    RemoteMessage::Mode _mode;
    bool                _playWayPoints;
    bool                _isProcessingModes;
    InterruptIn*        _pBtnIntr[4];
    std::list<LabMonkey::WayPoint> _wayPoints;
};

//------------------------------------------------------------------------------
bool RobotController::insertWayPoint(int index, const LabMonkey::WayPoint& wp)
//------------------------------------------------------------------------------
{
    if( index < getNumWayPoints() )
    {
        std::list<LabMonkey::WayPoint>::iterator it = _wayPoints.begin();
        std::advance(it, index);
        _wayPoints.insert(it, wp);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
bool RobotController::removeWayPoint(int index)
//------------------------------------------------------------------------------
{
    if( index < getNumWayPoints() )
    {
        std::list<LabMonkey::WayPoint>::iterator it = _wayPoints.begin();
        std::advance(it, index);
        _wayPoints.erase(it);
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
LabMonkey::WayPoint* RobotController::getWayPoint(int index)
//------------------------------------------------------------------------------
{
    if( index < getNumWayPoints() )
    {
        std::list<LabMonkey::WayPoint>::iterator it = _wayPoints.begin();
        std::advance(it, index);
        return &(*it);
    }
    return NULL;
}
#endif // ROBOT_CONTROLLER_H
