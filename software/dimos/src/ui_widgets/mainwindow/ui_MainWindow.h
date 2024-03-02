#pragma once

#include <QStackedWidget>
#include <QtWidgets/QMainWindow>

#include "constants.h"

// pages
#include "landing/LandingPage.h"
#include "startup/StartupPage.h"
// components
#include "components/switcher/Switcher.h"
// racing pages
#include "racing/acceleration/AccelerationPage.h"
#include "racing/autocross/AutocrossPage.h"
#include "racing/braking/BrakingPage.h"
#include "racing/endurance/EndurancePage.h"
#include "racing/skidpad/SkidpadPage.h"

QT_BEGIN_NAMESPACE

class ui_MainWindow
{
  public:
    // Frames should only be switching when indicated by switcher
    enum Frames
    {
        LandingFrame,
        EnduranceFrame,
        AcclerationFrame,
        SkidpadFrame,
        AutocrossFrame,
        BrakeFrame,
        LVFrame,
        FramesCount
    }; // order does matter :)
  private:
    QWidget        *centerWidget{};
    QVBoxLayout    *centerLayout{};
    QWidget        *limiter{};
    QStackedWidget *MainStack{};

    // pages
    LandingPage *landing_page{};
    StartupPage *startup_page{};

    // racing pages
    AccelerationPage *acceleration_page{};
    AutocrossPage    *autocross_page{};
    BrakingPage      *braking_page{};
    EndurancePage    *endurance_page{};
    SkidpadPage      *skidpad_page{};

    // switcher logic
    Switcher                            *SwitcherFrame{};
    std::unique_ptr<QGraphicsBlurEffect> switcherBackgroundEffect;
    bool                                 isSwitcherOpen = false;

    static inline std::map<SwitcherButton::SwitcherButtonOption, Frames> switcherOptionToFrame = {
        { SwitcherButton::ENDURANCE, EnduranceFrame }, { SwitcherButton::ACCELERATION, AcclerationFrame },
        { SwitcherButton::SKIDPAD, SkidpadFrame },     { SwitcherButton::AUTOCROSS, AutocrossFrame },
        { SwitcherButton::BRAKING, BrakeFrame },       { SwitcherButton::LOWVOLTAGE, LVFrame }
    };

  public:
    void setupUi(QMainWindow *MainWindow)
    {
        // MainWindow Setup
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->setMinimumSize(SCREEN_WIDTH, SCREEN_HEIGHT);
        MainWindow->setStyleSheet("QMainWindow {"
                                  "	background-color: #002D67;"
                                  "}"
                                  "QLabel {"
                                  "	color:white;"
                                  "}");
#if USING_dimos_dev
        MainWindow->setWindowTitle("Dimos Development");
#elif USING_dimos
        MainWindow->setWindowTitle("Dimos");
#endif
        centerWidget = new QWidget(MainWindow);
        centerWidget->setObjectName("centerWidget");
        MainWindow->setCentralWidget(centerWidget);

        centerLayout = new QVBoxLayout(centerWidget);
        centerLayout->setObjectName("centerLayout");
        centerLayout->setAlignment(Qt::AlignCenter);
        centerLayout->setContentsMargins(0, 0, 0, 0);

        limiter = new QWidget();
        limiter->setContentsMargins(0, 0, 0, 0);
        limiter->setFixedSize(SCREEN_WIDTH, SCREEN_HEIGHT);
        limiter->setObjectName("limiter");
        limiter->setStyleSheet("QWidget#limiter {"
                               "  background-color: #141414;"
                               "}");
        centerLayout->addWidget(limiter);
        centerWidget->setLayout(centerLayout);

        // mainframe
        MainStack = new QStackedWidget(limiter);
        MainStack->setObjectName("MainStack");
        MainStack->setGeometry(QRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));

        // populating mainframe
        landing_page = new LandingPage();
        // apparently it does not like it when you add it in the constructor
        MainStack->addWidget(landing_page);

        asyncSetup();

        QMetaObject::connectSlotsByName(MainWindow);
    }

    void asyncSetup()
    {
        // context frame
        SwitcherFrame = new Switcher(limiter);
        SwitcherFrame->hide();

        switcherBackgroundEffect = std::make_unique<QGraphicsBlurEffect>();
        switcherBackgroundEffect->setBlurRadius(30);
        switcherBackgroundEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
        MainStack->setGraphicsEffect(switcherBackgroundEffect.get());
        switcherBackgroundEffect->setEnabled(false);

        acceleration_page = new AccelerationPage();
        autocross_page    = new AutocrossPage();
        braking_page      = new BrakingPage();
        endurance_page    = new EndurancePage();
        skidpad_page      = new SkidpadPage();
        startup_page      = new StartupPage();

        MainStack->addWidget(endurance_page);
        MainStack->addWidget(acceleration_page);
        MainStack->addWidget(skidpad_page);
        MainStack->addWidget(autocross_page);
        MainStack->addWidget(braking_page);
        MainStack->addWidget(startup_page);
    }

    void toggleFrame(const Frames toFrame) const
    {
        const int nextIdx = toFrame;
        if (nextIdx >= MainStack->count())
        {
            qWarning() << "ERROR: Invalid frame index" << nextIdx << "with last index " << MainStack->count() - 1;
            return;
        }
        MainStack->setCurrentIndex(nextIdx);
    }

    void setSwitcherSelectionToFrame() const
    {
        const auto switcher_option = SwitcherFrame->getSelectedOption();
        const auto frame_id        = switcherOptionToFrame[switcher_option];
        toggleFrame(frame_id);
    }

    void toggleSwitcher()
    {
        if (isSwitcherOpen)
        {
            SwitcherFrame->hide();
            switcherBackgroundEffect->setEnabled(false);
        }
        else
        {
            SwitcherFrame->show();
            switcherBackgroundEffect->setEnabled(true);
        }
        isSwitcherOpen = !isSwitcherOpen;
    }

    void forceCloseSwitcher()
    {
        SwitcherFrame->hide();
        switcherBackgroundEffect->setEnabled(false);
        isSwitcherOpen = false;
    }

    void switcherRight() const
    {
        if (!isSwitcherOpen)
            return;
        SwitcherFrame->toggleRight();
    }
    void switcherLeft() const
    {
        if (!isSwitcherOpen)
            return;
        SwitcherFrame->toggleLeft();
    }

    [[nodiscard]] bool getSwitcherOpen() const { return isSwitcherOpen; }
};

namespace ui
{
class MainWindow : public ui_MainWindow
{
};
} // namespace ui

QT_END_NAMESPACE