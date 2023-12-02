#pragma once

#include <QWidget>

class Squircle {
    int radius = 1;
    QPainter * p;
public:
    explicit Squircle(QPainter * p, int radius);
    explicit Squircle(QPainter * p);
    void setRadius(int radius);
    void paint() const;
};

