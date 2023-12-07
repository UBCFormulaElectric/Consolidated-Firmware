#pragma once

#include <QGraphicsBlurEffect>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>
#include <QPainter>
#include <QIcon>
#include "components/squircle/squircle.h"

QT_BEGIN_NAMESPACE

namespace ui
{
class NotifFrame final : public QFrame
{
    const static inline auto backgroundBrush = QBrush(QColor(255, 255, 255, 255 * 0.1));
    QLinearGradient          borderGradient;
    QPen                     pen;

  public:
    explicit NotifFrame(QWidget *parent = nullptr) : QFrame(parent)
    {
        // setStyleSheet("background-color: rgba(128, 128, 128, 0.3); border-radius: 20px;");
        borderGradient = QLinearGradient(10, 0, 300, 500);
        borderGradient.setColorAt(0, QColor(255, 255, 255, 255 * 0.4));
        borderGradient.setColorAt(0.45, QColor(255, 255, 255, 0));
        borderGradient.setColorAt(0.55, QColor(255, 255, 255, 0));
        borderGradient.setColorAt(1, QColor(255, 255, 255, 255 * 0.1));
        pen = QPen(borderGradient, 2);
    }

    void paintEvent(QPaintEvent *event) override
    {
        QPainter p(this);
        // p.setBrush(backgroundBrush);
        // p.setPen(pen);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(QPen(QColorConstants::Red, 3));
        Squircle::paint(&p, this->rect(), 50, 0.6);
        QFrame::paintEvent(event);
    }
};

class RacingBase
{
  public:
    QFrame *    notifParentFrame;
    QLabel *    notifBackgroundLogo;
    NotifFrame *notifFrame;

    QGraphicsBlurEffect *notifLogoEffect;

    QFrame *     innerElements;
    QGridLayout *splitLayout;

    void setupUi(QWidget *parent, QFrame *innerElems)
    {
        notifParentFrame = new QLabel();
        notifParentFrame->setObjectName("Notif Parent Frame");
        innerElements = innerElems;

        splitLayout = new QGridLayout(parent);
        splitLayout->setContentsMargins(0, 0, 0, 0);
        splitLayout->setHorizontalSpacing(0);
        splitLayout->setVerticalSpacing(0);
        splitLayout->addWidget(notifParentFrame, 0, 0, 1, 3);
        splitLayout->addWidget(innerElements, 0, 3, 1, 2);

        notifBackgroundLogo = new QLabel(notifParentFrame);
        notifBackgroundLogo->setObjectName("Notif Background Logo");
        notifBackgroundLogo->setAlignment(Qt::AlignCenter);
        notifBackgroundLogo->setPixmap(QIcon(":/fe_logo.svg").pixmap(QSize(350, 400)));

        notifLogoEffect = new QGraphicsBlurEffect;
        notifLogoEffect->setBlurRadius(60);
        notifLogoEffect->setBlurHints(QGraphicsBlurEffect::QualityHint);
        notifBackgroundLogo->setGraphicsEffect(notifLogoEffect);

        notifFrame = new NotifFrame(notifParentFrame);
    }

    void resizeEvent(QResizeEvent *) const
    {
        constexpr int MARGIN       = 10;
        const int     parentHeight = notifParentFrame->height(), parentWidth = notifParentFrame->width();
        notifFrame->setGeometry(MARGIN, MARGIN, parentWidth - 2 * MARGIN, parentHeight - 2 * MARGIN);
        notifBackgroundLogo->setGeometry(notifParentFrame->geometry());
    }
};
} // namespace ui

QT_END_NAMESPACE