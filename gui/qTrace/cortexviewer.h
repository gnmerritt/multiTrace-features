#ifndef CORTEXVIEWER_H
#define CORTEXVIEWER_H

#include <QWidget>

#include "multiTrace.hpp"
#include "QtUtility.hpp"
#include "layerviewer.hpp"

namespace Ui {
	class CortexViewer;
}

class CortexViewer : public QWidget
{
	Q_OBJECT

public:
	explicit CortexViewer(QWidget *parent = 0);
	~CortexViewer();

	void setCortex(Cortex::ptr);

private:
	void update();

private:
	Ui::CortexViewer *ui;
	QThreadEx cortexThread; /// @TODO!

	Cortex::ptr thisCortex;

	float tickPause_s;
	bool isRunning;

private slots:
	void on_runStopButton_clicked();
	void on_tickPause_valueChanged(double );
	void on_tickButton_clicked();

signals:
	void tick();
};

#endif // CORTEXVIEWER_H
