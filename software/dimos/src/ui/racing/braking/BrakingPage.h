#pragma once
#include "racing/RacingBase.h"
#include "ui_BrakingPage.h"

class BrakingPage : public RacingBase
{
    std::unique_ptr<ui::BrakingPage> ui = std::make_unique<ui::BrakingPage>();

  public:
    explicit BrakingPage(QWidget *parent = nullptr);
};
