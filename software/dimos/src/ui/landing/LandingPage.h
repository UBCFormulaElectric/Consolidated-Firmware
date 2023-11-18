#pragma once

#include <QFrame>
#include "ui_LandingPage.h"

class LandingPage: public QFrame {
Q_OBJECT
public:
	explicit LandingPage(QWidget *parent = nullptr);

private:
	std::unique_ptr<ui::LandingPage> ui;
};