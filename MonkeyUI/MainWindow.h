//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : MainWindow.h
// Brief    : Top level window for the Lab Monkey GUI
//==============================================================================

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ConsoleClient.h"
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
    void on_actionConnect_triggered();

private:
    void connectToServer(const QString& ip, int port);

private:
    Ui::MainWindow *ui;
    Grape::TcpSocket _transport;
    ConsoleClient _client;
}; // MainWindow

#endif // MAINWINDOW_H
