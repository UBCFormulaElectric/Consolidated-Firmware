#pragma once

#include <QFrame>
#include <QLabel>

#include "SwitcherButton.h"

class Switcher: public QFrame {
Q_OBJECT
public:
	explicit Switcher(QWidget *parent = nullptr);
	void setupUI();
	SwitcherButtonEvent getSelectedEvent() {
		return selectedEvent;
	}

private:
	SwitcherButtonEvent selectedEvent = ENDURANCE;
	std::unique_ptr<SwitcherButton> enduranceOption, accelerationOption, skidpadOption, autoCrossOption, brakingOption;
	std::unique_ptr<QLabel> selectedEventText;

	static QPoint place_center(double angle_deg);
	static QPoint center_to_top_left(QWidget * w, QPoint p);
};