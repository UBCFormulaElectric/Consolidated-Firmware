#pragma once

#include <QGridLayout>
#include <QWidget>

QT_BEGIN_NAMESPACE

namespace ui {
	class RacingBase {
	public:
		QFrame* notifFrame;
		QFrame* innerElements;
		void setupUi(QWidget * parent, QFrame * innerElements) {
			this->innerElements = innerElements;
			auto layout = QGridLayout(parent);
			layout.addWidget(notifFrame, 0, 0, 1, 3);
			layout.addWidget(innerElements, 3, 0, 1, 2);
		}
	};
}

QT_END_NAMESPACE