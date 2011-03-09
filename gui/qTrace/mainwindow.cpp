#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cortexviewer.h"

#include <stdio.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    layerRowSize(0),
    layerColSize(0),
    numberOfLayers(0),
    connectTo(0),
    connectFrom(0),
    interLayerConnectionsEnabled(true),
    testName(""),
    saveDirectory(""),
    connectionTemplate_i(0),
    learningRule_i(0),
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
Cortex<UNR, SonntagUpdate, NoLearning>::ptr MainWindow::createCortexFromParameters() {
    // TODO: figure out the templating
    Cortex<UNR, SonntagUpdate, NoLearning>::ptr newCortex
            ( new Cortex<UNR, SonntagUpdate, NoLearning>(numberOfLayers, layerRowSize,  layerColSize, connectTo, connectFrom, testName.toStdString(), saveDirectory.toStdString() ) );

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
