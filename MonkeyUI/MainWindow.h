//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : MainWindow.h
// Brief    : Top level window for the Lab Monkey GUI
//==============================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
}; // MainWindow

#endif // MAINWINDOW_H
