#pragma once

#include <QPushButton>
#include <QLabel>

#include <QPainter>
#include <QTime>

class SwitcherButton : public QPushButton {
Q_OBJECT
public:
	enum SwitcherButtonOption {
		ENDURANCE,
		ACCELERATION,
		SKIDPAD,
		AUTOCROSS,
		BRAKING,
        LOWVOLTAGE,
		SwitcherButtonOptionSize
	};

	inline static std::map<SwitcherButtonOption, QString> switcherOptionToName = {
		{ENDURANCE, "ENDURANCE"},
		{ACCELERATION, "ACCELERATION"},
		{SKIDPAD, "SKIDPAD"},
		{AUTOCROSS, "AUTOCROSS"},
		{BRAKING, "BRAKING"},
        {LOWVOLTAGE, "LOWVOLTAGE"}
	};


	explicit SwitcherButton(SwitcherButtonOption event, QWidget *parent = nullptr);
	void setupUi(SwitcherButtonOption event);
protected:
	void paintEvent(QPaintEvent *event) override;
private:
	inline static std::map<SwitcherButtonOption, QString> optionToImagePath = {
			{ENDURANCE, ":/SwitcherEventIcons/Endurance.svg"},
			{ACCELERATION, ":/SwitcherEventIcons/Acceleration.svg"},
			{SKIDPAD, ":/SwitcherEventIcons/Skidpad.svg"},
			{AUTOCROSS, ":/SwitcherEventIcons/Autocross.svg"},
			{BRAKING, ":/SwitcherEventIcons/Brake.svg"},
            {LOWVOLTAGE, ":/SwitcherEventIcons/LV.svg"}
	};

	QLabel * iconLabel;
};
