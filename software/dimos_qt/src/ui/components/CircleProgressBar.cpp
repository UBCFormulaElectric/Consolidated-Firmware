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
    const Qt::PenCapStyle tipShape             = m_round_tip ? Qt::RoundCap : Qt::FlatCap;
    const int             direction_multiplier = m_turn_right ? -1 : 1;

    const double remove_margin =
        static_cast<double>(std::max(m_bg_stroke_width.has_value() ? m_bg_stroke_width.value() : -1, m_stroke_width)) /
        2.0;
    const QRectF boundRect =
        boundingRect().marginsRemoved(QMarginsF(remove_margin, remove_margin, remove_margin, remove_margin));

    if (m_background)
    {
        if (!m_bg_start_angle.has_value() || !m_bg_end_angle.has_value())
        {
            m_bg_start_angle = m_start_angle;
            m_bg_end_angle   = m_end_angle;
        }
        if (!m_bg_stroke_width.has_value())
        {
            m_bg_stroke_width = m_stroke_width;
        }
        if (!m_bg_color.has_value())
        {
            qCritical() << "[CircleProgressBar] Background color is not set";
            return;
        }
        if (!m_bg_percentage.has_value())
        {
            qCritical() << "[CircleProgressBar] Background percentage is not set";
            return;
        }
        const int delta_theta = m_bg_end_angle.value() - m_bg_start_angle.value();
        p->setPen(QPen(m_bg_color.value(), m_bg_stroke_width.value(), Qt::SolidLine, tipShape, Qt::RoundJoin));
        p->drawArc(
            boundRect, m_bg_start_angle.value() * 16,
            static_cast<int>(round(delta_theta * 16.0 * m_bg_percentage.value())) * direction_multiplier);
    }

    const int delta_theta = m_end_angle - m_start_angle;
    p->setPen(QPen(m_bar_color, m_stroke_width, Qt::SolidLine, tipShape, Qt::RoundJoin));
    p->drawArc(
        boundRect, m_start_angle * 16,
        static_cast<int>(round(delta_theta * 16.0 * m_percentage)) * direction_multiplier);
}