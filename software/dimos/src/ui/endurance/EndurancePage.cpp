#include "EndurancePage.h"


EndurancePage::EndurancePage(QWidget * parent):
	RacingBase(parent, std::move(ui::EndurancePage::setupUi()))
{
}