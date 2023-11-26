#pragma once
#include <QLabel>

namespace ui {
	class EndurancePage {
	public:
		static std::unique_ptr<QFrame> setupUi() {
			auto itemFrame = std::make_unique<QFrame>();
			auto * itemLayout = new QVBoxLayout(itemFrame.get());
			auto * itemLabel = new QLabel("Endurance Page");
			itemLabel->setAlignment(Qt::AlignCenter);
			itemLabel->setStyleSheet("color: white;");
			itemLayout->addWidget(itemLabel);
			itemFrame->setLayout(itemLayout);
			return itemFrame;
		}
	};
}