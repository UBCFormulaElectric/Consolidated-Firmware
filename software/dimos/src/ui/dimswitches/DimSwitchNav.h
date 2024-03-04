#pragma once
#include "DimSwitchEmitter.h"

#include <QObject>
#include <QQmlEngine>
#include <qqmlintegration.h>
#include <qquickitem.h>

// ReSharper disable once CppClassCanBeFinal
class DimSwitchNav : public QObject
{
    Q_OBJECT
    QML_ELEMENT

    QQuickItem *out      = nullptr;
    QQuickItem *err      = nullptr;
    QQuickItem *settings = nullptr;
    QQuickItem *f1       = nullptr;
    QQuickItem *f2       = nullptr;
    QQuickItem *leftRot  = nullptr;
    QQuickItem *rightRot = nullptr;
    QQuickItem *pushRot  = nullptr;

    Q_PROPERTY(QQuickItem *out MEMBER out)
    Q_PROPERTY(QQuickItem *err MEMBER err)
    Q_PROPERTY(QQuickItem *settings MEMBER settings)
    Q_PROPERTY(QQuickItem *f1 MEMBER f1)
    Q_PROPERTY(QQuickItem *f2 MEMBER f2)
    Q_PROPERTY(QQuickItem *leftRot MEMBER leftRot)
    Q_PROPERTY(QQuickItem *rightRot MEMBER rightRot)
    Q_PROPERTY(QQuickItem *pushRot MEMBER pushRot)

  private slots:
    void handleOutPressed() const
    {
        if (out != nullptr)
            out->setFocus(true);
    }
    void handleErrPressed() const
    {
        if (err != nullptr)
            err->setFocus(true);
    }
    void handleSettingsPressed() const
    {
        if (settings != nullptr)
            settings->setFocus(true);
    }
    void handleF1Pressed() const
    {
        if (f1 != nullptr)
            f1->setFocus(true);
    }
    void handleF2Pressed() const
    {
        if (f2 != nullptr)
            f2->setFocus(true);
    }
    void handleLeftRot() const
    {
        if (leftRot != nullptr)
            leftRot->setFocus(true);
    }
    void handleRightRot() const
    {
        if (rightRot != nullptr)
            rightRot->setFocus(true);
    }
    void handlePushRot() const
    {
        if (pushRot != nullptr)
            pushRot->setFocus(true);
    }

    void logTest() const
    {
        qInfo() << out << err << settings << f1 << f2 << leftRot << rightRot << pushRot;
    }

  public:
    explicit DimSwitchNav(QObject *parent = nullptr) : QObject(parent)
    {
        const auto dse = DimSwitchEmitter::getInstance();
        connect(dse, &DimSwitchEmitter::outButtonPressed, this, handleOutPressed);
        connect(dse, &DimSwitchEmitter::errButtonPressed, this, handleErrPressed);
        connect(dse, &DimSwitchEmitter::settingsButtonPressed, this, handleSettingsPressed);
        connect(dse, &DimSwitchEmitter::f1ButtonPressed, this, handleF1Pressed);
        connect(dse, &DimSwitchEmitter::f2ButtonPressed, this, handleF2Pressed);
        connect(dse, &DimSwitchEmitter::leftRot, this, handleLeftRot);
        connect(dse, &DimSwitchEmitter::rightRot, this, handleRightRot);
        connect(dse, &DimSwitchEmitter::pushRot, this, handlePushRot);
        connect(dse, &DimSwitchEmitter::outButtonPressed, this, logTest);
    }
};