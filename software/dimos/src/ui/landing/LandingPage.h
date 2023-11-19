#pragma once

#include "ui_LandingPage.h"

class LandingPage: public QWidget {
Q_OBJECT
public:
	explicit LandingPage(QWidget *parent = nullptr);

private:
	std::unique_ptr<ui::LandingPage> ui;
};