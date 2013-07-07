//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : IpPortDlg.cpp
//==============================================================================

#include "IpPortDlg.h"
#include "ui_IpPortDlg.h"
#include <QDebug>
#include <errno.h>

//==============================================================================
IpPortDlg::IpPortDlg(QWidget *parent)
//==============================================================================
    : QDialog(parent),
    _pUi(new Ui::IpPortDlg),
    _pSocket(NULL)
{
    _pUi->setupUi(this);
}

//------------------------------------------------------------------------------
IpPortDlg::~IpPortDlg()
//------------------------------------------------------------------------------
{
    delete _pUi;
}

//------------------------------------------------------------------------------
void IpPortDlg::on_addressSel_editingFinished()
//------------------------------------------------------------------------------
{

}

//------------------------------------------------------------------------------
void IpPortDlg::on_connectBtn_clicked()
//------------------------------------------------------------------------------
{
    if( _pSocket )
    {
        _pSocket->close();
        _pSocket = NULL;
    }

    _pSocket = new Grape::TcpSocket;
    bool success = _pSocket->connect(_pUi->addressSel->text().toStdString(), _pUi->portSel->text().toInt());
    if( success )
    {
        _pUi->statusLbl->setText("Connected");
        _pUi->statusLbl->setPixmap(QPixmap(":images/LiveMonkey.png"));
    }
    else
    {
        delete _pSocket;
        _pSocket = NULL;
        _pUi->statusLbl->setText("Unable");
        _pUi->statusLbl->setPixmap(QPixmap(":images/DeadMonkey.png"));
    }

}
