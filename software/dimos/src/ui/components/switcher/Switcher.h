#pragma once

#include <QFrame>
#include <QLabel>

#include <vector>

#include "SwitcherButton.h"

class Switcher: public QFrame {
Q_OBJECT
public:
	explicit Switcher(QWidget *parent = nullptr);
	void setupUI();
	SwitcherButtonOption getSelectedOption() {
		return selectedEvent;
	}
	void toggleRight();
	void toggleLeft();

private:
	SwitcherButtonOption selectedEvent;
	std::unique_ptr<SwitcherButton> enduranceOption, accelerationOption, skidpadOption, autoCrossOption, brakingOption;
	std::unique_ptr<QLabel> selectedEventLabel;

	std::vector<SwitcherButton *> activeButtons;

	inline static double BUTTON_ANGLE_OFFSETS[] = {0, 13, 26, -26, -13};

	inline static std::map<SwitcherButtonOption, QString> switcherOptionToName = {
			{ENDURANCE, "ENDURANCE"},
			{ACCELERATION, "ACCELERATION"},
			{SKIDPAD, "SKIDPAD"},
			{AUTOCROSS, "AUTOCROSS"},
			{BRAKING, "BRAKING"}
	};

	void repositionSwitcherButtons();

	QPoint place_circle_from_index(int ii);
	static QPoint c_2_tl(QRect dims, QPoint p);

};