#include <QGridLayout>
#include <QPainter>
#include <QPainterPath>
#include "squircle.h"

Squircle::Squircle(QPainter * p, int radius): p(p), radius(radius) {
}

Squircle::Squircle(QPainter * p): p(p) {
}

void Squircle::setRadius(int radius) {
    this->radius = radius;
}


void Squircle::paint() const {
    // Using equation x^4 + y^4 = r
    QPainterPath p1, p2, p3 ,p4;

    // LMAO R4
    const double r_four = std::pow(radius, 4);

    p1.moveTo(radius, 0);
    p2.moveTo(radius, 2 * radius);
    p3.moveTo(radius, 0);
    p4.moveTo(radius, 2 * radius);
    for (int x = 0; x <= radius; x += 1) {
        const double
            x_four = std::pow(x, 4),
            y_four = r_four - x_four,
            y = std::pow(y_four, 1.0/4.0);

        p1.lineTo(radius + x, radius - y); //tr  // from prevX + radius, radius - prevY
        p2.lineTo(radius + x, radius + y); //br // from prevX + radius, prevY + radius
        p3.lineTo(radius - x, radius - y); //tl // from radius - prevX, radius - prevY
        p4.lineTo(radius - x, radius + y); //bl // from radius - prevX, prevY + radius
    }
    const auto fullpathlist = p1.toSubpathPolygons()[0] + p2.toReversed().toSubpathPolygons()[0] + p4.toSubpathPolygons()[0] + p3.toReversed().toSubpathPolygons()[0];
    auto fullpath = QPainterPath();
    fullpath.addPolygon(fullpathlist);
    p->drawPath(fullpath);
}