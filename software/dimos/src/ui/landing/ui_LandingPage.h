#pragma once

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

#include <QSvgRenderer>

QT_BEGIN_NAMESPACE

class ui_LandingPage
{
public:
    QLabel *BootLogoLabel;

    void setupUi(QWidget *landing)
    {
        if (landing->objectName().isEmpty())
            landing->setObjectName("landing");
        landing->resize(1024, 600);
		auto gridLayout = new QGridLayout(landing);
		gridLayout->setAlignment(Qt::AlignCenter);

        BootLogoLabel = new QLabel(landing);
        BootLogoLabel->setObjectName("BootLogoLabel");
		auto pixmap = QIcon(":/assets/fe_logo.svg").pixmap(QSize(350, 400));
		BootLogoLabel->setPixmap(pixmap);

		gridLayout->addWidget(BootLogoLabel);

		landing->setLayout(gridLayout);
		QMetaObject::connectSlotsByName(landing);
    }
};

namespace ui {
    class LandingPage: public ui_LandingPage {};
}

QT_END_NAMESPACE
