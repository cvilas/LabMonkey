//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : PlayModeUi.h
// Brief    : UI for replay mode
//==============================================================================

#ifndef PLAYMODEUI_H
#define PLAYMODEUI_H

#include <QWidget>
#include <QIcon>
#include "ConsoleClient.h"

namespace Ui {
class PlayModeUi;
}

//==============================================================================
/// \brief play mode controls
class PlayModeUi : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlayModeUi(ConsoleClient& client, QWidget *parent = 0);
    ~PlayModeUi();
    void init();

private slots:
    void on_playBtn_clicked();
    void on_speedSlider_sliderReleased();

private:
    ConsoleClient& _messenger;
    Ui::PlayModeUi *_pUi;
    QIcon _icoEnabled;
    QIcon _icoDisabled;
    QIcon _icoStandby;
};

#endif // PLAYMODEUI_H
