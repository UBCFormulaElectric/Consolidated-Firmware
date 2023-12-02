#include <QGridLayout>
#include "squircle.h"

Squircle::Squircle(int radius, QWidget *parent) : QWidget(parent) {
    this->radius = radius;
    setupUi();
}

void Squircle::setupUi() {
    this->setContentsMargins(0, 0, 0, 0);

    // Setting the square for the squircle
    this->setFixedSize(2*radius, 2*radius);

    // I am Thanos
}

void Squircle::paintEvent(QPaintEvent *event) {
    // Using equation x^4 + y^4 = r

    QPainter painter(this);
    QPolygon border;

    double r_four, x, y, x_four, y_four, prevX, prevY, prevX_four, prevY_four;

    // LMAO R4
    r_four = std::pow(radius, 4);

    for (double i = 0; i <= radius; i += 0.1) {
        x = i;
        x_four = std::pow(x, 4);
        y_four = r_four - x_four;
        y = std::pow(y_four, 1.0/4.0);
        std::cout << y_four << " " << y <<  "\n";

        prevX = x-0.1;
        prevX_four = std::pow(prevX, 4);
        prevY_four = r_four - prevX_four;
        prevY = std::pow(prevY_four, 1.0/4.0);

        // right side
        painter.drawLine(prevX + radius, radius - prevY, x + radius, radius - y);
        painter.drawLine(prevX + radius, prevY + radius, x + radius, y + radius);

        // left side
        painter.drawLine(radius - prevX, radius - prevY, radius - x, radius - y);
        painter.drawLine(radius - prevX, prevY + radius, radius - x, y + radius);

        // border << QPoint(x + radius, radius - y);
        // border << QPoint(x + radius, y + radius);
        // border << QPoint(radius - x, radius - y);
        // border << QPoint(radius - x, y + radius);
    }

    // painter.setClipRegion(border);

    QWidget::paintEvent(event);
}