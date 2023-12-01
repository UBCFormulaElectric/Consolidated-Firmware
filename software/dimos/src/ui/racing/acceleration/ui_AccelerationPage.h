#pragma once
#include <QLabel>

namespace ui {
	class AccelerationPage {
	public:
		static QFrame * setupUi() {
			const auto itemFrame = new QFrame();
			new QLabel("Acceleration Page", itemFrame);
			return itemFrame;
		}
	};
}