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

	enduranceOption 	= std::make_unique<SwitcherButton>(ENDURANCE, this);
	accelerationOption 	= std::make_unique<SwitcherButton>(ACCELERATION,this);
	skidpadOption 		= std::make_unique<SwitcherButton>(SKIDPAD, 	this);
	autoCrossOption 	= std::make_unique<SwitcherButton>(AUTOCROSS, this);
	brakingOption 		= std::make_unique<SwitcherButton>(BRAKING, 	this);

	selectedEvent = ENDURANCE;
	selectedEventLabel = std::make_unique<QLabel>(this);
	selectedEventLabel->setStyleSheet(
			"QLabel {"
			 "	color: white;"
			 "	font-size: 20px;"
			 "	font-weight: bold;"
			 "}");


	repositionSwitcherButtons();
}

void Switcher::toggleRight() {
	selectedEvent = (SwitcherButtonOption) ((selectedEvent + 1) % SwitcherButtonOptionSize);
	repositionSwitcherButtons();
}

void Switcher::toggleLeft() {
	selectedEvent = (SwitcherButtonOption) ((selectedEvent - 1 + SwitcherButtonOptionSize) % SwitcherButtonOptionSize);
	repositionSwitcherButtons();
}

void Switcher::repositionSwitcherButtons() {
	enduranceOption->move(c_2_tl(enduranceOption.get(),
								 place_center(BUTTON_ANGLE_OFFSETS[(selectedEvent - ENDURANCE  + SwitcherButtonOptionSize) % SwitcherButtonOptionSize])));
	accelerationOption->move(c_2_tl(accelerationOption.get(),
									place_center(BUTTON_ANGLE_OFFSETS[(selectedEvent - ACCELERATION + SwitcherButtonOptionSize) % SwitcherButtonOptionSize])));
	skidpadOption->move(c_2_tl(skidpadOption.get(),
							   place_center(BUTTON_ANGLE_OFFSETS[(selectedEvent - SKIDPAD + SwitcherButtonOptionSize) % SwitcherButtonOptionSize])));
	autoCrossOption->move(c_2_tl(autoCrossOption.get(),
								 place_center(BUTTON_ANGLE_OFFSETS[(selectedEvent - AUTOCROSS + SwitcherButtonOptionSize) % SwitcherButtonOptionSize])));
	brakingOption->move(c_2_tl(brakingOption.get(),
							   place_center(BUTTON_ANGLE_OFFSETS[(selectedEvent - BRAKING + SwitcherButtonOptionSize) % SwitcherButtonOptionSize])));

	selectedEventLabel->setText(switcherOptionToName[selectedEvent]);
	selectedEventLabel->adjustSize();
	selectedEventLabel->move(BUTTON_CENTER_X - selectedEventLabel->width() / 2,
							 BUTTON_BOTTOM_Y - selectedEventLabel->height() - 10);
}

/**
 * Places a button at a certain angle
 * @param angle_deg The angle in degrees
 * @return The center point of the button
 */
QPoint Switcher::place_center(double angle_deg) {
	return  {
		(int) round(BUTTON_CENTER_X + R * sin(angle_deg * M_PI / 180)),
		(int) round(BUTTON_BOTTOM_Y + R - R * cos(angle_deg * M_PI / 180) - OFFSET)
	};
}

/**
 * Converts a center point to a top left point
 * @param w The widget for which to convert it
 * @param p The point to convert
 * @return The top left point
 */
QPoint Switcher::c_2_tl(QWidget * w, QPoint p) {
	return {
		p.x() - w->width() / 2,
		p.y() - w->height()
	};
}