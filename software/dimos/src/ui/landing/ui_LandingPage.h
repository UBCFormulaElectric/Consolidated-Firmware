#pragma once

#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include <QSvgRenderer>

#include "constants.h"

QT_BEGIN_NAMESPACE

namespace ui {
    class LandingPage {
    public:
        QLabel *BootLogoLabel;

        void setupUi(QFrame *landing)
        {
            if (landing->objectName().isEmpty()) landing->setObjectName("Landing_Page");
            landing->resize(SCREEN_WIDTH, SCREEN_HEIGHT);
            const auto gridLayout = new QBoxLayout(QBoxLayout::TopToBottom, landing);
            gridLayout->setAlignment(Qt::AlignCenter);

            BootLogoLabel = new QLabel(landing);
            BootLogoLabel->setObjectName("BootLogoLabel");
            const auto pixmap = QIcon(":/fe_logo.svg").pixmap(QSize(350, 400));
            BootLogoLabel->setPixmap(pixmap);

            gridLayout->addWidget(BootLogoLabel);

            landing->setLayout(gridLayout);
            QMetaObject::connectSlotsByName(landing);
        }
    };
}

QT_END_NAMESPACE
