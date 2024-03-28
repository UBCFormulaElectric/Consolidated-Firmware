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
    Q_PROPERTY(float percentage READ get_m_percentage WRITE set_m_percentage REQUIRED)
    // background on
    Q_PROPERTY(bool background MEMBER m_background)
    // background properties
    Q_PROPERTY(int bg_stroke_width READ get_bg_stroke_width WRITE set_bg_stroke_width) // only for override
    Q_PROPERTY(QColor bg_color READ get_bg_color WRITE set_bg_color)
    Q_PROPERTY(int bg_start_angle READ get_bg_start_angle WRITE set_bg_start_angle)
    Q_PROPERTY(int bg_end_angle READ get_bg_end_angle WRITE set_bg_end_angle)
    Q_PROPERTY(float bg_percentage READ get_bg_percentage WRITE set_bg_percentage)
    // angles
    Q_PROPERTY(bool turn_right MEMBER m_turn_right)
  public:
    explicit CircleProgressBar(QQuickItem *parent = nullptr);
    void     paint(QPainter *p) override;

  private:
    /**
     * @brief -1 -> 1 value
     */
    [[nodiscard]] float get_m_percentage() const { return m_percentage; }
    void                set_m_percentage(const float v)
    {
        m_percentage = v;
        update();
    }
    float                m_percentage;
    std::optional<float> m_bg_percentage = std::nullopt;
    [[nodiscard]] float  get_bg_percentage() const
    {
        assert(m_bg_percentage.has_value());
        return m_bg_percentage.value();
    }
    void set_bg_percentage(float v) { m_bg_percentage = v; }

    /**
     * @brief The stroke_width` of the circle stroke
     */
    int                m_stroke_width;
    std::optional<int> m_bg_stroke_width = std::nullopt;
    [[nodiscard]] int  get_bg_stroke_width() const
    {
        assert(m_bg_stroke_width.has_value());
        return m_bg_stroke_width.value();
    }
    void set_bg_stroke_width(int v) { m_bg_stroke_width = v; }

    /**
     * @brief ON ANGLES
     * +angle = counter-clockwise, -angle = clockwise
     * 0 degrees => 3 o'clock position/to the right
     */
    int                m_start_angle;
    int                m_end_angle;
    std::optional<int> m_bg_start_angle = std::nullopt;
    std::optional<int> m_bg_end_angle   = std::nullopt;
    [[nodiscard]] int  get_bg_start_angle() const
    {
        assert(m_bg_start_angle.has_value());
        return m_bg_start_angle.value();
    }
    void              set_bg_start_angle(int v) { m_bg_start_angle = v; }
    [[nodiscard]] int get_bg_end_angle() const
    {
        assert(m_bg_end_angle.has_value());
        return m_bg_end_angle.value();
    }
    void set_bg_end_angle(int v) { m_bg_end_angle = v; }

    /**
     * @brief Whether or not the stroke is round
     */
    bool m_round_tip;

    QColor                m_bar_color;
    std::optional<QColor> m_bg_color = std::nullopt;
    [[nodiscard]] QColor  get_bg_color() const
    {
        assert(m_bg_color.has_value());
        return m_bg_color.value();
    }
    void set_bg_color(QColor v) { m_bg_color = v; }

    bool m_turn_right = false;
    bool m_background = false;
};