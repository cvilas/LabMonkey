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
    AppBoard::logStream().printf("[RobotController::setConsoleActive] %d\n", option);
    _isConsoleActive = option;
}

//------------------------------------------------------------------------------
void RobotController::setHome()
//------------------------------------------------------------------------------
{
    AppBoard::logStream().printf("[RobotController::setHome]\n");
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
    AppBoard::logStream().printf("[RobotController::setPosition]\n");
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
    AppBoard::logStream().printf("[RobotController::changeMode]\n");
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
    AppBoard::logStream().printf("[RobotController::doFunctionButtons]\n");

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
            setSpeed( _monkey.getSpeedScale() - 5);
            AppBoard::lcd().updateSpeed( _monkey.getSpeedScale() );
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
            clearWayPoints();
            setHome();
            break;
        case RemoteMessage::MODE_REPLAY:
            setSpeed(_monkey.getSpeedScale() + 5);
            AppBoard::lcd().updateSpeed( _monkey.getSpeedScale() );
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
    AppBoard::logStream().printf("[RobotController::setMode] %d -> %d\n", _mode, mode);

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
void RobotController::play(bool option)
//------------------------------------------------------------------------------
{
    AppBoard::logStream().printf("[RobotController::play] %d\n", option);
    lockRobot(true);
    _monkey.stop();
    _playWayPoints = option && (_mode == RemoteMessage::MODE_REPLAY);
    lockRobot(false);
}

//------------------------------------------------------------------------------
void RobotController::recordPosition()
//------------------------------------------------------------------------------
{
    AppBoard::logStream().printf("[RobotController::recordPosition]\n");
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
    AppBoard::logStream().printf("[RobotController::setSpeed] %d\n", speed);
    lockRobot(true);
    _monkey.setSpeedScale(speed);
    lockRobot(false);
}

//------------------------------------------------------------------------------
void RobotController::runPlayMode()
//------------------------------------------------------------------------------
{
    AppBoard::logStream().printf("[RobotController::runPlayMode] entered\n");

    AppBoard::lcd().updateModeInfo(RemoteMessage::MODE_REPLAY, getNumWayPoints() );
    setSpeed(0);


    int iCurrentWp = 0;
    int nWp = getNumWayPoints();

    lockRobot(true);
    _monkey.enableMotorPower();
    lockRobot(false);

    while( !_modeButtonPressed && (_mode == RemoteMessage::MODE_REPLAY) )
    {
        AppBoard::logStream().printf("[RobotController::runPlayMode] loop\n");

        doFunctionButtons();

        // cue next waypoint if we are playing
        lockRobot(true);
        if( _playWayPoints && nWp)
        {
            if( _monkey.isMoveCompleted() )
            {
                iCurrentWp = (iCurrentWp+1) % nWp;
                AppBoard::lcd().updateModeInfo(RemoteMessage::MODE_REPLAY, iCurrentWp);
                _monkey.moveToWaypoint( *getWayPoint(iCurrentWp) );
            } // last waypoint completed

        } // is playing
        lockRobot(false);

    } // while

    AppBoard::logStream().printf("[RobotController::runPlayMode] exited\n");
}

//------------------------------------------------------------------------------
void RobotController::runTeachMode()
//------------------------------------------------------------------------------
{
    AppBoard::logStream().printf("[RobotController::runTeachMode] entered\n");

    AppBoard::lcd().updateModeInfo(RemoteMessage::MODE_TEACH, getNumWayPoints());
    setSpeed(0);

    lockRobot(true);
    _monkey.disableMotorPower();
    lockRobot(false);

    while( !_modeButtonPressed && (_mode == RemoteMessage::MODE_TEACH) )
    {
        AppBoard::logStream().printf("[RobotController::runTeachMode] loop\n");
        doFunctionButtons();
        Thread::wait(100);
    }

    AppBoard::logStream().printf("[RobotController::runTeachMode] exited\n");
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
