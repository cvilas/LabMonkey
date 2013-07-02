#include "labmonkey.h"
#include <iostream>

int main()
{
    const std::string port = "/dev/ttyACM0";
    Grape::SerialPort::BaudRate b = Grape::SerialPort::B9600;
    bool verbose = false;

    LabMonkey monkey(port, b, verbose, std::cout);
    monkey.run();
    return 0;
}

