#pragma once

#include <QPushButton>
#include <QLabel>

enum SwitcherButtonEvent {
	ENDURANCE,
	ACCELERATION,
	SKIDPAD,
	AUTOCROSS,
	BRAKING
};

class SwitcherButton : public QPushButton {
Q_OBJECT
public:
	explicit SwitcherButton(SwitcherButtonEvent event, QWidget *parent = nullptr);
	void setupUI(SwitcherButtonEvent event);
private:
	inline static std::map<SwitcherButtonEvent, QString> eventToImagePath = {
			{ENDURANCE, ":/SwitcherEventIcons/Endurance.svg"},
			{ACCELERATION, ":/SwitcherEventIcons/Acceleration.svg"},
			{SKIDPAD, ":/SwitcherEventIcons/Skidpad.svg"},
			{AUTOCROSS, ":/SwitcherEventIcons/Autocross.svg"},
			{BRAKING, ":/SwitcherEventIcons/Brake.svg"}
	};

	std::unique_ptr<QLabel> iconLabel;
};
