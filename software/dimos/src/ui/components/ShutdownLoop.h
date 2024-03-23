#pragma once
#include <QQuickPaintedItem>

// ReSharper disable once CppClassCanBeFinal
class ShutdownLoop : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(double percentage READ get_m_percentage WRITE set_m_percentage)

  public:
    explicit ShutdownLoop(QQuickItem *parent = nullptr);
    void     paint(QPainter *p) override;

  private:
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
};