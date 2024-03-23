#include "ShutdownLoop.h"

#include <QPainter>
#include <QSvgRenderer>
#include <qstatictext.h>

// nodes
constexpr int nodeWidth = 15;
// battery
constexpr int batteryWidth  = 50;
constexpr int batteryHeight = 40;
// loop
constexpr int loopPenWidth = 8;
constexpr int loopRadius   = 20;
// loop label
constexpr int      loopLabelTextSize    = 12;
constexpr int      loopLabelPadDistance = 16;
const QFont        loopLabelFont("Helvetica", loopLabelTextSize, 700, false);
const QFontMetrics loopLabelsFontMetrics(loopLabelFont);
// theme colours
const QColor inactiveColour(0x434343);
const QColor activeColour(0x36FB61);
const QColor errorColour(0xFF3A45);

std::map<ShutdownLoop::ShutdownLoopNode, ShutdownLoop::LoopNodeMetadata> ShutdownLoop::node_thresholds = {
    { ShutdownLoopNode::BSPD, { 0.048, "BSPD", TextCenterAnchor::Left } },
    { ShutdownLoopNode::BMS, { 0.153, "BMS", TextCenterAnchor::Top } },
    { ShutdownLoopNode::IMD, { 0.237, "IMD", TextCenterAnchor::Top } },
    { ShutdownLoopNode::EStop, { 0.325, "E-Stop", TextCenterAnchor::Top } },
    { ShutdownLoopNode::InertiaSwitch, { 0.417, "Inertia Switch", TextCenterAnchor::Right } },
    { ShutdownLoopNode::BOTS, { 0.472, "BOTS", TextCenterAnchor::Right } },
    { ShutdownLoopNode::LEStop, { 0.527, "Left E-Stop", TextCenterAnchor::Right } },
    { ShutdownLoopNode::REStop, { 0.583, "Right E-Stop", TextCenterAnchor::Right } },
    { ShutdownLoopNode::HVDInterlock, { 0.718, "HVD Interlock", TextCenterAnchor::Bottom } },
    { ShutdownLoopNode::TSMS, { 0.803, "TSMS", TextCenterAnchor::Bottom } },
    { ShutdownLoopNode::PCMInterlock, { 0.953, "PCM Interlock", TextCenterAnchor::Left } }
};

