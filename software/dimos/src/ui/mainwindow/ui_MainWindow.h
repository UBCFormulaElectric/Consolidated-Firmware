#pragma once

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMainWindow>
#include <iostream>
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

    std::unique_ptr<QFrame> SwitcherFrame;
	std::unique_ptr<QGraphicsBlurEffect> switcherBackgroundEffect;
	bool isSwitcherOpen = false;

    void setupUi(QMainWindow *MainWindow)
    {
		// MainWindow Setup
        if (MainWindow->objectName().isEmpty()) MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1024, 600);
        MainWindow->setMinimumSize(QSize(1024, 600));
		MainWindow->setStyleSheet("QMainWindow {background-color: #141414;}");

		// mainframe
		MainStack = std::make_unique<QStackedWidget>(MainWindow);
        MainStack->setObjectName("MainFrame");
        MainStack->setGeometry(QRect(0, 0, 1024, 600));

		//populating mainframe
		landingPage = std::make_unique<LandingPage>();
		startupPage = std::make_unique<StartupPage>();
		MainStack->addWidget(landingPage.get());
		MainStack->addWidget(startupPage.get());
		std::cout << MainStack->currentIndex() << std::endl;

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
};

namespace ui {
    class MainWindow: public Ui_MainWindow {};
}

QT_END_NAMESPACE