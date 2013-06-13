//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : MainWindow.cpp
//==============================================================================

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>

//==============================================================================
MainWindow::MainWindow(QWidget *parent)
//==============================================================================
    :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _client(_transport)
{
    ui->setupUi(this);
}

//------------------------------------------------------------------------------
MainWindow::~MainWindow()
//------------------------------------------------------------------------------
{
    delete ui;
}

//------------------------------------------------------------------------------
void MainWindow::connectToServer(const QString& ip, int port)
//------------------------------------------------------------------------------
{
    _transport.close();
    if( !_transport.connect(ip.toStdString(), port) )
    {
        QMessageBox::critical(this, "Unable to connect", "No can do :(");
        return;
    }
}

//------------------------------------------------------------------------------
void MainWindow::on_actionConnect_triggered()
//------------------------------------------------------------------------------
{

}
