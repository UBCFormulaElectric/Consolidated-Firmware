#pragma once

#include <QWidget>

class Squircle {
    QRect bounds;
    QPainter * p;
public:
    explicit Squircle(QPainter * p, QRect bounds);
    explicit Squircle(QPainter * p);
    void setBounds(QRect bounds);
    void paint() const;
};

