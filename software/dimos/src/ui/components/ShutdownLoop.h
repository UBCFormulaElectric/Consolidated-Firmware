#pragma once
#include <QQuickPaintedItem>

// ReSharper disable once CppClassCanBeFinal
class ShutdownLoop : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    // Q_PROPERTY(float percentage MEMBER m_percentage FINAL REQUIRED)

  public:
    explicit ShutdownLoop(QQuickItem *parent = nullptr);
    void     paint(QPainter *p) override;

  private:
};