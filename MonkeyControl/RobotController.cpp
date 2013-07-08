//==============================================================================
// Project  : Lab Monkey
// Module   : robot
// File     : RobotController.cpp
//==============================================================================

#include "RobotController.h"
#include "AppBoard.h"

//==============================================================================
RobotController::RobotController()
//==============================================================================
    : _numWayPoints(0),
      _isConsoleActive(false),
      _mode(RemoteMessage::MODE_UNKNOWN),
      _playWayPoints(false),
      _isProcessingModes(false),
      _modeBtn(AppBoard::BTN_MODE),
      _functionBtn(AppBoard::BTN_FUNC),
      _clearBtn(AppBoard::BTN_CLEAR),
      _homeBtn(AppBoard::BTN_HOME),
      _modeButtonPressed(false),
      _functionBtnPressed(false),
      _clearBtnPressed(false),
      _homeBtnPressed(false)
{
    _modeBtn.rise(this, &RobotController::onModeBtn);
    _functionBtn.rise(this, &RobotController::onFunctionBtn);
    _clearBtn.rise(this, &RobotController::onClearBtn);
    _homeBtn.rise(this, &RobotController::onHomeBtn);
}

//------------------------------------------------------------------------------
RobotController::~RobotController()
//------------------------------------------------------------------------------
{
}

//------------------------------------------------------------------------------
bool RobotController::init()
//------------------------------------------------------------------------------
{
    setMode(RemoteMessage::MODE_TEACH);
    return true;
}

//------------------------------------------------------------------------------
void RobotController::RobotController::onModeBtn()
//------------------------------------------------------------------------------
{
    _modeButtonPressed = !_isConsoleActive;
}

//------------------------------------------------------------------------------
void RobotController::RobotController::onFunctionBtn()
//------------------------------------------------------------------------------
{
    _functionBtnPressed = !_isConsoleActive;
}

//------------------------------------------------------------------------------
void RobotController::RobotController::onClearBtn()
//------------------------------------------------------------------------------
{
    _clearBtnPressed = !_isConsoleActive;
}

//------------------------------------------------------------------------------
void RobotController::RobotController::onHomeBtn()
//------------------------------------------------------------------------------
{
    _homeBtnPressed = !_isConsoleActive;
}

//------------------------------------------------------------------------------
void RobotController::setConsoleActive(bool option)
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::setConsoleActive] %d\n", option);
    _isConsoleActive = option;
}

//------------------------------------------------------------------------------
void RobotController::setHome()
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::setHome]\n");
    if( _mode == RemoteMessage::MODE_TEACH )
    {
        clearWayPoints();

        lockRobot(true);
        _monkey.setHome();
        lockRobot(false);
    }
}

//------------------------------------------------------------------------------
void RobotController::setPosition(int p[LabMonkey::NUM_JOINTS])
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::setPosition]\n");
    if( _mode == RemoteMessage::MODE_TEACH )
    {
        clearWayPoints();

        lockRobot(true);
        _monkey.setPosition(p);
        lockRobot(false);
    }
}

//------------------------------------------------------------------------------
void RobotController::getPosition(LabMonkey::WayPoint& wp)
//------------------------------------------------------------------------------
{
    AppBoard::logStream().printf("[RobotController::getPosition]\n");
    lockRobot(true);
    _monkey.getPosition(wp.pos);
    lockRobot(false);
}

//------------------------------------------------------------------------------
void RobotController::changeMode()
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::changeMode]\n");
    if( _modeButtonPressed )
    {
        _modeButtonPressed = false;
        switch( _mode )
        {
        case RemoteMessage::MODE_TEACH:
            setMode( RemoteMessage::MODE_REPLAY );
            break;
        case RemoteMessage::MODE_REPLAY:
            setMode( RemoteMessage::MODE_TEACH );
            break;
        default:
            AppBoard::logStream().printf("[RobotController::changeMode] invalid current mode\n");
            break;
        };
    }
}

//------------------------------------------------------------------------------
void RobotController::doFunctionButtons()
//------------------------------------------------------------------------------
{
    //if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::doFunctionButtons]\n");

    // mode function control (record, play, stop)

    if( _functionBtnPressed )
    {
        _functionBtnPressed = false;
        switch (_mode)
        {
        case RemoteMessage::MODE_TEACH:
            recordPosition();
            break;
        case RemoteMessage::MODE_REPLAY:
            play(!_playWayPoints);
            break;
        default:
            break;
        }
    }

    // clear waypoints or speed down

    if( _clearBtnPressed )
    {
        _clearBtnPressed = false;
        switch (_mode)
        {
        case RemoteMessage::MODE_TEACH:
            clearWayPoints();
            break;
        case RemoteMessage::MODE_REPLAY:
            setSpeed( _monkey.getSpeedScale() - 1);
            break;
        default:
            break;
        }
    }

    // set home or speed up

    if( _homeBtnPressed )
    {
        _homeBtnPressed = false;

        switch (_mode)
        {
        case RemoteMessage::MODE_TEACH:
            setHome();
            break;
        case RemoteMessage::MODE_REPLAY:
            setSpeed(_monkey.getSpeedScale() + 1);
            break;
        default:
            break;
        }
    }

}

