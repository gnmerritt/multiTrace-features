#include "cortexviewer.h"
#include "ui_cortexviewer.h"

const int LAYER_IMG_WIDTH = 300;
const int LAYER_IMG_HEIGHT = 300;

CortexViewer::CortexViewer(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::CortexViewer),
	inputLayer(0),
	inputRow(0),
	inputCol(0)
{
	ui->setupUi(this);
}

CortexViewer::~CortexViewer()
{
	delete ui;
}

void CortexViewer::update()
{
	QImage img (LAYER_IMG_WIDTH, LAYER_IMG_HEIGHT, QImage::Format_RGB32);

	Layer::vector *layers = thisCortex->getLayers();
	Layer::ptr top = layers->front();

	// pixels per Assembly, how big each Assembly's output is drawn
	int ppa_cols = LAYER_IMG_WIDTH / top->cols;
	int ppa_rows = LAYER_IMG_HEIGHT / top->rows;

	Layer::LayerOutput* layerOutput = top->getOutputBlock();

	for (int col = 0; col < LAYER_IMG_WIDTH; ++col) {
		int assemblyCol = col / ppa_cols;

		for (int row = 0; row < LAYER_IMG_HEIGHT; ++row) {
			int assemblyRow = row / ppa_rows;

			float assemblyOutput = layerOutput->at(assemblyRow).at(assemblyCol);

			QColor c;
			c.setRgbF(assemblyOutput, assemblyOutput, assemblyOutput);

			img.setPixel(col, row, c.rgb());
		}
	}
	QPixmap pix;
	pix.convertFromImage(img);
	ui->layerImg->setFixedSize(LAYER_IMG_WIDTH, LAYER_IMG_HEIGHT);
	ui->layerImg->setPixmap(pix);
	ui->layerImg->repaint();
}

void CortexViewer::setCortex(Cortex::ptr newCortex) {
	thisCortex = newCortex;

	Layer::vector *layers = thisCortex->getLayers();
	Layer::ptr top = layers->front();

	// set up min/max for manual input
	ui->inputLayer->setMaximum(layers->size() - 1);
	ui->inputRow->setMaximum(top->rows - 1);
	ui->inputCol->setMaximum(top->cols - 1);

	update();
}

void CortexViewer::on_tickButton_clicked()
{
	thisCortex->tick();
	update();
}

/**
  * @brief Uses values from spinner boxes, sets the input of a target Assembly
  * @see Cortex::triggerAssembly
  * @return void
  */
void CortexViewer::on_addInput_clicked()
{
	thisCortex->triggerAssembly(inputLayer, inputRow, inputCol, inputStrength);
}

void CortexViewer::on_inputLayer_valueChanged(int value)
{
	inputLayer = value;
}

void CortexViewer::on_inputRow_valueChanged(int value)
{
	inputRow = value;
}

void CortexViewer::on_inputCol_valueChanged(int value)
{
	inputCol = value;
}

void CortexViewer::on_inputStrength_valueChanged(double value)
{
	inputStrength = value;
}
