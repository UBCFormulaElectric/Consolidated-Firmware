#pragma once

#include <QWidget>

class Squircle {
    QRect bounds;
public:
    explicit Squircle(QRect bounds);
    void setBounds(QRect bounds);
    void paint(QPainter * p) const;
};

