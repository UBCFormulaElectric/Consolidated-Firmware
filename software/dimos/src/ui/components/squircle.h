#pragma once

#include <QtQuick/QQuickPaintedItem>
#include <QColor>

// ReSharper disable once CppClassCanBeFinal because of QML
class Squircle : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int radius MEMBER m_radius FINAL REQUIRED)
    Q_PROPERTY(double smoothness MEMBER m_smoothness FINAL)
    Q_PROPERTY(QColor color MEMBER m_color FINAL)

  public:
    explicit Squircle(QQuickItem *parent = nullptr);
    void     paint(QPainter *p) override;

  private:
    int    m_radius;
    double m_smoothness;
    QColor m_color;
};
