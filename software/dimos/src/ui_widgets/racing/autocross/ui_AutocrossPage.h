#pragma once
#include <QLabel>

namespace ui
{
class AutocrossPage
{
  public:
    static QFrame *setupUi()
    {
        const auto itemFrame = new QFrame();
        new QLabel("Autocross Page", itemFrame);
        return itemFrame;
    }
};
} // namespace ui