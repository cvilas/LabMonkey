#ifndef LABMONKEY_H
#define LABMONKEY_H

#include "motor.h"

typedef struct _motor_config
{
    unsigned int id;
    unsigned int max_vel;
    unsigned int acc;
}MotorConfig;


typedef struct _way_point
{
    int pos[5];
}WayPoint;

class LabMonkey
{
public:
    static MotorConfig _motorConfig[5];

public:
    LabMonkey(int baud, bool verbose);

    ~LabMonkey();

    void setHome();

    void enable();

    void disable();

    void waitPosition();

    void getPositions(int p[5]);

    void playWaypoints(WayPoint* points, int nwp, bool waitCompletion = true);

    void run();

    bool isRecBtn();
    
    bool isPlayBtn();
    
    bool isShowBtn();
    
    bool isClrBtn();
    
    bool isHomeBtn();
    
private:
    Transport _transport;
    Motor* _pMotor[5];
    
    DigitalIn _recBtn;
    DigitalIn _playBtn;
    DigitalIn _showBtn;
    DigitalIn _clrBtn;
    DigitalIn _homeBtn;
    
    bool _recBtnState;
    bool _playBtnState;
    bool _showBtnState;
    bool _clrBtnState;
    bool _homeBtnState;
};

#endif // LABMONKEY_H
