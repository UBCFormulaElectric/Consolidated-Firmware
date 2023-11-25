#include "Switcher.h"

Switcher::Switcher(QWidget *parent): QFrame(parent) {
	setupUI();
}

void Switcher::toggleRight() {
	selectedEvent = static_cast<SwitcherButtonOption>((selectedEvent + 1) % SwitcherButtonOptionSize);
	repositionSwitcherButtons();
}

void Switcher::toggleLeft() {
	selectedEvent = static_cast<SwitcherButtonOption>((selectedEvent - 1 + SwitcherButtonOptionSize) % SwitcherButtonOptionSize);
	repositionSwitcherButtons();
}

void Switcher::setupUI() {
	this->resize(1024, 600);
	this->setMinimumSize(QSize(1024, 600));

	enduranceOption 	= std::make_unique<SwitcherButton>(ENDURANCE, this);
	accelerationOption 	= std::make_unique<SwitcherButton>(ACCELERATION,this);
	skidpadOption 		= std::make_unique<SwitcherButton>(SKIDPAD, 	this);
	autoCrossOption 	= std::make_unique<SwitcherButton>(AUTOCROSS, this);
	brakingOption 		= std::make_unique<SwitcherButton>(BRAKING, 	this);

	activeButtons = {
		enduranceOption.get(),
		accelerationOption.get(),
		skidpadOption.get(),
		autoCrossOption.get(),
		brakingOption.get()
	};

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


constexpr int SCREEN_CENTER_X = 1024 / 2, SCREEN_BOTTOM_Y = 600;
/**
 * \brief Repositions the buttons
 * TODO consider making this part of the update event
 */
void Switcher::repositionSwitcherButtons() {
	for(int ii = -2; ii <= 2; ii++) {
		const uint objIndex = (selectedEvent + ii + activeButtons.size()) % activeButtons.size();
		const auto button = activeButtons[objIndex];
		const QPoint centerLoc = place_circle_from_index(ii), loc = c_2_tl(enduranceOption->rect(), centerLoc);
		button->move(loc);
	}

	// adjust label
	selectedEventLabel->setText(switcherOptionToName[selectedEvent]);
	selectedEventLabel->adjustSize();
	selectedEventLabel->move(SCREEN_CENTER_X - selectedEventLabel->width() / 2,
							 SCREEN_BOTTOM_Y - selectedEventLabel->height() - 10);
}

constexpr int R = 955, BUTTON_GAP = 15, BUTTON_WIDTH = 200;
constexpr double VERTICAL_CORRECTION = 50, OVAL_COEFFICIENT = 1.3;
/**
 * Places a button at a certain angle
 * @param ii The index of the button
 * @return The center point of the button
 */
QPoint Switcher::place_circle_from_index(const int ii) {
	const int uncorrected_x = ii * (BUTTON_WIDTH + BUTTON_GAP);
	return  {
		SCREEN_CENTER_X + uncorrected_x,
		SCREEN_BOTTOM_Y -
			static_cast<int>(std::round(sqrt(pow(R,2) - pow(uncorrected_x,2)/pow(OVAL_COEFFICIENT,2)))
			- (R - VERTICAL_CORRECTION))
	};
}

/**
 * Converts a center point to a top left point
 *
 * @param dims The dimensions of the object
 * @param p The point to convert
 * @return The top left point
 */
QPoint Switcher::c_2_tl(const QRect dims, const QPoint p) {
	return {
		p.x() - dims.width() / 2,
		p.y() - dims.height()
	};
}