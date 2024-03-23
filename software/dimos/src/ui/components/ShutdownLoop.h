#pragma once
#include <QQuickPaintedItem>
#include <QPainterPath>
#include <QPen>
#include <map>

// ReSharper disable once CppClassCanBeFinal
class ShutdownLoop : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(double percentage READ get_m_percentage WRITE set_m_percentage)

  public:
    explicit ShutdownLoop(QQuickItem *parent = nullptr);
    void paint(QPainter *p) override;

    enum class ShutdownLoopNode
    {
        BSPD,
        BMS,
        IMD,
        EStop,
        InertiaSwitch,
        BOTS,
        LEStop,
        REStop,
        TSMS,
        HVDInterlock,
        PCMInterlock,
    };

  private:
    enum class TextCenterAnchor
    {
        Left,
        Top,
        Right,
        Bottom
    };
    struct LoopNodeMetadata
    {
        double           percentage;
        std::string      name;
        TextCenterAnchor center_anchor;
    };
    static std::map<ShutdownLoopNode, LoopNodeMetadata> node_thresholds;

    [[nodiscard]] double get_m_percentage() const { return m_percentage; }
    void                 set_m_percentage(const double value)
    {
        m_percentage = value;
        update();
    }
    /**
     * Value between 0-1 representing the percentage of the shutdown loop that has been completed.
     */
    double m_percentage = 1.0;

    void drawShutdownLoopNode(QPainter *p, double percentage, const QString &label, TextCenterAnchor center_anchor)
        const;
    void                        drawShutdownLoopPath(QPainter *p);
    static QPainterPath         renderShutdownLoopPath(const QRectF &loopBounds);
    std::optional<QPainterPath> loopPath;
    QPen                        loopForegroundPen;
    QPen                        loopBackgroundPen;
};