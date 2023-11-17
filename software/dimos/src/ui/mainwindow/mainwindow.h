#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = nullptr);

	~MainWindow() override;

private:
	std::unique_ptr<Ui::MainWindow> ui;

	[[noreturn]] static void CanRXTask();

	[[noreturn]] static void CanPeriodicTXTask();

	QTimer tx100Hz{};
	QTimer tx1Hz{};
	QThread *CanRxTaskThread;
	QThread *CanTxPeriodicTaskThread;
};

#endif // MAINWINDOW_H
