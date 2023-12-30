#include "squircle.h"
#include <QPainter>
#include <QPainterPath>
#include <iostream>

// naive/very slow implementation
// void Squircle::paint() const {
//     // Using equation x^4 + y^4 = r
//     QPainterPath p1, p2, p3 ,p4;
//
//     constexpr int power = 4;
//     // LMAO R4
//     const double r_four = std::pow(radius, power);
//
//     p1.moveTo(radius, 0);
//     p2.moveTo(radius, 2 * radius);
//     p3.moveTo(radius, 0);
//     p4.moveTo(radius, 2 * radius);
//     for (int x = 0; x <= radius; x += 1) {
//         const double
//             x_four = std::pow(x, power),
//             y_four = r_four - x_four,
//             y = std::pow(y_four, 1.0/static_cast<double>(power));
//
//         p1.lineTo(radius + x, radius - y); //tr  // from prevX + radius,
//         radius - prevY p2.lineTo(radius + x, radius + y); //br // from prevX
//         + radius, prevY + radius p3.lineTo(radius - x, radius - y); //tl //
//         from radius - prevX, radius - prevY p4.lineTo(radius - x, radius +
//         y); //bl // from radius - prevX, prevY + radius
//     }
//     const auto fullpathlist = p1.toSubpathPolygons()[0] +
//     p2.toReversed().toSubpathPolygons()[0] + p4.toSubpathPolygons()[0] +
//     p3.toReversed().toSubpathPolygons()[0]; auto fullpath = QPainterPath();
//     fullpath.addPolygon(fullpathlist);
//     p->drawPath(fullpath);
// }

constexpr static double rad_deg_factor = 180/M_PI;
#define RAD_TO_DEG(x) ((x)*rad_deg_factor)

/**
 * \brief Draws a squircle with the given painter.
 * \note  If given a radius that is too large, it will be clamped to the maximum
 * possible radius.
 * \param p painter
 * \param bounds the bounds of the square
 * \param radius the corner radius
 * \param smoothness how much smoothness to apply to the corners
 */
void Squircle::paint(QPainter *p, QRect bounds, int radius, double smoothness) {
  // clamp radius
  radius = std::clamp(radius, 0, std::min(bounds.width(), bounds.height()) / 2);
  smoothness = std::clamp(smoothness, 0.0, 1.0); // todo find a better clamp (depending on R)
  if (radius == 0 || smoothness == 0)
    return p->drawRoundedRect(bounds, radius, radius);

  const int w = bounds.width(), h = bounds.height();
  const double corner_dim = radius * (1 + smoothness),
               straightWidthLength = w - 2 * corner_dim,
               straightHeightLength = h - 2 * corner_dim,
               s = sin(M_PI_4 * smoothness),
               c = cos(M_PI_4 * smoothness),
               f_1 = radius * (1 - s),
               f_2 = radius * (1 - c), // f1 is the long side, f2 is the short side
               f_prime_1 = radius * M_PI_4 * c,
               f_prime_2 = radius * M_PI_4 * s,
               three_shift = f_1 + f_2 * f_prime_1 / f_prime_2;

  QPainterPath path(QPointF(corner_dim, 0)); // start tl
  // the list describes (in order) the approach to tr, br, bl, tl
  for (const struct {
         bool isHorizontal;
         int baseX, baseY;
         int xTransform, yTransform;
         int baseAngleOffset;
       } edgeInfo[4] = {{true,  w, 0, -1, 1, 0},
                        {false, w, h, -1, -1, 270},
                        {true,  0, h, 1, -1, 180},
                        {false, 0, 0, 1, 1, 90}};
       const auto [isH, bx, by, xt, yt, angleOffset] : edgeInfo) {
    // horizontal travel
    const QPointF startPos = path.currentPosition();
    path.lineTo(startPos.x() + isH * straightWidthLength * -xt, startPos.y() + !isH * straightHeightLength * -yt);

    // define the points
    const QPointF p1_1(bx + corner_dim * xt,          by),
                  p4_1(bx + f_1 * xt,                 by + f_2 * yt),
                  p3_1(bx + three_shift * xt,         by),
                  p2_1((p1_1.x() + 2 * p3_1.x()) / 3, by),
                  p1_2(bx,                            by + corner_dim * yt),
                  p4_2(bx + f_2 * xt,                 by + f_1 * yt),
                  p3_2(bx,                            by + three_shift * yt),
                  p2_2(bx,                            (p1_2.y() + 2 * p3_2.y()) / 3);

    const double unCorrectedArcAngle = RAD_TO_DEG(atan2(abs(by + radius * yt - p4_1.y()), abs(bx + radius * xt - p4_1.x()))),
                 arcAngle = unCorrectedArcAngle;
    // assert(path.currentPosition() == p1_1);

    // draw
    if(isH) {
      path.cubicTo(p2_1, p3_1, p4_1);
      path.arcTo(bx + (xt - 1) * radius, by + (yt - 1) * radius, radius * 2, radius * 2, arcAngle + angleOffset, (arcAngle - 45) * -2);
      // const QPointF finishArcPos = path.currentPosition();
      // assert(pow(finishArcPos.x() - p4_2.x(), 2) + pow(finishArcPos.y() - p4_2.y(), 2) < pow(0.5, 2)); // ensure delta < 1/2 of pixel
      path.cubicTo(p3_2, p2_2, p1_2);
    } else {
      path.cubicTo(p2_2, p3_2, p4_2);
      path.arcTo(bx + (xt - 1) * radius, by + (yt - 1) * radius, radius * 2, radius * 2, arcAngle + angleOffset, (arcAngle - 45) * -2);
      path.cubicTo(p3_1, p2_1, p1_1);
    }

    #ifndef QT_DEBUG
    p->setPen(QPen(QColorConstants::Red, 3));
    p->drawPoint(p1_1);
    p->setPen(QPen(QColorConstants::Magenta, 3));
    p->drawPoint(p4_1);
    p->setPen(QPen(QColorConstants::Cyan, 3));
    p->drawPoint(p3_1);
    p->setPen(QPen(QColorConstants::Green, 3));
    p->drawPoint(p2_1);
    p->setPen(QPen(QColorConstants::Red, 3));
    p->drawPoint(p1_2);
    p->setPen(QPen(QColorConstants::Magenta, 3));
    p->drawPoint(p4_2);
    p->setPen(QPen(QColorConstants::Cyan, 3));
    p->drawPoint(p3_2);
    p->setPen(QPen(QColorConstants::Green, 3));
    p->drawPoint(p2_2);
    #endif
  }
  p->drawPath(path);
}
