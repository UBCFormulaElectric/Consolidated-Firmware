#pragma once

#include <QStackedWidget>
#include <QtWidgets/QMainWindow>

#include "constants.h"

//pages
#include "landing/LandingPage.h"
#include "startup/StartupPage.h"
//components
#include "components/switcher/Switcher.h"
#include "racing/endurance/EndurancePage.h"

QT_BEGIN_NAMESPACE


class ui_MainWindow {
public:
    // Frames should only be switching when indicated by switcher
    enum Frames {
        LandingFrame,
        LVFrame,
        EnduranceFrame,
        AcclerationFrame,
        SkidpadFrame,
        AutocrossFrame,
        BrakeFrame,
        FramesCount
    };
private:
	QMainWindow * mw{};
    QStackedWidget* MainStack{};

	// pages
	static inline std::map<Frames, int> frameToMainstackIndex = {
		{LandingFrame, 0},
		{LVFrame, 1},
		{EnduranceFrame, 2},
		{AcclerationFrame, 3},
		{SkidpadFrame, 4},
		{AutocrossFrame, 5},
		{BrakeFrame, 6},
	};
	LandingPage* landingPage{};
	StartupPage* startupPage{};
	EndurancePage* endurance_page{};
	// AccelerationPage* acceleration_page;

	// switcher logic
    Switcher* SwitcherFrame{};
	std::unique_ptr<QGraphicsBlurEffect> switcherBackgroundEffect;
	bool isSwitcherOpen = false;
	static inline std::map<SwitcherButton::SwitcherButtonOption, Frames> switcherOptionToFrame = {
		{SwitcherButton::ENDURANCE, EnduranceFrame},
		{SwitcherButton::ACCELERATION, AcclerationFrame},
		{SwitcherButton::SKIDPAD, SkidpadFrame},
		{SwitcherButton::AUTOCROSS, AutocrossFrame},
		{SwitcherButton::BRAKING, BrakeFrame},
	};

public:
    void setupUi(QMainWindow *MainWindow)
    {
    	mw = MainWindow;
		// MainWindow Setup
        if (MainWindow->objectName().isEmpty()) MainWindow->setObjectName("MainWindow");
        MainWindow->resize(SCREEN_WIDTH,SCREEN_HEIGHT);
        MainWindow->setMinimumSize(SCREEN_WIDTH, SCREEN_HEIGHT);
		MainWindow->setStyleSheet("QMainWindow {"
									"	background-color: #141414;"
									"}"
									"QLabel {"
									"	color:white;"
									"}");
		#if USING_dimos_dev
		MainWindow->setWindowTitle("Dimos Development");
		#elif USING_dimos
		MainWindow->setWindowTitle("Dimos");
		#endif

		// mainframe
		MainStack = new QStackedWidget(MainWindow);
        MainStack->setObjectName("MainStack");
        MainStack->setGeometry(QRect(0, 0, 1024, 600));

		//populating mainframe
		landingPage = new LandingPage();
    	// apparently it does not like it when you add it in the constructor
    	MainStack->addWidget(landingPage);

    	asyncSetup();

        QMetaObject::connectSlotsByName(MainWindow);

    }

	void asyncSetup() {
    	//context frame
    	SwitcherFrame = new Switcher(mw);
    	SwitcherFrame->hide();

    	switcherBackgroundEffect = std::make_unique<QGraphicsBlurEffect>();
    	switcherBackgroundEffect->setBlurRadius(30);
    	switcherBackgroundEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
    	MainStack->setGraphicsEffect(switcherBackgroundEffect.get());
    	switcherBackgroundEffect->setEnabled(false);

    	startupPage = new StartupPage();
    	endurance_page = new EndurancePage();

    	MainStack->addWidget(startupPage);
    	MainStack->addWidget(endurance_page);
    }

	void toggleFrame(const Frames toFrame) const {
		const int nextIdx = frameToMainstackIndex[toFrame];
    	if(nextIdx >= MainStack->count()) {
    		qWarning() << "ERROR: Invalid frame index" << nextIdx << "with last index " << MainStack->count() - 1;
    		return;
    	}
        MainStack->setCurrentIndex(nextIdx);
	}

	void setSwitcherSelectionToFrame() const {
    	const auto switcher_option = SwitcherFrame->getSelectedOption();
    	const auto frame_id = switcherOptionToFrame[switcher_option];
    	toggleFrame(frame_id);
    }

	void toggleSwitcher() {
		if(isSwitcherOpen) {
			SwitcherFrame->hide();
			switcherBackgroundEffect->setEnabled(false);
		} else {
			SwitcherFrame->show();
			switcherBackgroundEffect->setEnabled(true);
		}
		isSwitcherOpen = !isSwitcherOpen;
	}

	void forceCloseSwitcher() {
		SwitcherFrame->hide();
		switcherBackgroundEffect->setEnabled(false);
		isSwitcherOpen = false;
	}

	void switcherRight() const {
		if(!isSwitcherOpen) return;
		SwitcherFrame->toggleRight();
	}
	void switcherLeft() const {
		if(!isSwitcherOpen) return;
		SwitcherFrame->toggleLeft();
	}

	[[nodiscard]] bool getSwitcherOpen() const {
		return isSwitcherOpen;
	}
};

namespace ui {
	class MainWindow: public ui_MainWindow{};
}

QT_END_NAMESPACE