//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : MainWindow.cpp
//==============================================================================

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include "IpPortDlg.h"

//==============================================================================
MainWindow::MainWindow(QWidget *parent)
//==============================================================================
    : QMainWindow(parent),
    _pUi(new Ui::MainWindow),
    _modeSelect(_messenger)
{
    _pUi->setupUi(this);

    _pConnectionStatus = new QLabel;
    _pErrorInfo = new QLabel;
    _pUi->statusBar->addWidget(_pConnectionStatus);
    _pUi->statusBar->addWidget(_pErrorInfo);

    this->setCentralWidget(&_modeSelect);
}

//------------------------------------------------------------------------------
MainWindow::~MainWindow()
//------------------------------------------------------------------------------
{
    _messenger.setTransport(NULL);
    delete _pConnectionStatus;
    delete _pErrorInfo;
    delete _pUi;
}

//------------------------------------------------------------------------------
void MainWindow::on_actionExit_triggered()
//------------------------------------------------------------------------------
{
    close();
}

//------------------------------------------------------------------------------
void MainWindow::on_actionTcpConnect_triggered()
//------------------------------------------------------------------------------
{
    IpPortDlg* pDlg = new IpPortDlg(this);
    pDlg->show();
    pDlg->exec();

    Grape::TcpSocket* pSocket = pDlg->getSocket();
    if( pSocket == NULL )
    {
        _pConnectionStatus->setText("Not Connected");
        _messenger.setTransport(NULL);
    }
    else
    {
        _pConnectionStatus->setText(QString("connected to ") + QString(pSocket->getPeerName().c_str()) );
        _messenger.setTransport(pSocket);
    }

}
