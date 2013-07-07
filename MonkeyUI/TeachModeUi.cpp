//==============================================================================
// Project  : Lab Monkey
// Module   : GUI
// File     : TeachModeUi.cpp
//==============================================================================

#include "TeachModeUi.h"
#include "ui_TeachModeUi.h"

//==============================================================================
TeachModeUi::TeachModeUi(ConsoleClient& client, QWidget *parent)
//==============================================================================
    : _messenger(client),
    QWidget(parent),
    _pUi(new Ui::TeachModeUi)
{
    _pUi->setupUi(this);
}

//------------------------------------------------------------------------------
void TeachModeUi::init()
//------------------------------------------------------------------------------
{
    int nwp = _messenger.getNumWayPoints();
    _pUi->wayPointsLabel->setText(QString::number(nwp) + QString(" waypoints"));
}

//------------------------------------------------------------------------------
TeachModeUi::~TeachModeUi()
//------------------------------------------------------------------------------
{
    delete _pUi;
}

//------------------------------------------------------------------------------
void TeachModeUi::on_learnBtn_clicked()
//------------------------------------------------------------------------------
{
    int nwp = _messenger.addWayPoint();
    _pUi->wayPointsLabel->setText(QString::number(nwp) + QString(" waypoints"));
}

//------------------------------------------------------------------------------
void TeachModeUi::on_homeBtn_clicked()
//------------------------------------------------------------------------------
{
    if( _messenger.setHome() )
    {
        _pUi->wayPointsLabel->setText(QString::number(0) + QString(" waypoints"));
    }
}

//------------------------------------------------------------------------------
void TeachModeUi::on_clearBtn_clicked()
//------------------------------------------------------------------------------
{
    int nwp = _messenger.clearWayPoints();
    _pUi->wayPointsLabel->setText(QString::number(nwp) + QString(" waypoints"));
}
