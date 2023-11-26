#include <QGridLayout>
#include "SwitcherButton.h"

SwitcherButton::SwitcherButton(SwitcherButtonOption event, QWidget *parent) : QPushButton(parent) {
	setupUi(event);
}

void SwitcherButton::setupUi(SwitcherButtonOption event) {
	this->setFixedSize(QSize(200, 200));
	this->setStyleSheet("QPushButton {"
						"	background-color:#002D67;"
						"	border-radius: 50px;"
						"}");
	this->setContentsMargins(0, 0, 0, 0);
	this->setObjectName("SwitcherButton_"+switcherOptionToName[event]);

	auto gridLayout = new QGridLayout(this);
	gridLayout->setObjectName("SwitcherButton_"+switcherOptionToName[event]+"_gridLayout");
	iconLabel = new QLabel();
	iconLabel->setObjectName("SwitcherButton_"+switcherOptionToName[event]+"_iconLabel");
	auto icon = QIcon(optionToImagePath[event]);
	auto iconPixmap = icon.pixmap(QSize(140, 130));
	iconLabel->setPixmap(iconPixmap);
	gridLayout->addWidget(iconLabel, 0, 0, Qt::AlignCenter);
	this->setLayout(gridLayout);
}

void SwitcherButton::paintEvent(QPaintEvent *event)
{
	// static const QPoint hourHand[4] = {
	// 		QPoint(5, 14),
	// 		QPoint(-5, 14),
	// 		QPoint(-4, -71),
	// 		QPoint(4, -71)
	// };
	// static const QPoint minuteHand[4] = {
	// 		QPoint(4, 14),
	// 		QPoint(-4, 14),
	// 		QPoint(-3, -89),
	// 		QPoint(3, -89)
	// };
	//
	// static const QPoint secondsHand[4] = {
	// 		QPoint(1, 14),
	// 		QPoint(-1, 14),
	// 		QPoint(-1, -89),
	// 		QPoint(1, -89)
	// };
	//
	// const QColor hourColor(255, 255, 255);
	// const QColor minuteColor(255, 255, 255);
	// const QColor secondsColor(255, 255, 255);
	//
	// int side = qMin(width(), height());
	//
	// QPainter painter(this);
	// painter.setRenderHint(QPainter::Antialiasing);
	// painter.translate(width() / 2, height() / 2);
	// painter.scale(side / 200.0, side / 200.0);
	// QTime time = QTime::currentTime();
	// painter.setPen(Qt::NoPen);
	//
	// painter.setBrush(hourColor);
	// painter.save();
	// painter.rotate(30.0 * ((time.hour() + time.minute() / 60.0)));
	// painter.drawConvexPolygon(hourHand, 4);
	// painter.restore();
	//
	// for (int i = 0; i < 12; ++i) {
	// 	painter.drawRect(73, -3, 16, 6);
	// 	painter.rotate(30.0);
	// }
	// painter.setBrush(minuteColor);
	// painter.save();
	// painter.rotate(6.0 * time.minute());
	// painter.drawConvexPolygon(minuteHand, 4);
	// painter.restore();
	//
	// painter.setBrush(secondsColor);
	// painter.save();
	// painter.rotate(6.0 * time.second());
	// painter.drawConvexPolygon(secondsHand, 4);
	// painter.drawEllipse(-3, -3, 6, 6);
	// painter.drawEllipse(-5, -68, 10, 10);
	// painter.restore();
	//
	// painter.setPen(minuteColor);
	// for (int j = 0; j < 60; ++j) {
	// 	painter.drawLine(92, 0, 96, 0);
	// 	painter.rotate(6.0);
	// }

	QPushButton::paintEvent(event);
}