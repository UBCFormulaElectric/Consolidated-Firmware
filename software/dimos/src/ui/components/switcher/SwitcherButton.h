#pragma once

#include <QPushButton>
#include <QLabel>

#include <QPalette>
#include <QPainter>
#include <QTime>

enum SwitcherButtonOption {
	ENDURANCE,
	ACCELERATION,
	SKIDPAD,
	AUTOCROSS,
	BRAKING,
	SwitcherButtonOptionSize
};

class SwitcherButton : public QPushButton {
Q_OBJECT
public:
	explicit SwitcherButton(SwitcherButtonOption event, QWidget *parent = nullptr);
	void setupUI(SwitcherButtonOption event);
protected:
	void paintEvent(QPaintEvent *event) override;
private:
	inline static std::map<SwitcherButtonOption, QString> optionToImagePath = {
			{ENDURANCE, ":/SwitcherEventIcons/Endurance.svg"},
			{ACCELERATION, ":/SwitcherEventIcons/Acceleration.svg"},
			{SKIDPAD, ":/SwitcherEventIcons/Skidpad.svg"},
			{AUTOCROSS, ":/SwitcherEventIcons/Autocross.svg"},
			{BRAKING, ":/SwitcherEventIcons/Brake.svg"}
	};

	std::unique_ptr<QLabel> iconLabel;
};
