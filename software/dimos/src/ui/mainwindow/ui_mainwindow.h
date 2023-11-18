#pragma once

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMainWindow>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QFrame *MainFrame;
    QFrame *ContextFrame;

    void setupUi(QMainWindow *MainWindow)
    {
		// MainWindow Setup
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(1024, 600);
        MainWindow->setMinimumSize(QSize(1024, 600));
		MainWindow->setStyleSheet("QMainWindow {background-color: #141414;}");

		// mainframe
		MainFrame = new QFrame(MainWindow);
        MainFrame->setObjectName("MainFrame");
        MainFrame->setGeometry(QRect(0, 0, 1024, 600));
        MainWindow->setCentralWidget(MainFrame);

		//context frame
        ContextFrame = new QFrame(MainWindow);
        ContextFrame->setObjectName("ContextFrame");
        ContextFrame->setGeometry(QRect(0, 0, 1024, 600));
        MainWindow->setCentralWidget(ContextFrame);

        QMetaObject::connectSlotsByName(MainWindow);
    }
};

namespace ui {
    class MainWindow: public Ui_MainWindow {};
}

QT_END_NAMESPACE