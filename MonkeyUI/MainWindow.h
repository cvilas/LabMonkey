//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : MainWindow.h
// Brief    : Top level window for the Lab Monkey GUI
//==============================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include "ConsoleClient.h"
#include "ModeSelectUi.h"
#include <io/TcpSocket.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void on_actionTcpConnect_triggered();
    void on_actionExit_triggered();
    void onModeSelected(RemoteMessage::Mode m);

private:
    Ui::MainWindow *_pUi;
    QLabel* _pConnectionStatus;
    QLabel* _pErrorInfo;
    ConsoleClient _messenger;
    ModeSelectUi _modeSelect;
    QVBoxLayout _controlsLayout;
}; // MainWindow

#endif // MAINWINDOW_H
