#include <stdio.h>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "cortexviewer.hpp"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow),
	layerRowSize(6),
	layerColSize(6),
	numberOfLayers(1),
	connectTo(1),
	connectFrom(0),
	interLayerConnectionsEnabled(false),
	lateralInhibition(true),
	testName(""),
	saveDirectory(""),
	connectionTemplate_i(0),
	learningRule_i(1),
	updateModel_i(0)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;
}

/**
  * Looks at our Cortex parameters (set by various buttons/spinners in mainwindow)
  * and builds a Cortex object appropriately.
  *
  * @return shared pointer to the instantiated Cortex object
  */
Cortex::ptr MainWindow::createCortexFromParameters() {
    Cortex::ptr newCortex ( new Cortex() );

    // add all the Layers
    for (int layer = 0; layer < numberOfLayers; ++layer) {
	newCortex->addLayer((ConnectionPatterns::classes)connectionTemplate_i,
			    (UpdateModels::classes)updateModel_i,
			    (LearningRules::classes)learningRule_i,
			    Cortex::DEFAULT_LAYER, layerRowSize, layerColSize,
			    interLayerConnectionsEnabled, lateralInhibition);
    }

    // and then connect them
    Layer::vector layers = newCortex->getLayers();
    Layer::vector::iterator eachLayer;

    for (eachLayer = layers.begin(); eachLayer != layers.end(); ++eachLayer) {
	Layer::ptr thisLayer = *eachLayer;
	newCortex->connectLayerRange(thisLayer->getId(), connectFrom, connectTo);
    }

    newCortex->setTestName(testName.toStdString());
    newCortex->setLogLocation(saveDirectory.toStdString());

    return  newCortex;
}

void MainWindow::on_createButton_clicked()
{
	CortexViewer *cv = new CortexViewer();
	cv->setCortex(createCortexFromParameters());
	cv->show();
}

void MainWindow::on_layerRowSize_valueChanged(int _layerRowSize)
{
	layerRowSize = _layerRowSize;
}

void MainWindow::on_numberLayers_valueChanged(int _numberOfLayers)
{
	numberOfLayers = _numberOfLayers;
}

void MainWindow::on_connectFrom_valueChanged(int _connectFrom)
{
	connectFrom = _connectFrom;
}

void MainWindow::on_connectTo_valueChanged(int _connectTo)
{
	connectTo = _connectTo;
}

void MainWindow::on_layerConnectedToggle_toggled(bool checked)
{
	interLayerConnectionsEnabled = checked;
}

void MainWindow::on_connectionTemplate_currentIndexChanged(int index)
{
	connectionTemplate_i = index;
}

void MainWindow::on_learningRule_currentIndexChanged(int index)
{
	learningRule_i = index;
}

void MainWindow::on_updateModel_currentIndexChanged(int index)
{
	updateModel_i = index;
}

void MainWindow::on_testName_textEdited(QString newString)
{
	testName = newString;
}

void MainWindow::on_saveLocation_textEdited(QString newString)
{
	saveDirectory = newString;
}

void MainWindow::on_layerColSize_valueChanged(int _layerColSize)
{
	layerColSize = _layerColSize;
}

void MainWindow::on_localInhibitionEnabled_clicked(bool checked)
{
	lateralInhibition = checked;
}
