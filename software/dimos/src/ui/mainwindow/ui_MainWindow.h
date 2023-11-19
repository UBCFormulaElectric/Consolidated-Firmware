#pragma once

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMainWindow>
#include <iostream>
#include "landing/LandingPage.h"
#include "startup/StartupPage.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QStackedWidget *MainFrame;
    QFrame *ContextFrame;

	std::unique_ptr<LandingPage> landingPage;
	std::unique_ptr<StartupPage> startupPage;

    void setupUi(QMainWindow *MainWindow)
    {
		// MainWindow Setup
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1024, 600);
        MainWindow->setMinimumSize(QSize(1024, 600));
		MainWindow->setStyleSheet("QMainWindow {background-color: #141414;}");

		// mainframe
		MainFrame = new QStackedWidget(MainWindow);
        MainFrame->setObjectName("MainFrame");
        MainFrame->setGeometry(QRect(0, 0, 1024, 600));

		//populating mainframe
		landingPage = std::make_unique<LandingPage>();
		startupPage = std::make_unique<StartupPage>();
		MainFrame->addWidget(landingPage.get());
		MainFrame->addWidget(startupPage.get());
		MainFrame->setCurrentIndex(1);
		std::cout << MainFrame->currentIndex() << std::endl;

		//context frame
        ContextFrame = new QFrame(MainWindow);
        ContextFrame->setObjectName("ContextFrame");
        ContextFrame->setGeometry(QRect(0, 0, 1024, 600));

        QMetaObject::connectSlotsByName(MainWindow);
    }
};

namespace ui {
    class MainWindow: public Ui_MainWindow {};
}

QT_END_NAMESPACE