#include "CircleProgressBar.h"
#include <QPainter>

// ReSharper disable once CppPossiblyUninitializedMember
CircleProgressBar::CircleProgressBar(QQuickItem *parent) // NOLINT(*-pro-type-member-init)
  : QQuickPaintedItem(parent)
{
}

void CircleProgressBar::paint(QPainter *p)
{
    p->setRenderHint(QPainter::Antialiasing);
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    const Qt::PenCapStyle tipShape = m_round_tip ? Qt::RoundCap : Qt::FlatCap;

    const double remove_margin = static_cast<double>(std::max(m_bg_stroke_width, m_stroke_width)) / 2.0;
    const QRectF boundRect =
        boundingRect().marginsRemoved(QMarginsF(remove_margin, remove_margin, remove_margin, remove_margin));

    if (m_background)
    {
        p->setPen(QPen(
            m_bg_color, m_bg_stroke_width != -1 ? m_bg_stroke_width : m_stroke_width, Qt::SolidLine, tipShape,
            Qt::RoundJoin));
        p->drawEllipse(boundRect);
    }

    const int delta_theta = m_end_angle - m_start_angle;
    p->setPen(QPen(m_bar_color, m_stroke_width, Qt::SolidLine, tipShape, Qt::RoundJoin));
    p->drawArc(
        boundRect, m_start_angle * 16,
        static_cast<int>(round(delta_theta * 16.0 * m_percentage)) * (m_turn_right ? -1 : 1));
}