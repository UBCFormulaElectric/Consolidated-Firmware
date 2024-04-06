#pragma once

#include <QtQml>
#include <QString>

#define SHARED_CONSTANT(type_quali, type, name, value) \
    static inline type_quali type name = value;        \
    Q_PROPERTY(type name MEMBER name CONSTANT)

// ReSharper disable once CppClassCanBeFinal
class Constants : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QML_UNCREATABLE("Class only exposes constants to QML.")
    QML_SINGLETON

  public:
    SHARED_CONSTANT(constexpr, int, SCREEN_WIDTH, 1024);
    SHARED_CONSTANT(constexpr, int, SCREEN_HEIGHT, 600);

#ifdef USING_TARGET_deploy
    SHARED_CONSTANT(const, QString, WINDOW_TITLE, "Dimos");
    SHARED_CONSTANT(const, bool, deploy, true);
#elif USING_TARGET_dev
    SHARED_CONSTANT(const, QString, WINDOW_TITLE, "[Development] Dimos");
    SHARED_CONSTANT(const, bool, deploy, false);
#endif
};