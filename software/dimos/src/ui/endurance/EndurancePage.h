#pragma once
#include "racingbase/RacingBase.h"
#include "ui_EndurancePage.h"

class EndurancePage: public RacingBase {
	std::unique_ptr<ui::EndurancePage> ui = std::make_unique<ui::EndurancePage>();
public:
	explicit EndurancePage(QWidget * parent = nullptr);
};
