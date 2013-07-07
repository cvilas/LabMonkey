//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : ModeSelectUi.h
// Brief    : Mode selection dialog
//==============================================================================

#ifndef MODESELECTUI_H
#define MODESELECTUI_H

#include <QWidget>
#include <QIcon>
#include <ConsoleClient.h>

namespace Ui {
class ModeSelectUi;
}

//==============================================================================
/// \brief select a control mode
class ModeSelectUi : public QWidget
{
    Q_OBJECT
    
public:
    explicit ModeSelectUi(ConsoleClient& client, QWidget *parent = 0);
    ~ModeSelectUi();
    void init();

signals:
    void modeSelected(RemoteMessage::Mode m);
private slots:
    void on_teachModeBtn_clicked();
    void on_replayModeBtn_clicked();

    void setModeFeedback(RemoteMessage::Mode m);

private:
    Ui::ModeSelectUi *_pUi;
    ConsoleClient& _messenger;
    QIcon _icoEnabled;
    QIcon _icoDisabled;
    QIcon _icoStandby;
};

#endif // MODESELECTUI_H
