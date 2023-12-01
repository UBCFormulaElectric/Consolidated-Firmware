#pragma once

#include <QGraphicsBlurEffect>
#include <QGridLayout>
#include <QLabel>
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace ui {

	class RacingBase {
	public:
		QFrame* notifParentFrame;
		QLabel* notifBackgroundLogo;
		QFrame * notifFrame;

		QGraphicsBlurEffect * notifLogoEffect;

		QFrame* innerElements;
		QGridLayout * splitLayout;

		void setupUi(QWidget * parent, QFrame * innerElems) {
			notifParentFrame = new QLabel();
			notifParentFrame->setObjectName("Notif Parent Frame");
			innerElements = innerElems;

			splitLayout = new QGridLayout(parent);
			splitLayout->setContentsMargins(0,0,0,0);
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


			notifFrame = new QFrame(notifParentFrame);
			notifFrame->setStyleSheet("background-color: rgba(128, 128, 128, 0.3); border-radius: 20px;");
		}

		void resizeEvent(QResizeEvent* event) const {
			constexpr int MARGIN = 10;
			const int parentHeight = notifParentFrame->height(), parentWidth = notifParentFrame->width();
			notifFrame->setGeometry(MARGIN, MARGIN, parentWidth - 2 * MARGIN, parentHeight- 2 * MARGIN);
			notifBackgroundLogo->setGeometry(notifParentFrame->geometry());
			qInfo() << notifParentFrame->geometry();
		}
	};
}

QT_END_NAMESPACE