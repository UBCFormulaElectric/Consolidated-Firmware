#pragma once

#include <QObject>
#include <qevent.h>
#include "ui/dimswitches/DimSwitchEmitter.h"

class KeyTranslator final : public QObject
{
    Q_OBJECT
public:
    KeyTranslator() = default;

    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress)
        {
            const auto keyEvent = dynamic_cast<QKeyEvent *>(event);
            const auto dse = DimSwitchEmitter::getInstance();
            // qDebug() << "text" << keyEvent->text() << "key" <<  << "modifiers" << keyEvent->modifiers();
            switch(keyEvent->key())
            {
                case Qt::Key_Escape:
                    emit dse->outButtonPressed();
                    break;
                case Qt::Key_E:
                    emit dse->errButtonPressed();
                    break;
                case Qt::Key_S:
                    emit dse->settingsButtonPressed();
                    break;
                case Qt::Key_F1:
                    emit dse->f1ButtonPressed();
                    break;
                case Qt::Key_F2:
                    emit dse->f2ButtonPressed();
                    break;
                case Qt::Key_Left:
                    emit dse->leftRot();
                    break;
                case Qt::Key_Right:
                    emit dse->rightRot();
                    break;
                case Qt::Key_Enter:
                    emit dse->pushRot();
                    break;
                default:
                    break;
            }
        }
        QObject::eventFilter(watched, event);
        return false;
    }
};
