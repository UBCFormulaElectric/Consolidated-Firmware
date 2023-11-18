#pragma once

#include <QMainWindow>
#include <QTimer>
#include "landing/LandingPage.h"

QT_BEGIN_NAMESPACE
namespace ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow() override;

private:
	std::unique_ptr<LandingPage> l;
	std::unique_ptr<ui::MainWindow> ui;

	void setupCan();
	[[noreturn]] static void CanRXTask();
	[[noreturn]] static void CanPeriodicTXTask();
	QTimer tx100Hz{};
	QTimer tx1Hz{};
	QThread *CanRxTaskThread;
	QThread *CanTxPeriodicTaskThread;
};