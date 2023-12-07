#pragma once
#include "racing/RacingBase.h"
#include "ui_AccelerationPage.h"

class AccelerationPage : public RacingBase
{
    std::unique_ptr<ui::AccelerationPage> ui = std::make_unique<ui::AccelerationPage>();

  public:
    explicit AccelerationPage(QWidget *parent = nullptr);
};
