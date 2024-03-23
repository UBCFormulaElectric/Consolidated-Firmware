#include "ShutdownLoop.h"

#include <QPainter>
#include <QPainterPath>
#include <QSvgRenderer>
#include <qstatictext.h>

ShutdownLoop::ShutdownLoop(QQuickItem *parent) : QQuickPaintedItem(parent) {}

// nodes
constexpr int nodeWidth = 15;
// battery
constexpr int batteryWidth  = 50;
constexpr int batteryHeight = 40;
// loop
constexpr int loopPenWidth = 8;
constexpr int loopRadius   = 20;

constexpr int      loopLabelTextSize    = 12;
constexpr int      loopLabelPadDistance = 16;
const QFont        loopLabelFont("Helvetica", loopLabelTextSize, 700, false);
const QFontMetrics loopLabelsFontMetrics(loopLabelFont);

void drawShutdownLoopNode(QPainter *p, const QPointF center)
{
    p->setPen(Qt::NoPen);
    p->setBrush(QColor(0x36FB61));
    p->drawEllipse(center, nodeWidth, nodeWidth);
}

enum class TextAlignment
{
    Left,
    Top,
    Right,
    Bottom
};
void drawShutdownLoopLabel(
    QPainter           *p,
    const double        anchor_x,
    const double        anchor_y,
    const QString      &label,
    const TextAlignment alignment)
{
    p->setFont(loopLabelFont);
    p->setPen(Qt::white);
    const double leftShift = (alignment == TextAlignment::Top || alignment == TextAlignment::Bottom)
                                 ? loopLabelsFontMetrics.horizontalAdvance(label) / 2
                             : alignment == TextAlignment::Right ? loopLabelsFontMetrics.horizontalAdvance(label)
                                                                 : 0;

    const int topShift = (alignment == TextAlignment::Left || alignment == TextAlignment::Right)
                             ? loopLabelsFontMetrics.height() / 2
                         : alignment == TextAlignment::Bottom ? loopLabelsFontMetrics.height()
                                                              : 0;
    p->drawText(
        static_cast<int>(std::round(anchor_x - leftShift)), static_cast<int>(std::round(anchor_y - topShift)),
        loopLabelsFontMetrics.horizontalAdvance(label), loopLabelsFontMetrics.height(), 0, label);
}

