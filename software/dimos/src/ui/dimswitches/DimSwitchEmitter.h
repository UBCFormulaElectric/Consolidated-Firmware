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
    static DimSwitchEmitter * getInstance()
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
    void errButtonPressed();
    void settingsButtonPressed();
    void f1ButtonPressed();
    void f2ButtonPressed();
    void leftRot();
    void rightRot();
    void pushRot();

private:
    static DimSwitchEmitter * instance;
    explicit DimSwitchEmitter(QObject* parent = nullptr): QObject(parent) {}
};