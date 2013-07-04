//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : RobotController.h
// Brief    : robot controller class
//==============================================================================

#ifndef ROBOT_CONTROLLER_H
#define ROBOT_CONTROLLER_H

#include "LabMonkey.h"
#include <list>

/// \brief hmi interface for labmonkey
/// \ingroup robot
/// \todo
/// - set CPR for all motors
/// - get/set mode
/// - record mode
///     - set home
///     - clear waypoints
///     - record waypoint
/// - play mode
///     - speed+
///     - speed-
///     - play/stop
class RobotController
{   
public:
    RobotController();
    ~RobotController();

    bool init();
    void run();

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
    void processConsoleCommand();
    void processConsoleCommandSetMode();

private:
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
