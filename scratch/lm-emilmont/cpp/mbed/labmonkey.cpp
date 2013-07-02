#include "labmonkey.h"
#include <iostream>
#include <vector>

MotorConfig LabMonkey::_motorConfig[5] = {
    {1, 3000, 5000},
    {2, 200, 500},
    {3, 300, 500},
    {4, 1500, 5000},
    {5, 2000, 5000}
};

//DigitalIn down(p12);
//DigitalIn left(p13);
//DigitalIn center(p14);
//DigitalIn up(p15);
//DigitalIn right(p16);

LabMonkey::LabMonkey(int baud, bool verbose)
    : _transport(baud, verbose),
    _recBtn(p14)/*center*/,
    _playBtn(p16)/*right*/,
    _showBtn(p13)/*left*/,
    _clrBtn(p12)/*down*/,
    _homeBtn(p15) /*up*/,
    _recBtnState(false),
    _playBtnState(false),
    _showBtnState(false),
    _clrBtnState(false),
    _homeBtnState(false)
{
    for(int i = 0; i < 5; ++i)
    {
        _pMotor[i] = new Motor(_transport, _motorConfig[i].id);
        _pMotor[i]->setVelocityLimit(_motorConfig[i].max_vel);
        _pMotor[i]->setAcceleration(_motorConfig[i].acc);
        _pMotor[i]->disable();
    }
}

LabMonkey::~LabMonkey()
{
    for(int i = 0; i < 5; ++i)
    {
        delete _pMotor[i];
    }
}

void LabMonkey::setHome()
{
    for(int i = 0; i < 5; ++i)
    {
        _pMotor[i]->home();
    }
}

void LabMonkey::enable()
{
    for(int i = 0; i < 5; ++i)
    {
        _pMotor[i]->enable();
    }
}

void LabMonkey::disable()
{
    for(int i = 0; i < 5; ++i)
    {
        _pMotor[i]->disable();
    }
}

void LabMonkey::waitPosition()
{
    for(int i = 0; i < 5; ++i)
    {
        _pMotor[i]->waitPosition();
    }
}

void LabMonkey::getPositions(int p[5])
{
    for(int i = 0; i < 5; ++i)
    {
        p[i] = _pMotor[i]->getPosition();
    }
}

void LabMonkey::playWaypoints(WayPoint* points, int nwp, bool waitCompletion)
{
    int currentPos[5];
    getPositions(currentPos);

    for( int i = 0; i < nwp; ++i )
    {
        int movementTime = 0;
        int distances[5];
        for(int m = 0; m < 5; ++m)
        {
            distances[m] = abs( _pMotor[m]->lastPosition - points[i].pos[m] );
            movementTime = std::max( movementTime, (distances[m]/_pMotor[m]->getVelocityLimit()) );
        }
        //std::cout << "T: " << movementTime << std::endl;

        //std::cout << "v: ";
        for(int m = 0; m < 5; ++m)
        {
            int v = std::max( (distances[m]/movementTime), 60);
            _pMotor[m]->setMaxSpeed(v);
            //std::cout << v << " ";
        }
        //std::cout << std::endl;

        //std::cout << "p: ";
        for(int m = 0; m < 5; ++m)
        {
            //std::cout << points[i].pos[m] << " ";
            _pMotor[m]->moveToLocation( points[i].pos[m] );
        }
        //std::cout << std::endl;

        if( waitCompletion )
        {
            waitPosition();
        }
    }
}

bool LabMonkey::isRecBtn()
{
    bool pushed = false;
    int b = _recBtn.read();
    if( b && !_recBtnState ) 
    {
        pushed = true;
    }
    _recBtnState = b;
    return pushed;
}

bool LabMonkey::isPlayBtn()
{
    bool pushed = false;
    int b = _playBtn.read();
    if( b && !_playBtnState ) 
    {
        pushed = true;
    }
    _playBtnState = b;
    return pushed;
}

bool LabMonkey::isShowBtn()
{
    bool pushed = false;
    int b = _showBtn.read();
    if( b && !_showBtnState ) 
    {
        pushed = true;
    }
    _showBtnState = b;
    return pushed;
}

bool LabMonkey::isClrBtn()
{
    bool pushed = false;
    int b = _clrBtn.read();
    if( b && !_clrBtnState ) 
    {
        pushed = true;
    }
    _clrBtnState = b;
    return pushed;
}

bool LabMonkey::isHomeBtn()
{
    bool pushed = false;
    int b = _homeBtn.read();
    if( b && !_homeBtnState ) 
    {
        pushed = true;
    }
    _homeBtnState = b;
    return pushed;
}
       
void LabMonkey::run()
{
    std::vector<WayPoint> waypoints;
    Serial& userConsole = _transport.getConsole(); 
    while(1)
    {        

        if( isRecBtn() )
        {
            WayPoint wp;
            getPositions(wp.pos);
            waypoints.push_back(wp);
        }

        else if( isPlayBtn() )
        {
            enable();
            playWaypoints(&waypoints[0], waypoints.size(), true);
            disable();
        }

        else if( isShowBtn() )
        {
            for(int i = 0; i < waypoints.size(); ++i)
            {
                std::cout << waypoints[i].pos[0]
                        << " " << waypoints[i].pos[1]
                        << " " << waypoints[i].pos[2]
                        << " " << waypoints[i].pos[3]
                        << " " << waypoints[i].pos[4]
                        << std::endl;

            }
        }
        else if( isClrBtn() )
        {
            waypoints.clear();
        }

        else if( isHomeBtn() )
        {
            setHome();
        }
        
    } // the big while
}
