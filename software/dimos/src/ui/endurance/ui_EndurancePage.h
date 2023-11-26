#pragma once
#include <QLabel>

namespace ui {
	class EndurancePage {
	public:
		static QFrame * setupUi() {
			const auto itemFrame = new QFrame();
			auto * itemLayout = new QVBoxLayout(itemFrame);
			auto * itemLabel = new QLabel("Endurance Page");
			itemLabel->setAlignment(Qt::AlignCenter);
			itemLabel->setStyleSheet("color: white;");
			itemLayout->addWidget(itemLabel);
			itemFrame->setLayout(itemLayout);
			return itemFrame;
		}
	};
}