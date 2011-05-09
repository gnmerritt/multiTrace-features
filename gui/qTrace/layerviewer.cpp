#include "layerviewer.hpp"
#include "ui_layerviewer.h"

LayerViewer::LayerViewer(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::LayerViewer),
	inputRow(0),
	inputCol(0),
	inputLayer(0)
{
	ui->setupUi(this);
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

	// set size for Layer visualization window
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

	FastGaussian::LayerOutput* layerOutput = thisLayer->getOutputBlock();

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
