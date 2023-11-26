#pragma once
#include <QFrame>
#include "ui_RacingBase.h"



class RacingBase: public QFrame {
Q_OBJECT
public:
	explicit RacingBase(QWidget *parent, QFrame * innerElements, const QString& pageName);

private:
	std::unique_ptr<ui::RacingBase> ui = std::make_unique<ui::RacingBase>();
};