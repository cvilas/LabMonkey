//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : IpPortDlg.h
// Brief    : IP connection dialog
//==============================================================================

#ifndef IPPORTDLG_H
#define IPPORTDLG_H

#include <QDialog>
#include <io/TcpSocket.h>

namespace Ui {
class IpPortDlg;
}

//==============================================================================
/// \brief connects to remote TCP host
class IpPortDlg : public QDialog
{
    Q_OBJECT
    
public:
    explicit IpPortDlg(QWidget *parent = 0);
    ~IpPortDlg();
    Grape::TcpSocket* getSocket() { return _pSocket; }
    
private slots:
    void on_addressSel_editingFinished();
    void on_connectBtn_clicked();

private:
    Ui::IpPortDlg *_pUi;
    Grape::TcpSocket* _pSocket;
};

#endif // IPPORTDLG_H
