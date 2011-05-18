#include <time.h>

#include "layerviewer.hpp"
#include "ui_layerviewer.h"

static const int IMAGE_MAX = 300;

LayerViewer::LayerViewer(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LayerViewer),
	windowSizeX(0),
	windowSizeY(0),
	inputRow(0),
	inputCol(0),
	inputLayer(0),
	inputStrength(0.6f),
	stimulateRandom(0.2f)
{
	ui->setupUi(this);
	srand ( time(NULL) );
}

LayerViewer::~LayerViewer()
{
	delete ui;
}

void LayerViewer::setLayer(Layer::ptr _thisLayer) {
	thisLayer = _thisLayer;
	thisLayer_id = thisLayer->getId();

	ui->inputRow->setMaximum(thisLayer->rows - 1);
	ui->inputCol->setMaximum(thisLayer->cols - 1);

	setWindowTitle("multiTRACE Layer " + QString::number(thisLayer->getId()));

	// set size for Layer visualization window (pixels per Assembly)
	int xSizeGuess = static_cast<int>(IMAGE_MAX / thisLayer->cols);
	windowSizeX = xSizeGuess * thisLayer->cols;

	int ySizeGuess = static_cast<int>(IMAGE_MAX / thisLayer->rows);
	windowSizeY = ySizeGuess * thisLayer->rows;

	update();
}

/**
 * Updates the Layer Visualization with new values from the Layer.
 *
 */
void LayerViewer::update()
{
	QImage img (windowSizeX, windowSizeY, QImage::Format_RGB32);

	// pixels per Assembly, how big each Assembly's output is drawn
	int ppa_cols = windowSizeX / thisLayer->cols;
	int ppa_rows = windowSizeY / thisLayer->rows;

	Layer::LayerOutput* layerOutput = thisLayer->getOutputBlock();

	for (int col = 0; col < windowSizeY; ++col) {
		int assemblyCol = col / ppa_cols;

		for (int row = 0; row < windowSizeX; ++row) {
			int assemblyRow = row / ppa_rows;

			float assemblyOutput = layerOutput->at(assemblyRow).at(assemblyCol);

			QColor c;
			c.setRgbF(assemblyOutput, assemblyOutput, assemblyOutput);

			img.setPixel(col, row, c.rgb());
		}
	}

	QPixmap pix;
	pix.convertFromImage(img);
	ui->layerImg->setFixedSize(windowSizeX, windowSizeY);
	ui->layerImg->setPixmap(pix);
	ui->layerImg->repaint();
}

/**
  * @brief stimulates approximately stimulateRandom % of the Assemblies in this Layer
  */
void LayerViewer::stimulateRandomAssemblies() {
	int rows = thisLayer->rows;
	int cols = thisLayer->cols;

	for (int r = 0; r < rows; ++r) {
		for (int c = 0; c < cols; ++c) {
			float random = rand()/double(RAND_MAX);

			if (random < stimulateRandom) {
				thisLayer->triggerAssembly(r, c, inputStrength);
			}
		}
	}
}

/**
  * @brief Uses values from spinner boxes, sets the input of a target Assembly
  * @see Cortex::triggerAssembly
  * @return void
  */
void LayerViewer::on_addInput_clicked()
{
	thisLayer->triggerAssembly(inputRow, inputCol, inputStrength);
}


void LayerViewer::on_inputStrength_valueChanged(double _inputStrength)
{
	inputStrength = _inputStrength;
}

void LayerViewer::on_inputRow_valueChanged(int _inputRow)
{
	inputRow = _inputRow;
}

void LayerViewer::on_inputCol_valueChanged(int _inputCol)
{
	inputCol = _inputCol;
}

void LayerViewer::on_stimulateRandom_valueChanged(double _random)
{
	stimulateRandom = _random * .01f;
}

void LayerViewer::on_pushButton_clicked()
{
	stimulateRandomAssemblies();
}
