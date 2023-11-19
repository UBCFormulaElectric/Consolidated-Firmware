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
    void setupUi(QWidget *parent)
    {
        if (parent->objectName().isEmpty())
            parent->setObjectName("landing");
        parent->resize(1024, 600);

		auto label = new QLabel(parent);
		label->setObjectName("label");
		label->setText("Hello World!");

        QMetaObject::connectSlotsByName(parent);
    }
};

namespace ui {
    class StartupPage: public ui_StartupPage {};
}

QT_END_NAMESPACE
