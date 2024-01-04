#pragma once

#include <QtQml>
#include <QString>

#define SHARED_CONSTANT(type_quali, type, name, value)                         \
  static inline type_quali type name = value;                                  \
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

    static QString getWindowTitle() {
      QString out;
#ifdef USING_dimos
        out = "Dimos";
#endif
#ifdef USING_dimos_dev
        out = "[Development] Dimos";
#endif
      return out;
    }
    Q_PROPERTY(QString WINDOW_TITLE READ getWindowTitle CONSTANT)
};