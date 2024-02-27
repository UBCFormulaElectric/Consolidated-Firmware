#pragma once
#include <QQmlEngine>
#include <qqmlintegration.h>
#include <QTimer>
#include <QtLogging>

// ReSharper disable once CppClassCanBeFinal
class DimSwitchEmitter : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    QTimer test_emission;

public:
    static DimSwitchEmitter * getInstance()
    {
        if (instance == nullptr)
        {
            instance = new DimSwitchEmitter();
        }
        return instance;
    }
    static DimSwitchEmitter *create(QQmlEngine *qmlEngine, QJSEngine *jsEngine)
    {
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
    explicit DimSwitchEmitter(QObject* parent = nullptr): QObject(parent)
    {
        // TEMPORARY!!! TODO remove this
        test_emission.setInterval(1000);
        test_emission.setSingleShot(false);
        connect(&test_emission, QTimer::timeout, this, settingsButtonPressed);
        test_emission.start();
    }
};