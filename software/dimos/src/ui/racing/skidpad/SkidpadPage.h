#pragma once
#include "racing/RacingBase.h"
#include "ui_SkidpadPage.h"

class SkidpadPage: public RacingBase {
	std::unique_ptr<ui::SkidpadPage> ui = std::make_unique<ui::SkidpadPage>();
public:
	explicit SkidpadPage(QWidget * parent = nullptr);
};
