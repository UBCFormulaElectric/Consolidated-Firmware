#pragma once

#include <QObject>
#include <qevent.h>
#include "../ui/dimswitches/DimSwitchEmitter.h"

class KeyTranslator final : public QObject
{
    Q_OBJECT
  public:
    KeyTranslator() = default;

    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
        {
            const auto keyEvent = dynamic_cast<QKeyEvent *>(event);
            const auto dse      = DimSwitchEmitter::getInstance();
            bool       handled  = true;
            if (event->type() == QEvent::KeyPress)
            {
                switch (keyEvent->key())
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
                    case Qt::Key_1:
                        emit dse->f1ButtonPressed();
                        break;
                    case Qt::Key_2:
                        emit dse->f2ButtonPressed();
                        break;
                    case Qt::Key_Left:
                        emit dse->leftRot();
                        break;
                    case Qt::Key_Right:
                        emit dse->rightRot();
                        break;
                    case Qt::Key_Return:
                        emit dse->pushRot();
                        break;
                    default:
                        handled = false;
                        break;
                }
            }
            else if (event->type() == QEvent::KeyRelease)
            {
                switch (keyEvent->key())
                {
                    case Qt::Key_Escape:
                        emit dse->outButtonReleased();
                        break;
                    case Qt::Key_E:
                        emit dse->errButtonReleased();
                        break;
                    case Qt::Key_S:
                        emit dse->settingsButtonReleased();
                        break;
                    case Qt::Key_1:
                        emit dse->f1ButtonReleased();
                        break;
                    case Qt::Key_2:
                        emit dse->f2ButtonReleased();
                        break;
                    case Qt::Key_Return:
                        emit dse->pushRotReleased();
                        break;
                    default:
                        handled = false;
                        break;
                }
            }
            else
                handled = false;
            if (handled)
                qDebug() << "text" << keyEvent->text() << "key" << keyEvent->key() << "modifiers"
                         << keyEvent->modifiers();
        }
        QObject::eventFilter(watched, event);
        return false;
    }
};