//------------------------------------------------------------------------------
void RobotController::setMode(RemoteMessage::Mode mode)
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::setMode] %d -> %d\n", _mode, mode);

    // stop mode processing, ignore button presses
    _mode = RemoteMessage::MODE_UNKNOWN;

    //wait for mode processing to exit
    while( _isProcessingModes )
    {
        Thread::wait(100);
    }

    // stop motion
    play(false);

    // start mode processing
    _mode = mode;
}

//------------------------------------------------------------------------------
bool RobotController::play(bool option)
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::play] %d\n", option);

    lockRobot(true);
    _monkey.stop();
    _playWayPoints = option && (_mode == RemoteMessage::MODE_REPLAY);
    lockRobot(false);

    return _playWayPoints;
}

//------------------------------------------------------------------------------
void RobotController::recordPosition()
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::recordPosition]\n");
    if( _mode == RemoteMessage::MODE_TEACH )
    {
        LabMonkey::WayPoint wp;
        getPosition(wp);
        appendWayPoint(wp);
    }
}

//------------------------------------------------------------------------------
void RobotController::setSpeed(int speed)
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::setSpeed] %d\n", speed);
    lockRobot(true);
    _monkey.setSpeedScale(speed);
    AppBoard::lcd().updateSpeed( _monkey.getSpeedScale() );
    lockRobot(false);
}

//------------------------------------------------------------------------------
void RobotController::runPlayMode()
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::runPlayMode] entered\n");
    AppBoard::lcd().updateModeInfo(RemoteMessage::MODE_REPLAY, getNumWayPoints() );

    int iCurrentWp = -1;
    int nWp = getNumWayPoints();

    bool moveComplete = true;
    int speed = 0;

    while(!_modeButtonPressed && (_mode == RemoteMessage::MODE_REPLAY))
    {
        doFunctionButtons();

        lockRobot(true);

        // completed move. line up next wp
        moveComplete = moveComplete || _monkey.isMoveCompleted();
        if( moveComplete && nWp )
        {
            iCurrentWp = (iCurrentWp+1) % nWp;
        }

        // speed changed. We want to recompute move to next waypoint
        int nowSpeed = _monkey.getSpeedScale();
        moveComplete = moveComplete || (nowSpeed != speed);
        speed = nowSpeed;

        // ensures transition from stop -> play works
        moveComplete = moveComplete || (!_playWayPoints);

        // cue next waypoint, if we are playing
        if( _playWayPoints && nWp && moveComplete )
        {
            moveComplete = false;
            AppBoard::lcd().updateModeInfo(RemoteMessage::MODE_REPLAY, iCurrentWp);
            AppBoard::logStream().printf("\n[wp %d]", iCurrentWp);
            _monkey.moveToWaypoint( *getWayPoint(iCurrentWp) );

        } // cue wp

        lockRobot(false);
    }

    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::runPlayMode] exited\n");
}

//------------------------------------------------------------------------------
void RobotController::runTeachMode()
//------------------------------------------------------------------------------
{
    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::runTeachMode] entered\n");

    AppBoard::lcd().updateModeInfo(RemoteMessage::MODE_TEACH, getNumWayPoints());
    setSpeed(0);

    lockRobot(true);
    _monkey.disableMotorPower();
    lockRobot(false);

    while( !_modeButtonPressed && (_mode == RemoteMessage::MODE_TEACH) )
    {
        //AppBoard::logStream().printf("[RobotController::runTeachMode] loop\n");
        doFunctionButtons();

        Thread::wait(100);
    }

    if( AppBoard::VERBOSITY > 1 ) AppBoard::logStream().printf("[RobotController::runTeachMode] exited\n");
}

//------------------------------------------------------------------------------
void RobotController::run()
//------------------------------------------------------------------------------
{
    while(1)
    {
        _isProcessingModes = false;

        changeMode();

        switch( _mode )
        {
        case RemoteMessage::MODE_TEACH:
            _isProcessingModes = true;
            runTeachMode();
            break;
        case RemoteMessage::MODE_REPLAY:
            _isProcessingModes = true;
            runPlayMode();
            break;
        default:
            _isProcessingModes = false;
            AppBoard::lcd().updateModeInfo(_mode);
            Thread::wait(100);
        };
    }

}
