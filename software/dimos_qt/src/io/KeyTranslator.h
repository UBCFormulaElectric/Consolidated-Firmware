#pragma once

#include <QObject>
#include <qevent.h>
#include "ui/dimswitches/DimSwitchEmitter.h"

class KeyTranslator final : public QObject
{
    Q_OBJECT
  public:
    KeyTranslator() = default;

    bool eventFilter(QObject *watched, QEvent *event) override;
};
