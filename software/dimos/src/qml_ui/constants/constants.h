#pragma once

#include <QtQml>

#define SHARED_CONSTANT(type, name, value)     \
    static inline constexpr type name = value; \
    Q_PROPERTY(type name MEMBER name CONSTANT)

class Constants : public QObject
{
    Q_OBJECT
    QML_UNCREATABLE("Class only exposes constants to QML.")
    QML_ELEMENT
    QML_SINGLETON

  public:
    SHARED_CONSTANT(int, SCREEN_WIDTH, 1024);
    SHARED_CONSTANT(int, SCREEN_HEIGHT, 600);
};