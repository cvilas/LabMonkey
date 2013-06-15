//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : main.cpp
// Brief    : Main function for the Lab Monkey GUI
//==============================================================================

#include "MainWindow.h"
#include <QApplication>

//==============================================================================
int main(int argc, char *argv[])
//==============================================================================
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.showFullScreen();
    w.show();
    
    return a.exec();
}
