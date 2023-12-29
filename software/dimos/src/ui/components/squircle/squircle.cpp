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

/**
 * \brief Draws a squircle with the given painter.
 * \note  If given a radius that is too large, it will be clamped to the maximum
 * possible radius.
 * \param p painter
 * \param bounds the bounds of the square
 * \param radius the corner radius
 * \param smoothness how much smoothness to apply to the corners
 */
void Squircle::paint(QPainter *p, QRect bounds, int radius, double smoothness)
{
    // clamp radius
    radius     = std::clamp(radius, 0, std::min(bounds.width(), bounds.height()) / 2);
    smoothness = std::clamp(smoothness, 0.0, 1.0);
    if (radius == 0 || smoothness == 0)
        return p->drawRoundedRect(bounds, radius, radius);

    const double corner_dim = radius * (1 + smoothness);
    QPainterPath path;
    path.moveTo(corner_dim, 0); // top left hand corner

    const int w = bounds.width(), h = bounds.height();
    const double straightWidthLength = w - 2 * corner_dim, straightHeightLength = h - 2 * corner_dim;
    const double s = sin(M_PI_4 * smoothness), c = cos(M_PI_4 * smoothness);
    const double f_1 = radius * (1 - s), f_2 = radius * (1 - c), // f1 is the long side, f2 is the short side
                 f_prime_1 = radius * M_PI_4 * -c, f_prime_2 = radius * M_PI_4 * s;
    const double three_shift = -f_2 * f_prime_1/f_prime_2;

    // the list describes (in order) the approach to tr, br, bl, tl
    p->setPen(QPen(QColorConstants::Green, 1));
    p->drawRoundedRect(bounds, radius, radius); // TODO remove after DEBUG
    for (const struct {
             bool isHorizontal;
             int  baseX, baseY;
         } edgeInfo[4] = {
            { true, w, 0 }, { false, w, h },
            { true, 0, h }, { false, 0, 0 }
         };
         const auto [isH, baseX, baseY] : edgeInfo)
    {
        const QPointF pos = path.currentPosition();
        path.lineTo(pos.x() + straightWidthLength * isH, pos.y() + straightHeightLength * !isH);

        const QPointF p1_1 = path.currentPosition(), p4_1(baseX - f_1, f_2),
                      p3_1(p4_1.x() - three_shift, baseY),
                      p2_1((p1_1.x() + 2 * p3_1.x())/3, 0);

        const QPointF p1_2(baseX, corner_dim), p4_2(baseX - f_2, f_1),
        p3_2(baseX, p4_2.y() + three_shift), p2_2(baseX, (p1_2.y() + 2 * p3_2.y())/3);

        path.cubicTo(p2_1, p3_1, p4_1);
        const int arcCenterX = baseX - 2 * radius, arcCenterY = baseY;
        const double arcAngle = atan((baseY + radius - p4_1.y())/(p4_1.x() - (baseX - radius))) * 180 / M_PI;
        path.arcTo(arcCenterX, arcCenterY, radius * 2, radius * 2, arcAngle, (arcAngle - 45) * -2); // todo make sure that we are at p4_2
        path.cubicTo(p3_2, p2_2, p1_2);

        // TODO REMOVE AFTER DEBUG
        // p->setPen(QPen(QColorConstants::Red, 3));
        // p->drawPoint(p1_1);
        // p->setPen(QPen(QColorConstants::Magenta, 3));
        // p->drawPoint(p4_1);
        // p->setPen(QPen(QColorConstants::Cyan, 3));
        // p->drawPoint(p3_1);
        // p->setPen(QPen(QColorConstants::Green, 3));
        // p->drawPoint(p2_1);
        // p->setPen(QPen(QColorConstants::Red, 3));
        // p->drawPoint(p1_2);
        // p->setPen(QPen(QColorConstants::Magenta, 3));
        // p->drawPoint(p4_2);
        // p->setPen(QPen(QColorConstants::Cyan, 3));
        // p->drawPoint(p3_2);
        // p->setPen(QPen(QColorConstants::Green, 3));
        // p->drawPoint(p2_2);
        // TODO REMOVE AFTER DEBUG
        break;
    }

    // path.closeSubpath();
    p->setPen(QPen(QColorConstants::White, 1));
    p->drawPath(path);
}
