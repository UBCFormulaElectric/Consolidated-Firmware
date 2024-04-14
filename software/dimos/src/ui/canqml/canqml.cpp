#include "canqml.h"

CanQML *CanQML::getInstance()
{
    static auto *instance = new CanQML();
    return instance;
}

CanQML *CanQML::create(const QQmlEngine *qmlEngine, const QJSEngine *jsEngine)
{
    Q_UNUSED(qmlEngine)
    Q_UNUSED(jsEngine)
    return getInstance();
}
