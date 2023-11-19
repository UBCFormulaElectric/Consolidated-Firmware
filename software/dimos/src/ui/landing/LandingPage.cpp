#include "LandingPage.h"

LandingPage::LandingPage(QWidget *parent): QWidget(parent), ui(new ui::LandingPage()) {
	this->setGeometry(QRect(0, 0, 1024, 600));
	ui->setupUi(this);
}
