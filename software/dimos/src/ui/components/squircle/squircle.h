#pragma once

#include <QWidget>

class Squircle
{
  public:
    static void paint(QPainter *p, QRect bounds, int radius, double smoothness);
};
