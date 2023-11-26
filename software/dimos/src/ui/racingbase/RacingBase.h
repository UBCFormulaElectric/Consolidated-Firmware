#pragma once
#include <QFrame>
#include "ui_RacingBase.h"



class RacingBase: public QFrame {
Q_OBJECT
public:
	explicit RacingBase(QWidget *parent, std::unique_ptr<QFrame> innerElements);

private:
	std::unique_ptr<ui::RacingBase> ui;
};