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
    _messenger(client)
{
    _pUi->setupUi(this);
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
    RemoteMessage::Mode m = _messenger.setMode(RemoteMessage::MODE_TEACH);

    /// \todo
    /// handle reply
}

//------------------------------------------------------------------------------
void ModeSelectUi::on_replayModeBtn_clicked()
//------------------------------------------------------------------------------
{
    RemoteMessage::Mode m = _messenger.setMode(RemoteMessage::MODE_TEACH);

    /// \todo
    /// handle reply
}
