#include "squircle.h"
#include <QPainter>
#include <QPainterPath>

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
 * possible radius. \param p painter \param bounds the bounds of the square
 * \param radius the corner radius
 * \param smoothness how much smoothness to apply to the corners
 */
void Squircle::paint(QPainter *p, QRect bounds, int radius, double smoothness)
{
    // clamp radius
    radius = std::clamp(radius, 0, std::min(bounds.width(), bounds.height()) / 2);
    if (smoothness == 0)
        return p->drawRoundedRect(bounds, radius, radius);

    QPainterPath path;
    path.moveTo(radius * (1 + smoothness), 0);

    const int w = bounds.width(), h = bounds.height(), straightWidthLength = w - 2 * (radius * (1 + smoothness)),
              straightHeightLength = h - 2 * (radius * (1 + smoothness));

    const double s = sin(M_PI_4 * smoothness), c = cos(M_PI_4 * smoothness);
    const double p4_long = radius * s, p4_short = radius * (1 - c), p3_grad_x = radius * M_PI_4 * c,
                 p3_grad_y = radius * M_PI_4 * s;

    constexpr struct
    {
        bool isHorizontal;
    } edgeInfo[4] = { { true }, { false }, { true }, { false } };
    for (const auto &einfo : edgeInfo)
    {
        const QPointF pos = path.currentPosition();
        path.moveTo(
            pos.x() + straightWidthLength * (einfo.isHorizontal),
            pos.y() + straightHeightLength * (!einfo.isHorizontal));

        const QPointF p4_1(w - radius + p4_long, p4_short), p4_2(w - p4_short, radius - p4_long);

        const QPointF p3_1(p4_1.x() - (p4_1.y() / p3_grad_y) * p3_grad_x, 0);
        const double  b = 1.5 * pow(pow(p4_1.x() - p3_1.x(), 2) + pow(p4_1.y(), 2), 1.5) / (p4_1.y() * radius);
        const QPointF p2_1(p3_1.x() - b, 0);

        p->setPen(QPen(QColorConstants::Red, 3));
        p->drawPoint(path.currentPosition());
        p->drawPoint(p4_1);
        p->setPen(QPen(QColorConstants::Blue, 3));
        p->drawPoint(p3_1);
        p->drawPoint(p2_1);
        // path.cubicTo(p2_1, p3_1, p4_1);

        // const QPointF p3_1_p(0, 0), p3_2_p(0, 0);
        // path.cubicTo(p3_1_p, p3_2_p, p4_2);
        //
        // const QPointF p2_2(0, 0), p3_2(0, 0);
        // path.cubicTo(p2_2, p3_2, QPointF(w, radius));

        break; // TODO remove after testing first quadrant
    }

    path.closeSubpath();
    p->drawPath(path);
}
