//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : MainWindow.cpp
//==============================================================================

#include "MainWindow.h"
#include "ui_MainWindow.h"

//==============================================================================
MainWindow::MainWindow(QWidget *parent)
//==============================================================================
    :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

//------------------------------------------------------------------------------
MainWindow::~MainWindow()
//------------------------------------------------------------------------------
{
    delete ui;
}
