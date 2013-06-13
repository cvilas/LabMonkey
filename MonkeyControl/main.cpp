
#include "mbed.h"
#include "rtos.h"
#include "C12832_lcd.h"
#include "CommandServer.h"
#include <vector>

/*
Design notes:

1. Command server thread (serial port)
- block waits for remote commands
- on GET_POSITION command, 
    -- reads shared memory 'current state buffer' for position
    -- replies to remote client
- on any other command
    -- verifies it, 
    -- copies to shared memory 'desired state buffer'
    -- Adds notification to 'command notification queue'
- block waits on 'reply notification queue'
- On reply notification 
    -- reads shared memory 'current state buffer'
    -- replies to remote client

2. Robot controller thread
- initialise motor controllers
- runs in a loop at all times
- at the top of the loop
    -- check command notification queue to check if command pending
        -- if command pending, process 'desired state buffer' for the command and set notify 'reply notification queue'
- do loop
    -- process button press flags
    -- process global velocity
- at the end of loop
    -- update shared memory 'current state buffer'

3. interrupt service routines
- update flags on button presses

Robot thread processes:
- mode control
- maintains motor waypoint list (position, max velocity, acceleration)
- allows modification of waypoint list
- Drives LCD (displays mode and immediate status)
- Drives lamp
- Drives buzzer 
- Reads and sets speed

Implementation steps
1. Define comms messages
2. Implement client-server code and test (Sunday June 9th)
3. Prepare tablet and desktop for android qt development
4. Implement command interface GUI (by Tue 11th)
5. Implement mbed - shared memory, notification queue (Wed 12th)
6. Implement mbed - robot controller emulator thread (Thu 13th)
7. Implement GUI tasklist editor (Fri, Sat 15th)
8. Integrate command thread and controller thread (Sun 16th)
9. Implement rest of the essential elements in GUI
10. Implement GUI 3D viewer for desktop guis (take coin and soqt from synaptic)
11. Desktop GUI test
12. Tablet GUI test
*/
C12832_LCD lcd;

void command_thread(void const* arg)
{
    CommandServer cs(lcd);
    if( cs.init(1234) )
    {
        cs.run();
    }
}

int main()
{
    Thread commandserver(command_thread);

    while(1)
    {
        Thread::wait(1000);
    }
}
