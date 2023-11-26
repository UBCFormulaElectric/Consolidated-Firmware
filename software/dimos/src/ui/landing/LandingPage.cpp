#include "LandingPage.h"
#include "constants.h"

LandingPage::LandingPage(QWidget *parent): QFrame(parent) {
	this->setGeometry(QRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
	ui->setupUi(this);
}
