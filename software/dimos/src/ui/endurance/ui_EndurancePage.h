#pragma once
#include <QLabel>

namespace ui {
	class EndurancePage {
	public:
		static QFrame * setupUi() {
			const auto itemFrame = new QFrame();
			new QLabel("Endurance Page", itemFrame);
			return itemFrame;
		}
	};
}