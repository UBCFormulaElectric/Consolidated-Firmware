#include <QGridLayout>
#include "squircle.h"
#include <iostream>

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

    for (int x = 1; x*x*x*x <= radius; x++) {
        int y = radius - x*x*x*x;
        int prevX = x-1;
        int prevY = radius - prevX*prevX*prevX*prevX;
        painter.drawLine(prevX, prevY, x, radius - y);
        painter.drawLine(prevX + radius, prevY, x + radius, radius - y);
        painter.drawLine(prevX, prevY, x, radius + y);
        painter.drawLine(prevX + radius, prevY, x + radius, radius + y);
        std:: cout << x << " " << y << " " << prevX << " " << prevY << "\n";
    }


}