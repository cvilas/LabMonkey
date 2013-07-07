//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : MainWindow.cpp
//==============================================================================

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QMessageBox>
#include <QFile>
#include "IpPortDlg.h"
#include "PlayModeUi.h"
#include "TeachModeUi.h"

//==============================================================================
MainWindow::MainWindow(QWidget *parent)
//==============================================================================
    : QMainWindow(parent),
    _pUi(new Ui::MainWindow),
    _modeSelect(_messenger)
{
    // Set stylesheet
    QFile styleFile(":/MonkeyUI.qss");
    styleFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleFile.readAll());
    styleFile.close();
    setStyleSheet(styleSheet);

    _pUi->setupUi(this);
    setWindowIcon(QIcon(QPixmap(":images/logo.png")));

    _pConnectionStatus = new QLabel;
    _pErrorInfo = new QLabel;
    _pUi->statusBar->addWidget(_pConnectionStatus);
    _pUi->statusBar->addWidget(_pErrorInfo);

    _controlsLayout.addWidget(&_modeSelect, 1);
    _controlsLayout.addWidget(new QWidget);

    QWidget* pWidget = new QWidget;
    pWidget->setLayout(&_controlsLayout);
    this->setCentralWidget(pWidget);

    QObject::connect(&_modeSelect, SIGNAL(modeSelected(RemoteMessage::Mode)), this, SLOT(onModeSelected(RemoteMessage::Mode)));
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
    _messenger.setTransport(pSocket);
    if( pSocket == NULL )
    {
        _pConnectionStatus->setText("Not Connected");
        _modeSelect.init();
    }
    else
    {
        _pConnectionStatus->setText(QString("connected to ") + QString(pSocket->getPeerName().c_str()) );
        _modeSelect.init();
    }

}

//------------------------------------------------------------------------------
void MainWindow::onModeSelected(RemoteMessage::Mode m)
//------------------------------------------------------------------------------
{
    QLayoutItem* pWidget = _controlsLayout.itemAt(1);
    if( pWidget )
    {
        _controlsLayout.removeItem(pWidget);
        delete pWidget->widget();
    }

    switch(m)
    {
    case RemoteMessage::MODE_TEACH:
    {
        TeachModeUi* pW = new TeachModeUi(_messenger);
        pW->init();
        _controlsLayout.addWidget(pW, 2);
        break;
    }
    case RemoteMessage::MODE_REPLAY:
    {
        PlayModeUi* pW = new PlayModeUi(_messenger);
        pW->init();
        _controlsLayout.addWidget(pW, 2);
        break;
    }
    default:
        _controlsLayout.addWidget(new QWidget, 2);
        break;
    }

}
