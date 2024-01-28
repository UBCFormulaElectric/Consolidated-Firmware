#pragma once
#include "racing/RacingBase.h"
#include "ui_AutocrossPage.h"

class AutocrossPage : public RacingBase
{
    Q_OBJECT
    std::unique_ptr<ui::AutocrossPage> ui = std::make_unique<ui::AutocrossPage>();

  public:
    explicit AutocrossPage(QWidget *parent = nullptr);
};
