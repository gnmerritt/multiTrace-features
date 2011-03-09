#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cortexviewer.h"

#include <stdio.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
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
void MainWindow::createCortexFromParameters() {
}

void MainWindow::on_createButton_clicked()
{
    createCortexFromParameters();
    // TODO: spawn cortexviewer window with pointer to current Cortex

    CortexViewer *cv = new CortexViewer();
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
