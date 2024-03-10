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
        qInfo() << "forcing active focus on" << out;
        if (out != nullptr)
        {
            out->forceActiveFocus();
        }
    }
    void handleErrPressed() const
    {
        if (err != nullptr)
            err->forceActiveFocus();
    }
    void handleSettingsPressed() const
    {
        if (settings != nullptr)
            settings->forceActiveFocus();
    }
    void handleF1Pressed() const
    {
        if (f1 != nullptr)
            f1->forceActiveFocus();
    }
    void handleF2Pressed() const
    {
        if (f2 != nullptr)
            f2->forceActiveFocus();
    }
    void handleLeftRot() const
    {
        if (leftRot != nullptr)
            leftRot->forceActiveFocus();
    }
    void handleRightRot() const
    {
        if (rightRot != nullptr)
            rightRot->forceActiveFocus();
    }
    void handlePushRot() const
    {
        if (pushRot != nullptr)
            pushRot->forceActiveFocus();
    }

    void logTest() const { qInfo() << out << err << settings << f1 << f2 << leftRot << rightRot << pushRot; }

  public:
    explicit DimSwitchNav(QObject *parent = nullptr) : QObject(parent)
    {
        const auto dse = DimSwitchEmitter::getInstance();
        connect(dse, &DimSwitchEmitter::outButtonPressed, this, &DimSwitchNav::handleOutPressed);
        connect(dse, &DimSwitchEmitter::errButtonPressed, this, &DimSwitchNav::handleErrPressed);
        connect(dse, &DimSwitchEmitter::settingsButtonPressed, this, &DimSwitchNav::handleSettingsPressed);
        connect(dse, &DimSwitchEmitter::f1ButtonPressed, this, &DimSwitchNav::handleF1Pressed);
        connect(dse, &DimSwitchEmitter::f2ButtonPressed, this, &DimSwitchNav::handleF2Pressed);
        connect(dse, &DimSwitchEmitter::leftRot, this, &DimSwitchNav::handleLeftRot);
        connect(dse, &DimSwitchEmitter::rightRot, this, &DimSwitchNav::handleRightRot);
        connect(dse, &DimSwitchEmitter::pushRot, this, &DimSwitchNav::handlePushRot);
    }
};