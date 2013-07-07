//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : TeachModeUi.h
// Brief    : UI for teach mode
//==============================================================================

#ifndef TEACHMODEUI_H
#define TEACHMODEUI_H

#include <QWidget>
#include "ConsoleClient.h"

namespace Ui {
class TeachModeUi;
}

//==============================================================================
class TeachModeUi : public QWidget
{
    Q_OBJECT
    
public:
    explicit TeachModeUi(ConsoleClient &client, QWidget *parent = 0);
    ~TeachModeUi();
    void init();
    
private slots:
    void on_learnBtn_clicked();
    void on_homeBtn_clicked();
    void on_clearBtn_clicked();

private:
    ConsoleClient& _messenger;
    Ui::TeachModeUi *_pUi;
};

#endif // TEACHMODEUI_H
