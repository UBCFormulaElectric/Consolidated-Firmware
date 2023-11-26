#pragma once

#include <QGridLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace ui {
	class RacingBase {
	public:
		std::unique_ptr<QFrame> notifFrame;
		std::unique_ptr<QFrame> innerElements;
		void setupUi(QWidget * parent, std::unique_ptr<QFrame> innerElements) {
			this->innerElements = std::move(innerElements);
			auto layout = QGridLayout(parent);
			layout.addWidget(notifFrame.get(), 0, 0, 1, 3);
			layout.addWidget(innerElements.get(), 3, 0, 1, 2);
		}
	};
}

QT_END_NAMESPACE