ShutdownLoop::ShutdownLoop(QQuickItem *parent) : QQuickPaintedItem(parent)
{
    loopBackgroundPen = QPen(inactiveColour, loopPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    loopForegroundPen = QPen(activeColour, loopPenWidth, Qt::CustomDashLine, Qt::RoundCap, Qt::RoundJoin);
}

void ShutdownLoop::drawShutdownLoopNode(
    QPainter              *p,
    const double           percentage,
    const QString         &label,
    const TextCenterAnchor center_anchor) const
{
    enum class LoopNodeState
    {
        inactive,
        active,
        error
    };

    constexpr bool      isNodeError = false;
    const LoopNodeState active      = isNodeError                  ? LoopNodeState::error
                                      : m_percentage >= percentage ? LoopNodeState::active
                                                                   : LoopNodeState::inactive;

    // draw the node
    p->setPen(Qt::NoPen);
    p->setBrush(
        active == LoopNodeState::inactive ? inactiveColour
        : active == LoopNodeState::active ? activeColour
                                          : errorColour);
    p->drawEllipse(loopPath.value().pointAtPercent(percentage), nodeWidth, nodeWidth);

    // draw the text
    p->setFont(loopLabelFont);
    p->setPen(active == LoopNodeState::inactive ? inactiveColour : Qt::white);
    const double leftShift = (center_anchor == TextCenterAnchor::Top || center_anchor == TextCenterAnchor::Bottom)
                                 ? loopLabelsFontMetrics.horizontalAdvance(label) / 2
                             : center_anchor == TextCenterAnchor::Right
                                 ? loopLabelsFontMetrics.horizontalAdvance(label) + loopLabelPadDistance
                                 : -loopLabelPadDistance;
    const int    topShift  = (center_anchor == TextCenterAnchor::Left || center_anchor == TextCenterAnchor::Right)
                                 ? loopLabelsFontMetrics.height() / 2
                             : center_anchor == TextCenterAnchor::Bottom
                                 ? loopLabelsFontMetrics.height() + loopLabelPadDistance
                                 : -loopLabelPadDistance;
    assert(loopPath.has_value());
    const QPointF point = loopPath.value().pointAtPercent(percentage) - QPointF(leftShift, topShift);
    p->drawText(
        static_cast<int>(std::round(point.x())), static_cast<int>(std::round(point.y())),
        loopLabelsFontMetrics.horizontalAdvance(label), loopLabelsFontMetrics.height(), 0, label);
}

QPainterPath ShutdownLoop::renderShutdownLoopPath(const QRectF &loopBounds)
{
    qInfo() << "Rerendering Shutdown Loop Path";
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
    return loop;
}

void ShutdownLoop::drawShutdownLoopPath(QPainter *p)
{
    assert(loopPath.has_value());
    p->setPen(loopBackgroundPen);
    p->drawPath(loopPath.value());

    const double loopLength = loopPath.value().length() / 8.0;
    loopForegroundPen.setDashPattern({ loopLength, loopLength });
    loopForegroundPen.setDashOffset((1 - m_percentage) * loopLength);
    p->setPen(loopForegroundPen);
    p->drawPath(loopPath.value());

#ifdef QT_DEBUG
    p->drawText(100, 100, QString::fromStdString(std::to_string(percentage)));
#endif
}

void ShutdownLoop::paint(QPainter *p)
{
    // set up painter
    p->setRenderHint(QPainter::Antialiasing, true);
    const QRectF bounds = boundingRect();

    // draw battery
    QSvgRenderer batteryRenderer(QStringLiteral(":/sdloop_battery.svg"));
    const auto   center = bounds.center();
    batteryRenderer.render(p, QRectF(bounds.left(), center.y() - batteryHeight / 2.0, batteryWidth, batteryHeight));

    // draw loop path
    const QRectF loopBounds = bounds.marginsRemoved(QMargins(
        std::max(loopPenWidth / 2, std::max(batteryWidth / 2, nodeWidth)), loopPenWidth / 2 + nodeWidth,
        loopPenWidth / 2 + nodeWidth, loopPenWidth / 2 + nodeWidth));
    if (!loopPath.has_value())
        loopPath = renderShutdownLoopPath(loopBounds);
    drawShutdownLoopPath(p);

    for (const auto &[node, lnmd] : node_thresholds)
    {
        drawShutdownLoopNode(p, lnmd.percentage, QString::fromStdString(lnmd.name), lnmd.center_anchor);
#ifdef QT_DEBUG
        static std::map<ShutdownLoopNode, QColor> colorMap = { { ShutdownLoopNode::BSPD, Qt::red },
                                                               { ShutdownLoopNode::BMS, Qt::cyan },
                                                               { ShutdownLoopNode::IMD, Qt::green },
                                                               { ShutdownLoopNode::EStop, Qt::gray },
                                                               { ShutdownLoopNode::InertiaSwitch, Qt::yellow },
                                                               { ShutdownLoopNode::BOTS, Qt::darkBlue },
                                                               { ShutdownLoopNode::LEStop, Qt::magenta },
                                                               { ShutdownLoopNode::REStop, Qt::darkGreen },
                                                               { ShutdownLoopNode::TSMS, Qt::darkRed },
                                                               { ShutdownLoopNode::HVDInterlock, Qt::blue },
                                                               { ShutdownLoopNode::PCMInterlock, Qt::darkYellow } };
        const auto                                point    = loopPath.value().pointAtPercent(lnmd.percentage);
        p->setPen(QPen(colorMap.at(node), 10));
        p->drawPoint(point);
#endif
    }
}
