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
    LabMonkey(const std::string& port, Grape::SerialPort::BaudRate b, bool verbose, std::ostream& log);

    ~LabMonkey();

    void setHome();

    void enable();

    void disable();

    void waitPosition();

    void getPositions(int p[5]);

    void playWaypoints(WayPoint* points, int nwp, bool waitCompletion = true);

    void run();

private:
    Transport _transport;
    Motor* _pMotor[5];
};

#endif // LABMONKEY_H
