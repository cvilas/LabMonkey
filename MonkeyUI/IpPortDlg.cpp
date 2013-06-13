//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : IpPortDlg.cpp
//==============================================================================

#include "IpPortDlg.h"
#include "ui_IpPortDlg.h"

//==============================================================================
IpPortDlg::IpPortDlg(Grape::TcpSocket& socket, QWidget *parent)
//==============================================================================
    : QDialog(parent),
    _pUi(new Ui::IpPortDlg),
    _socket(socket)
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
    _socket.close();
    bool success = _socket.connect(_pUi->addressSel->text().toStdString(), _pUi->portSel->text().toInt());
    if( success )
    {
        _pUi->statusLbl->setText(QString("Connected to ") + _pUi->addressSel->text());
    }
    else
    {
        _pUi->statusLbl->setText("No can do");
    }

}
