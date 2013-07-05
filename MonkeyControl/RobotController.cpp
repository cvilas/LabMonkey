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
    : _isConsoleActive(false), _mode(RemoteMessage::MODE_UNKNOWN),
      _playWayPoints(false), _isProcessingModes(false)
{
    for(int i = 0; i < 4; ++i)
    {
        _pBtnIntr[i] = NULL;
    }
}

//------------------------------------------------------------------------------
RobotController::~RobotController()
//------------------------------------------------------------------------------
{
    for(int i = 0; i < 4; ++i)
    {
        if( _pBtnIntr[i] ) { delete _pBtnIntr[i]; _pBtnIntr[i] = NULL; }
    }
}

//------------------------------------------------------------------------------
bool RobotController::init()
//------------------------------------------------------------------------------
{
    setMode(RemoteMessage::MODE_TEACH);
    return true;
}

//------------------------------------------------------------------------------
void RobotController::setConsoleActive(bool option)
//------------------------------------------------------------------------------
{
    _isConsoleActive = option;
    setMode( getMode() );
}

//------------------------------------------------------------------------------
void RobotController::onRecBtn()
//------------------------------------------------------------------------------
{
    if( _mode == RemoteMessage::MODE_TEACH )
    {
        _monkeyLock.lock();
        LabMonkey::WayPoint wp;
        _monkey.getPosition(wp.pos);
        appendWayPoint(wp);
        _monkeyLock.unlock();
    }
}

//------------------------------------------------------------------------------
void RobotController::onClearBtn()
//------------------------------------------------------------------------------
{
    if( _mode == RemoteMessage::MODE_TEACH )
    {
        _monkeyLock.lock();
        clearWayPoints();
        _monkeyLock.unlock();
    }
}

//------------------------------------------------------------------------------
void RobotController::onHomeBtn()
//------------------------------------------------------------------------------
{
    if( _mode == RemoteMessage::MODE_TEACH )
    {
        _monkeyLock.lock();
        clearWayPoints();
        _monkey.setHome();
        _monkeyLock.unlock();
    }
}

//------------------------------------------------------------------------------
void RobotController::setPosition(int p[LabMonkey::NUM_JOINTS])
//------------------------------------------------------------------------------
{
    if( _mode == RemoteMessage::MODE_TEACH )
    {
        _monkeyLock.lock();
        clearWayPoints();
        _monkey.setPosition(p);
        _monkeyLock.unlock();
    }
}

//------------------------------------------------------------------------------
void RobotController::onModeBtn()
//------------------------------------------------------------------------------
{
    switch( _mode )
    {
    case RemoteMessage::MODE_TEACH:
        setMode( RemoteMessage::MODE_REPLAY );
        break;
    case RemoteMessage::MODE_REPLAY:
        setMode( RemoteMessage::MODE_TEACH );
        break;
    default:
        AppBoard::logStream().printf("[RobotController::onModeBtn] invalid current mode\n");
        break;
    };
}

