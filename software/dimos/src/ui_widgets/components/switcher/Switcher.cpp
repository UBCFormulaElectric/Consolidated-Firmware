#include "Switcher.h"
#include "constants.h"

Switcher::Switcher(QWidget *parent) : QFrame(parent), selectedEvent(SwitcherButton::ENDURANCE)
{
    setupUI();
}

void Switcher::toggleRight()
{
    selectedEvent = static_cast<SwitcherButton::SwitcherButtonOption>(
        (selectedEvent + 1) % SwitcherButton::SwitcherButtonOptionSize);
    repositionSwitcherButtons();
}

void Switcher::toggleLeft()
{
    selectedEvent = static_cast<SwitcherButton::SwitcherButtonOption>(
        (selectedEvent - 1 + SwitcherButton::SwitcherButtonOptionSize) % SwitcherButton::SwitcherButtonOptionSize);
    repositionSwitcherButtons();
}

void Switcher::setupUI()
{
    this->resize(SCREEN_WIDTH, SCREEN_HEIGHT);
    this->setObjectName("Switcher");

    enduranceOption    = std::make_unique<SwitcherButton>(SwitcherButton::ENDURANCE, this);
    accelerationOption = std::make_unique<SwitcherButton>(SwitcherButton::ACCELERATION, this);
    skidpadOption      = std::make_unique<SwitcherButton>(SwitcherButton::SKIDPAD, this);
    autoCrossOption    = std::make_unique<SwitcherButton>(SwitcherButton::AUTOCROSS, this);
    brakingOption      = std::make_unique<SwitcherButton>(SwitcherButton::BRAKING, this);
    lowvoltageOption   = std::make_unique<SwitcherButton>(SwitcherButton::LOWVOLTAGE, this);

    allButtons = { enduranceOption.get(), accelerationOption.get(), skidpadOption.get(),
                   autoCrossOption.get(), brakingOption.get(),      lowvoltageOption.get() };

    activeButtons = allButtons; // temporarily

    selectedEventLabel = std::make_unique<QLabel>(this);
    selectedEventLabel->setStyleSheet("QLabel {"
                                      "	color: white;"
                                      "	font-size: 20px;"
                                      "	font-weight: bold;"
                                      "}");
    selectedEventLabel->setObjectName("Switcher_EventLabel");

    repositionSwitcherButtons();
}

constexpr int SCREEN_CENTER_X = SCREEN_WIDTH / 2, SCREEN_BOTTOM_Y = SCREEN_HEIGHT;
/**
 * \brief Repositions the buttons
 * TODO consider making this part of the update event
 */
void Switcher::repositionSwitcherButtons()
{
    for (const auto &button : allButtons)
    {
        button->hide();
    }
    for (int ii = -2; ii <= 2; ii++)
    {
        const uint   objIndex  = (selectedEvent + ii + activeButtons.size()) % activeButtons.size();
        const auto   button    = activeButtons[objIndex];
        const QPoint centerLoc = place_circle_from_index(ii), loc = c_2_tl(enduranceOption->rect(), centerLoc);
        button->move(loc);
        button->show();
    }

    // adjust label
    selectedEventLabel->setText(SwitcherButton::switcherOptionToName[selectedEvent]);
    selectedEventLabel->adjustSize();
    selectedEventLabel->move(
        SCREEN_CENTER_X - selectedEventLabel->width() / 2, SCREEN_BOTTOM_Y - selectedEventLabel->height() - 10);
}

constexpr int    R = 955, BUTTON_GAP = 15, BUTTON_WIDTH = 200;
constexpr double VERTICAL_CORRECTION = 50, OVAL_COEFFICIENT = 1.3;
/**
 * Places a button at a certain angle
 * @param ii The index of the button
 * @return The center point of the button
 */
QPoint Switcher::place_circle_from_index(const int ii)
{
    const int uncorrected_x = ii * (BUTTON_WIDTH + BUTTON_GAP);
    return { SCREEN_CENTER_X + uncorrected_x,
             SCREEN_BOTTOM_Y - static_cast<int>(
                                   std::round(sqrt(pow(R, 2) - pow(uncorrected_x, 2) / pow(OVAL_COEFFICIENT, 2))) -
                                   (R - VERTICAL_CORRECTION)) };
}

/**
 * Converts a center point to a top left point
 *
 * @param dims The dimensions of the object
 * @param p The point to convert
 * @return The top left point
 */
QPoint Switcher::c_2_tl(const QRect dims, const QPoint p)
{
    return { p.x() - dims.width() / 2, p.y() - dims.height() };
}