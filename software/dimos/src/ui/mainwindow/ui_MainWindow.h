#pragma once

#include <QStackedWidget>
#include <QtWidgets/QMainWindow>
#include <QGraphicsBlurEffect>
#include <iostream>

//pages
#include "landing/LandingPage.h"
#include "startup/StartupPage.h"
//components
#include "components/switcher/Switcher.h"
#include "endurance/EndurancePage.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
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
    std::unique_ptr<QStackedWidget> MainStack;
	std::unique_ptr<LandingPage> landingPage;
	std::unique_ptr<StartupPage> startupPage;
	std::unique_ptr<EndurancePage> endurance_page;
	// std::unique_ptr<AccelerationPage> acceleration_page;

	// switcher logic
    std::unique_ptr<Switcher> SwitcherFrame;
	std::unique_ptr<QGraphicsBlurEffect> switcherBackgroundEffect;
	bool isSwitcherOpen = false;

	static inline std::map<Frames, int> frameToMainstackIndex = {
		{LandingFrame, 0},
		{LVFrame, 1},
		{EnduranceFrame, 2},
		{AcclerationFrame, 3},
		{SkidpadFrame, 4},
		{AutocrossFrame, 5},
		{BrakeFrame, 6},
	};
	static inline std::map<SwitcherButtonOption, Frames> switcherOptionToFrame = {
		{ENDURANCE, EnduranceFrame},
		{ACCELERATION, AcclerationFrame},
		{SKIDPAD, SkidpadFrame},
		{AUTOCROSS, AutocrossFrame},
		{BRAKING, BrakeFrame},
	};

public:
    void setupUi(QMainWindow *MainWindow)
    {
		// MainWindow Setup
        if (MainWindow->objectName().isEmpty()) MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1024, 600);
        MainWindow->setMinimumSize(QSize(1024, 600));
		MainWindow->setStyleSheet("QMainWindow {background-color: #141414;}");
		#if USING_dimos_dev
		MainWindow->setWindowTitle("Dimos Development");
		#elif USING_dimos
		MainWindow->setWindowTitle("Dimos");
		#endif

		// mainframe
		MainStack = std::make_unique<QStackedWidget>(MainWindow);
        MainStack->setObjectName("MainFrame");
        MainStack->setGeometry(QRect(0, 0, 1024, 600));

		//populating mainframe
		landingPage = std::make_unique<LandingPage>();
		startupPage = std::make_unique<StartupPage>();
		MainStack->addWidget(landingPage.get());
		MainStack->addWidget(startupPage.get());

    	endurance_page = std::make_unique<EndurancePage>();
    	MainStack->addWidget(endurance_page.get());

		//context frame
		SwitcherFrame = std::make_unique<Switcher>(MainWindow);
		SwitcherFrame->hide();

		switcherBackgroundEffect = std::make_unique<QGraphicsBlurEffect>();
		switcherBackgroundEffect->setBlurRadius(30);
		switcherBackgroundEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
		MainStack->setGraphicsEffect(switcherBackgroundEffect.get());
		switcherBackgroundEffect->setEnabled(false);

        QMetaObject::connectSlotsByName(MainWindow);
    }

	void toggleFrame(Frames toFrame) const {
		const int nextIdx = frameToMainstackIndex[toFrame];
    	if(nextIdx >= MainStack->count()) {
    		std::cerr << "ERROR: Invalid frame index " << nextIdx << " with last index " << MainStack->count() - 1 << std::endl;
    		return;
    	}

    	std::cout << "Switching to frame " << nextIdx << std::endl;
        MainStack->setCurrentIndex(nextIdx);
    	std::cout << MainStack->currentIndex() << std::endl;
	}

	void setSwitcherSelectionToFrame() const {
    	const auto a = SwitcherFrame->getSelectedOption();
    	const auto b = switcherOptionToFrame[a];
    	toggleFrame(b);
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
    class MainWindow: public Ui_MainWindow {};
}

QT_END_NAMESPACE