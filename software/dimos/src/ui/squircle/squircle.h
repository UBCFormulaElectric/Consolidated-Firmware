#pragma once

#include <QtQuick/QQuickPaintedItem>
#include <QColor>

// ReSharper disable once CppClassCanBeFinal because of QML
class Squircle : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int radius READ getRadius WRITE setRadius FINAL)
    Q_PROPERTY(double smoothness READ getSmoothness WRITE setSmoothness FINAL)
    Q_PROPERTY(QColor color READ getColor WRITE setColor FINAL)

  public:
    explicit Squircle(QQuickItem *parent = nullptr);
    void paint(QPainter *p) override;

    [[nodiscard]] int getRadius() const { return m_radius; }
    void              setRadius(const int &radius) { this->m_radius = radius; }

    [[nodiscard]] double getSmoothness() const { return m_smoothness; }
    void                 setSmoothness(const double &smoothness) { this->m_smoothness = smoothness; }

    [[nodiscard]] QColor getColor() const { return m_color; }
    void                 setColor(const QColor &color) { this->m_color = color; }

  private:
    int    m_radius;
    double m_smoothness;
    QColor m_color;
};
