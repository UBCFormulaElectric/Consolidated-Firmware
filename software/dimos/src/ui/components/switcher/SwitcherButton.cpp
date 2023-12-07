#include <QGridLayout>
#include "SwitcherButton.h"

#include "components/squircle/squircle.h"

SwitcherButton::SwitcherButton(SwitcherButtonOption event, QWidget* parent) : QWidget(parent)
{
    setupUi(event);
}

void SwitcherButton::setupUi(SwitcherButtonOption event)
{
    this->setFixedSize(QSize(200, 200));
    this->setContentsMargins(0, 0, 0, 0);
    this->setObjectName("SwitcherButton_" + switcherOptionToName[event]);

    const auto gridLayout = new QGridLayout(this);
    gridLayout->setObjectName("SwitcherButton_" + switcherOptionToName[event] + "_gridLayout");
    iconLabel = new QLabel();
    iconLabel->setObjectName("SwitcherButton_" + switcherOptionToName[event] + "_iconLabel");
    const auto icon = QIcon(optionToImagePath[event]);
    const auto iconPixmap = icon.pixmap(QSize(140, 130));
    iconLabel->setPixmap(iconPixmap);
    gridLayout->addWidget(iconLabel, 0, 0, Qt::AlignCenter);
    this->setLayout(gridLayout);
}

void SwitcherButton::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setBrush(backgroundBrush);
    p.setPen(QColorConstants::Transparent);
    p.setRenderHint(QPainter::Antialiasing);
    // Squircle::paint(&p, this->rect(), 20, 0.6);
    QWidget::paintEvent(event);
}
