#pragma once

#include <QPushButton>
#include <QLabel>

#include <QPainter>
#include <QTime>

class SwitcherButton : public QWidget
{
    Q_OBJECT
  public:
    enum class SwitcherButtonOption
    {
        ENDURANCE,
        ACCELERATION,
        SKIDPAD,
        AUTOCROSS,
        BRAKING,
        LOWVOLTAGE,
        SwitcherButtonOptionSize
    };

    inline static std::map<SwitcherButtonOption, QString> switcherOptionToName = {
        { ENDURANCE, "ENDURANCE" }, { ACCELERATION, "ACCELERATION" }, { SKIDPAD, "SKIDPAD" },
        { AUTOCROSS, "AUTOCROSS" }, { BRAKING, "BRAKING" },           { LOWVOLTAGE, "LOWVOLTAGE" }
    };

    explicit SwitcherButton(SwitcherButtonOption event, QWidget *parent = nullptr);
    void setupUi(SwitcherButtonOption event);

  protected:
    void paintEvent(QPaintEvent *event) override;

  private:
    const static inline auto backgroundBrush = QBrush(QColor(0, 45, 103));

    inline static std::map<SwitcherButtonOption, QString> optionToImagePath = {
        { ENDURANCE, ":/SwitcherEventIcons/Endurance.svg" }, { ACCELERATION, ":/SwitcherEventIcons/Acceleration.svg" },
        { SKIDPAD, ":/SwitcherEventIcons/Skidpad.svg" },     { AUTOCROSS, ":/SwitcherEventIcons/Autocross.svg" },
        { BRAKING, ":/SwitcherEventIcons/Brake.svg" },       { LOWVOLTAGE, ":/SwitcherEventIcons/LV.svg" }
    };

    QLabel *iconLabel{};
};
