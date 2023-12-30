#pragma once
#include "racing/RacingBase.h"
#include "ui_EndurancePage.h"

class EndurancePage : public RacingBase
{
    Q_OBJECT
    std::unique_ptr<ui::EndurancePage> ui = std::make_unique<ui::EndurancePage>();

  public:
    explicit EndurancePage(QWidget *parent = nullptr);
};
