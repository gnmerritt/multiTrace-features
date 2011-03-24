#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "multiTrace.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Cortex::ptr createCortexFromParameters();

private:
    Ui::MainWindow *ui;

// variables we're collecting to build a Cortex
    int layerRowSize, layerColSize;
    int numberOfLayers;
    int connectTo, connectFrom;
    bool interLayerConnectionsEnabled;
    QString testName, saveDirectory;
// index variables for the three selection boxes
    int connectionTemplate_i, learningRule_i, updateModel_i;

private slots:
    void on_saveLocation_textEdited(QString );
    void on_testName_textEdited(QString );
    void on_updateModel_currentIndexChanged(int index);
    void on_learningRule_currentIndexChanged(int index);
    void on_connectionTemplate_currentIndexChanged(int index);
    void on_layerConnectedToggle_toggled(bool checked);
    void on_connectTo_valueChanged(int );
    void on_connectFrom_valueChanged(int );
    void on_numberLayers_valueChanged(int );
    void on_layerRowSize_valueChanged(int );
    void on_createButton_clicked();
};

#endif // MAINWINDOW_H
