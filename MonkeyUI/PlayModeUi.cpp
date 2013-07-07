//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : PlayModeUi.cpp
//==============================================================================

#include "PlayModeUi.h"
#include "ui_PlayModeUi.h"

//==============================================================================
PlayModeUi::PlayModeUi(ConsoleClient& client, QWidget *parent)
//==============================================================================
    : _messenger(client),
    QWidget(parent),
    _pUi(new Ui::PlayModeUi),
    _icoEnabled(QPixmap(":images/EnabledLamp.png")),
    _icoDisabled(QPixmap(":images/DisabledLamp.png")),
    _icoStandby(QPixmap(":images/StandbyLamp.png"))
{
    _pUi->setupUi(this);
}

//------------------------------------------------------------------------------
void PlayModeUi::init()
//------------------------------------------------------------------------------
{
    _pUi->speedSlider->setValue( _messenger.getSpeed() );

    _pUi->playBtn->setIcon(_icoStandby);
    _pUi->playBtn->repaint();
    bool isPlaying = _messenger.play(false);
    _pUi->playBtn->setIcon( isPlaying ? _icoEnabled : _icoDisabled );

    int nwp = _messenger.getNumWayPoints();
    _pUi->wayPointsLabel->setText(QString::number(nwp) + QString(" waypoints"));

    update();
}

//------------------------------------------------------------------------------
PlayModeUi::~PlayModeUi()
//------------------------------------------------------------------------------
{
    delete _pUi;
}

//------------------------------------------------------------------------------
void PlayModeUi::on_playBtn_clicked()
//------------------------------------------------------------------------------
{
    static bool isPlaying = false;
    _pUi->playBtn->setIcon(_icoStandby);
    _pUi->playBtn->repaint();
    isPlaying = _messenger.play(!isPlaying);
    _pUi->playBtn->setIcon( isPlaying ? _icoEnabled : _icoDisabled );
    update();
}

//------------------------------------------------------------------------------
void PlayModeUi::on_speedSlider_sliderReleased()
//------------------------------------------------------------------------------
{
    int sp = _messenger.setSpeed( _pUi->speedSlider->value() );
    _pUi->speedSlider->setValue(sp);
}
