#include "RacingBase.h"

RacingBase::RacingBase(QWidget *parent, QFrame* innerElements, const QString& pageName) : QFrame(parent) {
	this->setObjectName(pageName);
	this->setGeometry(QRect(0, 0, 1024, 600));
	ui->setupUi(this, innerElements);
}