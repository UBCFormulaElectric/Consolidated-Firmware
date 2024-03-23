#pragma once

#include <QPainterPath>
#include <QtQuick/QQuickPaintedItem>
#include <QColor>

struct SquircleSettings
{
    double         w, h, radius, smoothness;
    auto           operator<=>(const SquircleSettings &) const = default;
    friend QDebug &operator<<(QDebug dbg, const SquircleSettings &settings)
    {
        return dbg << "Settigs: " << settings.w << settings.h << settings.radius << settings.smoothness;
    }
};

// ReSharper disable once CppClassCanBeFinal because of QML
class Squircle : public QQuickPaintedItem
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(int radius MEMBER m_radius FINAL REQUIRED)
    Q_PROPERTY(double smoothness MEMBER m_smoothness FINAL)
    Q_PROPERTY(QColor bg_color MEMBER m_color FINAL)

  public:
    explicit Squircle(QQuickItem *parent = nullptr);
    void     paint(QPainter *p) override;

  private:
    std::optional<QPainterPath>     cachedSquirclePath;
    std::optional<SquircleSettings> cachedSettings;
    /**
     * @param p Pass in a QPen for debug purposes
     */
    void updateCachedPathFromCachedSettings(QPainter *p);

    int    m_radius;
    double m_smoothness;
    QColor m_color;
};
