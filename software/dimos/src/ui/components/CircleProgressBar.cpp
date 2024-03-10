#include "CircleProgressBar.h"
#include <QPainter>

// ReSharper disable once CppPossiblyUninitializedMember
CircleProgressBar::CircleProgressBar(QQuickItem *parent) // NOLINT(*-pro-type-member-init)
  : QQuickPaintedItem(parent)
{
}

void CircleProgressBar::paint(QPainter *p)
{
    const QPen bg_pen(m_bg_color, m_stroke_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    const QPen bar_pen(m_bar_color, m_stroke_width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p->setRenderHint(QPainter::Antialiasing);

    const int delta_theta = m_end_angle - m_start_angle;

    const QRectF boundRect = this->boundingRect().marginsRemoved(
        QMargins(m_stroke_width / 2, m_stroke_width / 2, m_stroke_width / 2, m_stroke_width / 2));
    p->setCompositionMode(QPainter::CompositionMode_SourceOver);
    p->setPen(bg_pen);
    p->drawArc(boundRect, m_start_angle * 16, delta_theta * 16);

    p->setPen(bar_pen);
    p->drawArc(boundRect, m_start_angle * 16, static_cast<int>(round(delta_theta * 16.0 * m_percentage)));

    p->end();
}