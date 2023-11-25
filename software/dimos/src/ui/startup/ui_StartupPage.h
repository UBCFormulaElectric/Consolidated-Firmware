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


namespace ui {
    class StartupPage {
        // TODO Maybe format into data structure?
        std::vector<DriverProfile> driverProfiles = {
            {"ZEDWIN", ":/Zheng_Edwin.png", 400, 400}
        };
        int selectedDriverProfile{};

        [[nodiscard]] size_t nextDriverProfile() const {
            return (selectedDriverProfile + 1) % driverProfiles.size();
        }

        [[nodiscard]] size_t prevDriverProfile() const {
            return (selectedDriverProfile - 1 + driverProfiles.size()) % driverProfiles.size();
        }

        QLabel * DriverPhoto{};
        QLabel * DriverName{};
    public:
        void setupUi(QWidget *startup) {
            if (startup->objectName().isEmpty())
                startup->setObjectName("startup");
            startup->resize(1024, 600);

            auto gridLayout = new QGridLayout(startup);
            gridLayout->setAlignment(Qt::AlignCenter);

            selectedDriverProfile = 0;

		    DriverPhoto = new QLabel(startup);
            DriverPhoto->setObjectName("Driver Photo");

            const DriverProfile* selectedDriver = &driverProfiles[selectedDriverProfile];

            const QPixmap image(selectedDriver->imagePath);
            DriverPhoto->setPixmap(image);
            gridLayout->addWidget(DriverPhoto);

            DriverName = new QLabel(startup);
            DriverName->setText(selectedDriver->name);
            gridLayout->addWidget(DriverName);


            startup->setLayout(gridLayout);

            QMetaObject::connectSlotsByName(startup);
        }
    };
}

QT_END_NAMESPACE
