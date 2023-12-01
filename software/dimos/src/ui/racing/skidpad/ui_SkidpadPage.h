#pragma once
#include <QLabel>

namespace ui {
	class SkidpadPage {
	public:
		static QFrame * setupUi() {
			const auto itemFrame = new QFrame();
			new QLabel("Skidpad Page", itemFrame);
			return itemFrame;
		}
	};
}