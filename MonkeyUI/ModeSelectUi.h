//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : ModeSelectUi.h
// Brief    : Mode selection dialog
//==============================================================================

#ifndef MODESELECTUI_H
#define MODESELECTUI_H

#include <QWidget>
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
    
private slots:
    void on_teachModeBtn_clicked();
    void on_replayModeBtn_clicked();

private:
    Ui::ModeSelectUi *_pUi;
    ConsoleClient& _messenger;
};

#endif // MODESELECTUI_H
