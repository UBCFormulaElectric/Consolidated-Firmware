#include "Switcher.h"

const int BUTTON_CENTER_X = 1024 / 2;
const int BUTTON_BOTTOM_Y = 600;
const int OFFSET = 50;
const int R = 955;

Switcher::Switcher(QWidget *parent): QFrame(parent) {
	setupUI();
}

void Switcher::setupUI() {
	this->resize(1024, 600);
	this->setMinimumSize(QSize(1024, 600));

	enduranceOption = std::make_unique<SwitcherButton>(ENDURANCE, this);
	enduranceOption->move(center_to_top_left(enduranceOption.get(), place_center(-26.5)));

	accelerationOption = std::make_unique<SwitcherButton>(ACCELERATION, this);
	accelerationOption->move(center_to_top_left(accelerationOption.get(), place_center(-13)));

	skidpadOption = std::make_unique<SwitcherButton>(SKIDPAD, this);
	skidpadOption->move(center_to_top_left(skidpadOption.get(), place_center(0)));

	autoCrossOption = std::make_unique<SwitcherButton>(AUTOCROSS, this);
	autoCrossOption->move(center_to_top_left(autoCrossOption.get(), place_center(13)));

	brakingOption = std::make_unique<SwitcherButton>(BRAKING, this);
	brakingOption->move(center_to_top_left(brakingOption.get(), place_center(26.5)));

	selectedEventText = std::make_unique<QLabel>(this);
	selectedEventText->setText("ENDURANCE");
	selectedEventText->setStyleSheet("QLabel {"
									 "	color: white;"
									 "	font-size: 20px;"
									 "	font-weight: bold;"
									 "}");
	selectedEventText->move(BUTTON_CENTER_X - selectedEventText->width()/2,
							BUTTON_BOTTOM_Y - selectedEventText->height() - 10);
}

QPoint Switcher::place_center(double angle_deg) {
	return  {
		(int) round(BUTTON_CENTER_X + R * sin(angle_deg * M_PI / 180)),
		(int) round(BUTTON_BOTTOM_Y + R - R * cos(angle_deg * M_PI / 180) - OFFSET)
	};
}

QPoint Switcher::center_to_top_left(QWidget * w, QPoint p) {
	return {
		p.x() - w->width() / 2,
		p.y() - w->height()
	};
}