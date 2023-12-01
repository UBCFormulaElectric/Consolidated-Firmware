#pragma once
#include <QLabel>

namespace ui {
	class BrakingPage {
	public:
		static QFrame * setupUi() {
			const auto itemFrame = new QFrame();
			new QLabel("Braking Page", itemFrame);
			return itemFrame;
		}
	};
}