#pragma once

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>
#include "components/squircle/Squircle.h"

QT_BEGIN_NAMESPACE

struct DriverProfile
{
    QString name;
    QString imagePath;
    int     height;
    int     width;
};

namespace ui
{
class StartupPage
{
    // TODO Maybe format into data structure?
    std::vector<DriverProfile> driverProfiles = { { "ZEDWIN", ":/Zheng_Edwin.png", 400, 400 } };
    int                        selectedDriverProfile{};

    [[nodiscard]] size_t nextDriverProfile() const { return (selectedDriverProfile + 1) % driverProfiles.size(); }

    [[nodiscard]] size_t prevDriverProfile() const
    {
        return (selectedDriverProfile - 1 + driverProfiles.size()) % driverProfiles.size();
    }

    double lowVoltagePercent; // 0 - 100

    QFrame *DriverPhotoFrame{};
    QFrame *LogoFrame{};
    QFrame *LowVoltageFrame{};
    QFrame *ShutdownLoopFrame{};

    QLabel * DriverPhoto{};
    QLabel * DriverName{};
    QLabel * LogoPhoto{};
    QLabel * LogoName{};
    QWidget *LowVoltageIcon{};
    QLabel * LowVoltageName{};
    QWidget *ShutdownLoopIcon{};
    QLabel * ShutdownLoopName{};

    const int PADDING = 50;
    const int MARGIN  = 25;

  public:
    // Outer setup
    void setupUi(QWidget *startup)
    {
        if (startup->objectName().isEmpty())
            startup->setObjectName("startup");
        startup->resize(1024, 600);

        auto gridLayout = new QGridLayout(startup);
        gridLayout->setAlignment(Qt::AlignCenter);
        selectedDriverProfile = 0;

        DriverPhotoFrame  = new QFrame();
        LogoFrame         = new QFrame();
        LowVoltageFrame   = new QFrame();
        ShutdownLoopFrame = new QFrame();

        initDriverPhotoFrame();
        initLogoFrame();
        initLowVoltageFrame();
        initShutdownLoopFrame();

        gridLayout->addWidget(DriverPhotoFrame, 0, 0, 1, 2);
        gridLayout->addWidget(LogoFrame, 1, 0, 1, 1);
        gridLayout->addWidget(LowVoltageFrame, 1, 1, 1, 1);
        gridLayout->addWidget(ShutdownLoopFrame, 0, 3, 2, 2);

        // gridLayout->setSpacing(MARGIN);
        gridLayout->setContentsMargins(PADDING, PADDING, PADDING, PADDING);

        startup->setLayout(gridLayout);
        QMetaObject::connectSlotsByName(startup);
    }

    // Initialize visual components for driver photo
    void initDriverPhotoFrame()
    {
        auto layout = new QBoxLayout(QBoxLayout::TopToBottom, DriverPhotoFrame);
        layout->setAlignment(Qt::AlignCenter);

        DriverPhoto = new QLabel(DriverPhotoFrame);
        DriverName  = new QLabel(DriverPhotoFrame);

        DriverPhoto->setStyleSheet("border-radius: 20px;");

        const DriverProfile *selectedDriver = &driverProfiles[selectedDriverProfile];
        auto                 driverImage    = QPixmap(selectedDriver->imagePath);

        DriverPhoto->setPixmap(driverImage);
        DriverName->setText(selectedDriver->name);

        layout->addWidget(DriverPhoto);
        layout->addWidget(DriverName);
        DriverPhotoFrame->setLayout(layout);
    }

    // Initialize visual components for Logo
    void initLogoFrame()
    {
        auto layout = new QBoxLayout(QBoxLayout::TopToBottom, LogoFrame);
        layout->setAlignment(Qt::AlignCenter);

        LogoPhoto = new QLabel();
        // LogoName = new QLabel();
        auto logoImage = QIcon(":/fe_logo.svg").pixmap(QSize(140, 160));

        LogoFrame->setStyleSheet("border-radius: 20px; background-color: #E4E4E4;");
        LogoFrame->setFixedSize(180, 180);
        LogoPhoto->setPixmap(logoImage);

        // LogoName->setText("UBC Formula Electric");
        layout->addWidget(LogoPhoto);
        // layout->addWidget(LogoName);
        LogoFrame->setLayout(layout);
    }

    // Initialize visual components for Low Voltage
    void initLowVoltageFrame()
    {
        // Trying to round corners?
        LowVoltageFrame->setStyleSheet("border-radius: 20px; background-color: #013372;");
        LowVoltageFrame->setFixedSize(180, 180);

        auto layout = new QBoxLayout(QBoxLayout::TopToBottom, LowVoltageFrame);
        layout->setAlignment(Qt::AlignCenter);

        LowVoltageName = new QLabel(LowVoltageFrame);
        LowVoltageName->setText("LV");

        layout->addWidget(LowVoltageName);
        LowVoltageFrame->setLayout(layout);
    }

    // Initialize visual components for Shutdown Loop
    void initShutdownLoopFrame()
    {
        auto layout = new QBoxLayout(QBoxLayout::TopToBottom, ShutdownLoopFrame);
        layout->setAlignment(Qt::AlignCenter);

        ShutdownLoopName = new QLabel(ShutdownLoopFrame);
        ShutdownLoopName->setText("Imagine there was a shutdown loop here");

        layout->addWidget(ShutdownLoopName);
        ShutdownLoopFrame->setLayout(layout);
    }
};
} // namespace ui

QT_END_NAMESPACE
