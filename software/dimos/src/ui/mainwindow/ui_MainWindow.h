#pragma once

#include <QStackedWidget>
#include <QtWidgets/QMainWindow>
#include <QGraphicsBlurEffect>


#include "landing/LandingPage.h"
#include "startup/StartupPage.h"
#include "components/switcher/Switcher.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    std::unique_ptr<QStackedWidget> MainStack;
	std::unique_ptr<LandingPage> landingPage;
	std::unique_ptr<StartupPage> startupPage;

    std::unique_ptr<Switcher> SwitcherFrame;
	std::unique_ptr<QGraphicsBlurEffect> switcherBackgroundEffect;
	bool isSwitcherOpen = false;

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

    // Frames should only be switching when indicated by switcher
	// TODO toggle properly LMAO
	void toggleFrame() const {
		const int nextIdx = (MainStack->currentIndex() + 1) % MainStack->count();
        MainStack->setCurrentIndex(nextIdx);
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