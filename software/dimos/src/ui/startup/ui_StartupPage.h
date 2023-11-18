#pragma once

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class ui_StartupPage
{
public:
    QLabel *BootLogoLabel;

    void f(std::array<int, 4> a);

    void setupUi(QWidget *landing)
    {
        if (landing->objectName().isEmpty())
            landing->setObjectName("landing");
        landing->resize(1024, 600);

        auto gridLayout = new QGridLayout(landing);
        gridLayout->setAlignment(Qt::AlignCenter);

        BootLogoLabel = new QLabel(landing);
        BootLogoLabel->setObjectName("BootLogoLabel");
        gridLayout->addWidget(BootLogoLabel);

        auto pixmap = QIcon(":/assets/fe_logo.svg").pixmap(QSize(375, 375), Qt::KeepAspectRatio);
        BootLogoLabel->setPixmap(pixmap);
//		auto svg = QSvgWidget(":/assets/fe_logo.svg");


        landing->setLayout(gridLayout);

        QMetaObject::connectSlotsByName(landing);
    }
};

namespace ui {
    class StartupPage: public ui_StartupPage {};
}

QT_END_NAMESPACE
