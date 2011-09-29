#include "cortexviewer.hpp"
#include "ui_cortexviewer.h"

CortexViewer::CortexViewer(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CortexViewer),
	tickPause_s(0.01f),
	isRunning(false)
{
	ui->setupUi(this);
}

CortexViewer::~CortexViewer()
{
	delete ui;
}

void CortexViewer::setCortex(Cortex::ptr newCortex) {
	cortexObj.setCortex(newCortex);

	cortexObj.connect(this,
			  SIGNAL(tick()),
			  SLOT(tick()));

	cortexThread = new QThreadEx();

	cortexObj.moveToThread(cortexThread);

	cortexThread->start();

	update();
}

void CortexViewer::update() {
	// empty for now
}

void CortexViewer::on_tickButton_clicked()
{
	emit tick();
	update();
}

/**
  * @brief Starts/stops Cortex::tick(), with a pause of tickPause_s
  * @return void
  */
void CortexViewer::on_runStopButton_clicked()
{
	// already running, so we stop
	if (isRunning) {
		isRunning = false;
		ui->runStopButton->setText("Multi-Tick");
	}
	else {
		isRunning = true;
		ui->runStopButton->setText("Pause");

		/// @todo MAKE ME THREADED!
		for (int i = 0; i < ui->tickTimes->value(); ++i) {
			emit tick();
			update();
			sleep(tickPause_s);
		}

		ui->runStopButton->setText("Multi-Tick");
		isRunning = false;
	}
}

void CortexViewer::on_tickPause_valueChanged(double value)
{
	tickPause_s = value;
}
