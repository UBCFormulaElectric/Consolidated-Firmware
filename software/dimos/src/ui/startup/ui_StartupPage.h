#pragma once

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

struct DriverProfile {
    QString name;
    QString imagePath;
    int height;
    int width;
};

class ui_StartupPage
{
public:
    // TODO Maybe format into data structure?
    DriverProfile driverOne = {"ZEDWIN", ":/Zheng_Edwin.png", 400, 400};

    QLabel *DriverPhoto;
    QLabel *DriverName;

    void setupUi(QWidget *startup)
    {
        if (startup->objectName().isEmpty())
            startup->setObjectName("startup");
        startup->resize(1024, 600);

        auto gridLayout = new QGridLayout(startup);
        gridLayout->setAlignment(Qt::AlignCenter);

		DriverPhoto = new QLabel(startup);
        DriverPhoto->setObjectName("Driver Photo");

        // TODO SETUP INPUT FOR SPECIFIC DRIVER
        QPixmap image(driverOne.imagePath);
        DriverPhoto->setPixmap(image);
        gridLayout->addWidget(DriverPhoto);

        DriverName = new QLabel(startup);
        DriverName->setText(driverOne.name);
        gridLayout->addWidget(DriverName);


        startup->setLayout(gridLayout);

        QMetaObject::connectSlotsByName(startup);
    }
};

namespace ui {
    class StartupPage: public ui_StartupPage {};
}

QT_END_NAMESPACE
