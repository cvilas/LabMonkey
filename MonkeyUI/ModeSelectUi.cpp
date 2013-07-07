//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : ModeSelectUi.cpp
//==============================================================================

#include "ModeSelectUi.h"
#include "ui_ModeSelectUi.h"
#include <QDebug>

//==============================================================================
ModeSelectUi::ModeSelectUi(ConsoleClient& client, QWidget *parent)
//==============================================================================
    : QWidget(parent),
    _pUi(new Ui::ModeSelectUi),
    _messenger(client),
    _icoEnabled(QPixmap(":images/EnabledLamp.png")),
    _icoDisabled(QPixmap(":images/DisabledLamp.png")),
    _icoStandby(QPixmap(":images/StandbyLamp.png"))
{
    _pUi->setupUi(this);
}

//------------------------------------------------------------------------------
void ModeSelectUi::init()
//------------------------------------------------------------------------------
{
    RemoteMessage::Mode m = _messenger.getMode();
    setModeFeedback(m);
}

//------------------------------------------------------------------------------
ModeSelectUi::~ModeSelectUi()
//------------------------------------------------------------------------------
{
    delete _pUi;
}

//------------------------------------------------------------------------------
void ModeSelectUi::on_teachModeBtn_clicked()
//------------------------------------------------------------------------------
{
    if( _messenger.getTransport() )
    {
        _pUi->teachModeBtn->setIcon(_icoStandby);
        _pUi->teachModeBtn->repaint();
        RemoteMessage::Mode m = _messenger.setMode(RemoteMessage::MODE_TEACH);
        setModeFeedback(m);
    }
}

//------------------------------------------------------------------------------
void ModeSelectUi::on_replayModeBtn_clicked()
//------------------------------------------------------------------------------
{
    if( _messenger.getTransport() )
    {
        _pUi->replayModeBtn->setIcon(_icoStandby);
        _pUi->replayModeBtn->repaint();
        RemoteMessage::Mode m = _messenger.setMode(RemoteMessage::MODE_REPLAY);
        setModeFeedback(m);
    }
}

//------------------------------------------------------------------------------
void ModeSelectUi::setModeFeedback(RemoteMessage::Mode m)
//------------------------------------------------------------------------------
{
    switch(m)
    {
    case RemoteMessage::MODE_REPLAY:
        _pUi->replayModeBtn->setIcon(_icoEnabled);
        _pUi->teachModeBtn->setIcon(_icoDisabled);
        break;
    case RemoteMessage::MODE_TEACH:
        _pUi->replayModeBtn->setIcon(_icoDisabled);
        _pUi->teachModeBtn->setIcon(_icoEnabled);
        break;
    default:
        _pUi->replayModeBtn->setIcon(_icoDisabled);
        _pUi->teachModeBtn->setIcon(_icoDisabled);
        break;
    }
    emit modeSelected(m);
    update();
}

