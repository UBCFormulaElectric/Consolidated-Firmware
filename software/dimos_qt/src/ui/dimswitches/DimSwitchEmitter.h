#pragma once
#include <QQmlEngine>
#include <QTimer>

// ReSharper disable once CppClassCanBeFinal
class DimSwitchEmitter : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
  public:
    static DimSwitchEmitter *getInstance()
    {
        if (instance == nullptr)
            instance = new DimSwitchEmitter();
        return instance;
    }
    static DimSwitchEmitter *create(const QQmlEngine *qmlEngine, const QJSEngine *jsEngine)
    {
        Q_UNUSED(qmlEngine);
        Q_UNUSED(jsEngine);
        return getInstance();
    }
  signals:
    void outButtonPressed();
    void outButtonReleased();
    void errButtonPressed();
    void errButtonReleased();
    void settingsButtonPressed();
    void settingsButtonReleased();
    void f1ButtonPressed();
    void f1ButtonReleased();
    void f2ButtonPressed();
    void f2ButtonReleased();
    void leftRot();
    void rightRot();
    void pushRot();
    void pushRotReleased();

  private:
    static DimSwitchEmitter *instance;
    explicit DimSwitchEmitter(QObject *parent = nullptr) : QObject(parent) {}
};