#include <QGridLayout>
#include "SwitcherButton.h"

SwitcherButton::SwitcherButton(SwitcherButtonEvent event, QWidget *parent) : QPushButton(parent) {
	setupUI(event);
}

void SwitcherButton::setupUI(SwitcherButtonEvent event) {
	this->setFixedSize(QSize(200, 200));
	this->setStyleSheet("QPushButton {"
						"	background-color:#002D67;"
						"	border-radius: 50px;"
						"}");
	this->setContentsMargins(0, 0, 0, 0);

	auto gridLayout = new QGridLayout(this);
	iconLabel = std::make_unique<QLabel>();
	auto icon = QIcon(eventToImagePath[event]);
	auto iconPixmap = icon.pixmap(QSize(140, 130));
	iconLabel->setPixmap(iconPixmap);
	gridLayout->addWidget(iconLabel.get(), 0, 0, Qt::AlignCenter);
	this->setLayout(gridLayout);
}
