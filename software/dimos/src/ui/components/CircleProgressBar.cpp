#include "CircleProgressBar.h"
#include <QPainter>

// ReSharper disable once CppPossiblyUninitializedMember
CircleProgressBar::CircleProgressBar(QQuickItem *parent) // NOLINT(*-pro-type-member-init)
  : QQuickPaintedItem(parent)
{
}

void CircleProgressBar::paint(QPainter *p)
{
    const QPen bg_pen(
        m_bg_color, m_stroke_width, Qt::SolidLine, m_round_tip ? Qt::RoundCap : Qt::FlatCap, Qt::RoundJoin);
    const QPen bar_pen(
        m_bar_color, m_bar_stroke_width != -1 ? m_bar_stroke_width : m_stroke_width, Qt::SolidLine,
        m_round_tip ? Qt::RoundCap : Qt::FlatCap, Qt::RoundJoin);

    p->setRenderHint(QPainter::Antialiasing);

    const int delta_theta = m_end_angle - m_start_angle;

    const double remove_margin = static_cast<double>(std::max(m_bar_stroke_width, m_stroke_width)) / 2.0;
    const QRectF boundRect =
        this->boundingRect().marginsRemoved(QMarginsF(remove_margin, remove_margin, remove_margin, remove_margin));
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->setPen(bg_pen);
    p->drawEllipse(boundRect);

    p->setPen(bar_pen);
    p->drawArc(
        boundRect, m_start_angle * 16,
        static_cast<int>(round(delta_theta * 16.0 * m_percentage)) * (m_turn_right ? -1 : 1));
}