void drawShutdownLoop(QPainter *p, const QRectF &loopBounds, const double percentage)
{
    QPainterPath  loop;
    const QPointF start(loopBounds.left(), loopBounds.center().y() - batteryHeight / 2.0 - 10);
    const QPointF end(loopBounds.left(), loopBounds.center().y() + batteryHeight / 2.0 + 10);
    loop.moveTo(start);
    loop.arcTo(QRectF(loopBounds.left(), loopBounds.top(), 2 * loopRadius, 2 * loopRadius), 180, -90);
    loop.arcTo(QRectF(loopBounds.right() - 2 * loopRadius, loopBounds.top(), 2 * loopRadius, 2 * loopRadius), 90, -90);
    loop.arcTo(
        QRectF(
            loopBounds.right() - 2 * loopRadius, loopBounds.bottom() - 2 * loopRadius, 2 * loopRadius, 2 * loopRadius),
        0, -90);
    loop.arcTo(
        QRectF(loopBounds.left(), loopBounds.bottom() - 2 * loopRadius, 2 * loopRadius, 2 * loopRadius), -90, -90);
    loop.lineTo(end);

    p->setPen(QPen(QBrush(0x434343), loopPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    p->drawPath(loop);
    const double loopLength    = loop.length() / 8.0;
    auto         filledPathPen = QPen(QColor(0x36FB61), loopPenWidth, Qt::CustomDashLine, Qt::RoundCap, Qt::RoundJoin);
    filledPathPen.setDashPattern({ loopLength, loopLength });
    filledPathPen.setDashOffset((1 - percentage) * loopLength);
    p->setPen(filledPathPen);
    p->drawPath(loop);

    p->drawText(100, 100, QString::fromStdString(std::to_string(percentage)));

    // const auto point = loop.pointAtPercent(loop.percentAtLength(160));
    // p->setPen(QPen(Qt::red, 10));
    // p->drawPoint(point);
}

void ShutdownLoop::paint(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing, true);
    const QRectF bounds = boundingRect();

    QSvgRenderer batteryRenderer(QStringLiteral(":/sdloop_battery.svg"));
    const auto   center = bounds.center();
    batteryRenderer.render(p, QRectF(bounds.left(), center.y() - batteryHeight / 2.0, batteryWidth, batteryHeight));

    const QRectF loopBounds = bounds.marginsRemoved(QMargins(
        std::max(loopPenWidth / 2, std::max(batteryWidth / 2, nodeWidth)), loopPenWidth / 2 + nodeWidth,
        loopPenWidth / 2 + nodeWidth, loopPenWidth / 2 + nodeWidth));
    drawShutdownLoop(p, loopBounds, m_percentage);

    drawShutdownLoopLabel(
        p, static_cast<int>(loopBounds.left() + loopLabelPadDistance),
        static_cast<int>(loopBounds.top() + 1 * loopBounds.height() / 4), "BSPD", TextAlignment::Left);
    drawShutdownLoopNode(p, { loopBounds.left(), loopBounds.top() + 1 * loopBounds.height() / 4 });

    drawShutdownLoopLabel(
        p, loopBounds.left() + loopBounds.width() / 6, loopBounds.top() + loopLabelPadDistance, "BMS",
        TextAlignment::Top);
    drawShutdownLoopNode(p, { loopBounds.left() + loopBounds.width() / 6, loopBounds.top() });

    drawShutdownLoopLabel(
        p, loopBounds.left() + 3 * loopBounds.width() / 6, loopBounds.top() + loopLabelPadDistance, "IMD",
        TextAlignment::Top);
    drawShutdownLoopNode(p, { loopBounds.left() + 3 * loopBounds.width() / 6, loopBounds.top() });

    drawShutdownLoopLabel(
        p, loopBounds.left() + 5 * loopBounds.width() / 6, loopBounds.top() + loopLabelPadDistance, "EStop",
        TextAlignment::Top);
    drawShutdownLoopNode(p, { loopBounds.left() + 5 * loopBounds.width() / 6, loopBounds.top() });

    drawShutdownLoopLabel(
        p, loopBounds.right() - loopLabelPadDistance, loopBounds.top() + loopBounds.height() / 5, "Inertia Switch",
        TextAlignment::Right);
    drawShutdownLoopNode(p, { loopBounds.right(), loopBounds.top() + loopBounds.height() / 5 });

    drawShutdownLoopLabel(
        p, loopBounds.right() - loopLabelPadDistance, loopBounds.top() + 2 * loopBounds.height() / 5, "BOTS",
        TextAlignment::Right);
    drawShutdownLoopNode(p, { loopBounds.right(), loopBounds.top() + 2 * loopBounds.height() / 5 });

    drawShutdownLoopLabel(
        p, loopBounds.right() - loopLabelPadDistance, loopBounds.top() + 3 * loopBounds.height() / 5, "LEStop",
        TextAlignment::Right);
    drawShutdownLoopNode(p, { loopBounds.right(), loopBounds.top() + 3 * loopBounds.height() / 5 });

    drawShutdownLoopLabel(
        p, loopBounds.right() - loopLabelPadDistance, loopBounds.top() + 4 * loopBounds.height() / 5, "REStop",
        TextAlignment::Right);
    drawShutdownLoopNode(p, { loopBounds.right(), loopBounds.top() + 4 * loopBounds.height() / 5 });

    drawShutdownLoopLabel(
        p, loopBounds.left() + loopBounds.width() / 3, loopBounds.bottom() - loopLabelPadDistance, "TSMS",
        TextAlignment::Bottom);
    drawShutdownLoopNode(p, { loopBounds.left() + loopBounds.width() / 3, loopBounds.bottom() });

    drawShutdownLoopLabel(
        p, loopBounds.left() + 2 * loopBounds.width() / 3, loopBounds.bottom() - loopLabelPadDistance, "HVD Interlock",
        TextAlignment::Bottom);
    drawShutdownLoopNode(p, { loopBounds.left() + 2 * loopBounds.width() / 3, loopBounds.bottom() });

    drawShutdownLoopLabel(
        p, loopBounds.left() + loopLabelPadDistance, loopBounds.top() + 3 * loopBounds.height() / 4, "PCM Interlock",
        TextAlignment::Left);
    drawShutdownLoopNode(p, { loopBounds.left(), loopBounds.top() + 3 * loopBounds.height() / 4 });
}
