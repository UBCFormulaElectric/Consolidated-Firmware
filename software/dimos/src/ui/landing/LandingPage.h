#pragma once

#include "ui_LandingPage.h"

class LandingPage: public QFrame {
Q_OBJECT
	std::unique_ptr<ui::LandingPage> ui = std::make_unique<ui::LandingPage>();
public:
	explicit LandingPage(QWidget *parent = nullptr);
};