//------------------------------------------------------------------------------
void RobotController::setMode(RemoteMessage::Mode mode)
//------------------------------------------------------------------------------
{
    play(false);

    // stop mode processing, ignore button presses
    _mode = RemoteMessage::MODE_UNKNOWN;

    //wait for mode processing to exit
    while( _isProcessingModes )
    {
        Thread::wait(100);
    }

    // initialise new mode
    for(int i = 0; i < 4; ++i)
    {
        if( _pBtnIntr[i] ) { delete _pBtnIntr[i]; _pBtnIntr[i] = NULL; }
    }

    // map buttons only if remote console is not in control
    if( !isConsoleActive() )
    {
        // map the mode button
        _pBtnIntr[0] = new InterruptIn(AppBoard::BTN_MODE);
        _pBtnIntr[0]->rise(this, &RobotController::onModeBtn );

        // map all other buttons
        switch( mode )
        {
        case RemoteMessage::MODE_REPLAY:
            _pBtnIntr[1] = new InterruptIn(AppBoard::BTN_PLAY);
            _pBtnIntr[1]->rise(this, &RobotController::onPlayBtn);

            _pBtnIntr[2] = new InterruptIn(AppBoard::BTN_SPEED_UP);
            _pBtnIntr[2]->rise(this, &RobotController::onSpeedUpBtn);

            _pBtnIntr[3] = new InterruptIn(AppBoard::BTN_SPEED_DN);
            _pBtnIntr[3]->rise(this, &RobotController::onSpeedDnBtn);

            break;

        case RemoteMessage::MODE_TEACH:
            _pBtnIntr[1] = new InterruptIn(AppBoard::BTN_REC);
            _pBtnIntr[1]->rise(this, &RobotController::onRecBtn);

            _pBtnIntr[2] = new InterruptIn(AppBoard::BTN_CLEAR);
            _pBtnIntr[2]->rise(this, &RobotController::onClearBtn);

            _pBtnIntr[3] = new InterruptIn(AppBoard::BTN_HOME);
            _pBtnIntr[3]->rise(this, &RobotController::onHomeBtn);

            break;
        default:
            break;
        };
    }

    // start mode processing
    _mode = mode;
}

//------------------------------------------------------------------------------
void RobotController::onPlayBtn()
//------------------------------------------------------------------------------
{
    play(!_playWayPoints);
}

//------------------------------------------------------------------------------
void RobotController::play(bool option)
//------------------------------------------------------------------------------
{
    _monkeyLock.lock();
    _monkey.stop();
    _playWayPoints = option && (_mode == RemoteMessage::MODE_REPLAY);
    _monkeyLock.unlock();
}

//------------------------------------------------------------------------------
void RobotController::setSpeed(int speed)
//------------------------------------------------------------------------------
{
    if( _mode == RemoteMessage::MODE_REPLAY )
    {
        _monkeyLock.lock();
        _monkey.setSpeedScale(speed);
        _monkeyLock.unlock();
    }
}

//------------------------------------------------------------------------------
void RobotController::onSpeedDnBtn()
//------------------------------------------------------------------------------
{
    setSpeed(_monkey.getSpeedScale() - 5);
}

//------------------------------------------------------------------------------
void RobotController::onSpeedUpBtn()
//------------------------------------------------------------------------------
{
    setSpeed( _monkey.getSpeedScale() + 5);
}

//------------------------------------------------------------------------------
void RobotController::runPlayMode()
//------------------------------------------------------------------------------
{
    AppBoard::logStream().printf("[RobotController::runPlayMode] entered\n");

    // mode controls:
    // - play: implemented here
    // - stop: see onPlayBtn(false)
    // - speed up: see onSpeedUpBtn()
    // - speed down: onSpeedDnBtn()

    int iCurrentWp = 0;
    int nWp = getNumWayPoints();

    _monkey.enableMotorPower();

    while( _mode == RemoteMessage::MODE_REPLAY )
    {
        // cue next waypoint if we are playing
        _monkeyLock.lock();
        if( _playWayPoints && nWp)
        {
            if( _monkey.isMoveCompleted() )
            {
                iCurrentWp = (iCurrentWp+1) % nWp;
                _monkey.moveToWaypoint( *getWayPoint(iCurrentWp) );
            } // last waypoint completed
        } // is playing
        _monkeyLock.unlock();

    } // while

    AppBoard::logStream().printf("[RobotController::runPlayMode] exited\n");
}

//------------------------------------------------------------------------------
void RobotController::runTeachMode()
//------------------------------------------------------------------------------
{
    AppBoard::logStream().printf("[RobotController::runTeachMode] entered\n");

    _monkey.disableMotorPower();

    while( _mode == RemoteMessage::MODE_TEACH )
    {
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
            Thread::wait(100);
        };
    }

}
