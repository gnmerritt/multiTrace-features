#include "cortexviewer.h"
#include "ui_cortexviewer.h"

const int LAYER_IMG_WIDTH = 300;
const int LAYER_IMG_HEIGHT = 300;

CortexViewer::CortexViewer(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CortexViewer),
	tickPause_s(0.25f),
	isRunning(false)
{
	ui->setupUi(this);
}

CortexViewer::~CortexViewer()
{
	delete ui;
}

void CortexViewer::setCortex(Cortex::ptr newCortex) {
	thisCortex = newCortex;

	Layer::vector *layers = thisCortex->getLayers();
	Layer::ptr top = layers->front();

	update();
}

void CortexViewer::update() {


}

void CortexViewer::on_tickButton_clicked()
{
	thisCortex->tick();
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
		ui->runStopButton->setText("Run");
	}
	else {
		isRunning = true;
		ui->runStopButton->setText("Pause");

		/// @todo MAKE ME THREADED!
		for (int i = 0; i < 10; ++i) {
			thisCortex->tick();
			update();
			//sleep(tickPause_s);
		}

		ui->runStopButton->setText("Run");
		isRunning = false;

		/// @todo FIGURE OUT QT THREADING
	}
}

void CortexViewer::on_tickPause_valueChanged(double value)
{
	tickPause_s = value;
}
