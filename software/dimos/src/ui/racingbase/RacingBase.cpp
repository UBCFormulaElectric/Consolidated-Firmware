#include "RacingBase.h"

RacingBase::RacingBase(QWidget *parent, std::unique_ptr<QFrame> innerElements) : QFrame(parent), ui(new ui::RacingBase) {
	ui->setupUi(this, std::move(innerElements));
}