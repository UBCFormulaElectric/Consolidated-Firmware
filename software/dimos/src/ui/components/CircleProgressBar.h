#pragma once
#include <QQuickPaintedItem>

// ReSharper disable once CppClassCanBeFinal because of QML
class CircleProgressBar : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(bool round_tip MEMBER m_round_tip REQUIRED)
    // bar properties
    Q_PROPERTY(int stroke_width MEMBER m_stroke_width REQUIRED)
    Q_PROPERTY(QColor bar_color MEMBER m_bar_color REQUIRED)
    Q_PROPERTY(int start_angle MEMBER m_start_angle REQUIRED)
    Q_PROPERTY(int end_angle MEMBER m_end_angle REQUIRED)
    Q_PROPERTY(float percentage MEMBER m_percentage REQUIRED)
    // background on
    Q_PROPERTY(bool background MEMBER m_background)
    // background properties
    Q_PROPERTY(int bg_stroke_width MEMBER m_bg_stroke_width) // only for override
    Q_PROPERTY(QColor bg_color MEMBER m_bg_color)
    Q_PROPERTY(int bg_start_angle MEMBER m_bg_start_angle)
    Q_PROPERTY(int bg_end_angle MEMBER m_bg_end_angle)
    Q_PROPERTY(float bg_percentage MEMBER m_bg_percentage)
    // angles
    Q_PROPERTY(bool turn_right MEMBER m_turn_right)
  public:
    explicit CircleProgressBar(QQuickItem *parent = nullptr);
    void     paint(QPainter *p) override;

  private:
    /**
     * @brief -1 -> 1 value
     */
    float m_percentage;
    float m_bg_percentage;

    /**
     * @brief The stroke_width` of the circle stroke
     */
    int m_stroke_width;
    int m_bg_stroke_width;

    /**
     * @brief ON ANGLES
     * +angle = counter-clockwise, -angle = clockwise
     * 0 degrees => 3 o'clock position/to the right
     */
    int m_start_angle;
    int m_end_angle;
    int m_bg_start_angle;
    int m_bg_end_angle;

    /**
     * @brief Whether or not the stroke is round
     */
    bool m_round_tip;

    QColor m_bar_color;
    QColor m_bg_color;

    bool m_turn_right = false;
    bool m_background = false;
};