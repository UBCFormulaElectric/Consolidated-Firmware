#pragma once
#include <QQuickPaintedItem>

// ReSharper disable once CppClassCanBeFinal because of QML
class CircleProgressBar : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(float percentage MEMBER m_percentage FINAL REQUIRED)
    Q_PROPERTY(int stroke_width MEMBER m_stroke_width FINAL REQUIRED)
    Q_PROPERTY(int start_angle MEMBER m_start_angle FINAL REQUIRED)
    Q_PROPERTY(int end_angle MEMBER m_end_angle FINAL REQUIRED)
    Q_PROPERTY(QColor bar_color MEMBER m_bar_color FINAL REQUIRED)
    Q_PROPERTY(QColor bg_color MEMBER m_bg_color FINAL REQUIRED)
  public:
    explicit CircleProgressBar(QQuickItem *parent = nullptr);
    void     paint(QPainter *p) override;

  private:
    /**
     * @brief 0 -> 1 value
     */
    float m_percentage;

    /**
     * @brief The stroke_width` of the circle stroke
     */
    int m_stroke_width;

    /**
     * @brief ON ANGLES
     * +angle = counter-clockwise, -angle = clockwise
     * 0 degrees => 3 o'clock position/to the right
     */
    int m_start_angle;
    int m_end_angle;

    QColor m_bar_color;
    QColor m_bg_color;
};